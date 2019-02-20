// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//
//	This page allows a user to submit an expense report.
//
//
//	Carolyn Van Slyck 06/2003 - Created
//  DaveMM - Updates 06/2005 - Tweaks, Updates, Fixes for SDK



using System;
using System.Collections;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using Microsoft.Interop.Security.AzRoles;

//
//	Application Namespace
//
namespace WebExpense
{
	/// <summary>
	/// Submit Page Class
	///	Contains all methods, events and properties of the Submit page
	/// </summary>
	public partial class Submit : System.Web.UI.Page
	{
		#region Page Variables
		/// <summary>
		/// Comment TextBox - Allows a user to attach a comment
		/// to the expense report
		/// </summary>

		/// <summary>
		/// Date TextBox - Date the expense was incurred
		/// </summary>

		/// <summary>
		/// Amount TextBox - Amount of the expense
		/// </summary>

		/// <summary>
		/// Description TextBox - Descriptiong of the expense
		/// </summary>

		/// <summary>
		/// Message Label - Displays any messages to the user
		/// </summary>

		/// <summary>
		/// Submit Group Panel - Contains the UI controls to submit
		/// an expense
		/// </summary>

		/// <summary>
		/// Logo Link - Displays the company logo and links back to
		/// the main page of the application
		/// </summary>

		/// <summary>
		/// Title Label - Displays the title of the page
		/// </summary>

		/// <summary>
		/// Description Label - Identifies the description textbox
		/// </summary>

		/// <summary>
		/// Amount Label - Identifies the amount textbox
		/// </summary>

		/// <summary>
		/// Date Label - Identifies the date textbox
		/// </summary>

		/// <summary>
		/// Comment Label - Identifies the comment textbox
		/// </summary>

		/// <summary>
		/// Submit Button - The user clicks this button to submit the expense
		/// </summary>

		/// <summary>
		/// Return Link - Link back to the main page of the application
		/// </summary>

		/// <summary>
		/// Description Validator - Requires that a description is entered
		/// A description cannot be longer than 50 characters
		/// </summary>

		/// <summary>
		/// Date Validator - Checks that the user entered a valid date between
		/// the range of 01/01/1900 and 12/31/2999
		/// </summary>

		/// <summary>
		/// Amount Validator - Checks that the user entered an amount
		/// </summary>

		/// <summary>
		/// Date Validator - Checks that the user entered a date
		/// </summary>

		/// <summary>
		/// Amount Validator - Checks that the user entered a valid amount
		/// The amount must be between 1 and 99999999999
		/// </summary>

		#endregion

		/// <summary>
		/// Submit Button Click - When the user clicks the submit button
		/// this saves the expense report in the application data store
		/// </summary>
		protected void SubmitBtn_Click(object sender, System.EventArgs e)
		{
			//
			//
			//	Get the client context from the session variables
			//
			IAzClientContext AzClient = ExpenseCommon.GetAzClientContext();

			//
			//	Check if the user has access to the administer
			//	operation and then save application settings
			//

			//
			//	Contains the scope of the access check request
			//	which is set to the application level (null)
			//
			object[] scope = new Object[1];
			scope[0] = (object)"";

			//
			//	Contains all the operations associated with 
			//	changing the application settings
			//	In this case the administer operation
			//
			object[] operations = new Object[1];
			operations[0] = ExpenseCommon.AzopSubmit;

			//
			//	Contains all the parameter names associated with
			//	submitting an expense.  These are organized as
			//	name-value pairs and passed to the business rule
			//	if one is defined
			//
			//	THEY MUST BE IN ALPHABETICAL ORDER (A-Z)
			//
			Object[] BRNames = new Object[3];
			BRNames[0] = (object)ExpenseCommon.ParamAmount;
			BRNames[1] = (object)ExpenseCommon.ParamDate;
			BRNames[2] = (object)ExpenseCommon.ParamUserName;

			//
			//	Contains all the paramenter values associted with
			//	the submitting an expense.
			//
			Object[] BRValues = new Object[3];
			BRValues[0] = (object)Amount.Text;
			BRValues[1] = (object)Date.Text;
			BRValues[2] = (object)ExpenseCommon.GetClientSamName();

			//	Contains the results from the AccessCheck
			object[] results;

			try
			{
				//
				//	Run the access check on the administer operation
				//	Passing the audit text, scope, operations and business rule parameters
				//
				results = (object[])AzClient.AccessCheck("Change Application Settings", (object)scope,(object)operations,BRNames,BRValues,null,null,null);
			}
			catch(Exception ex)
			{
				MSG.Text = "There was an error running the AccessCheck: " + ex.Message;
				return;
			}

			//
			//	Check for success of the access check
			//
			bool bAuthorized = true;
			foreach(int iResCode in results)
			{
				//
				//	Check for access denied
				//
				if(iResCode == ExpenseCommon.AccessDenied)
				{
					string errorMessage = AzClient.GetBusinessRuleString();
					if(errorMessage != "")
					{
						MSG.Text = "Submission Denied. " + errorMessage;
					}
					else
					{
						MSG.Text = "Access Denied.  You do not have sufficient permissions to perform this operation.";
					}
					bAuthorized = false;
					break;
				}
				//
				//	Check for other error
				//
				else if(iResCode != ExpenseCommon.NoError)
				{
					Win32Exception ex = new Win32Exception();
					MSG.Text = "There was an error performing the AccessCheck: " + ex.Message;
				}
			}
		
			if(bAuthorized)
			{
				//
				//	AccessCheck passed so submit the expense report
				//

				//
				//	Store the expense report in a name-value collection
				//
				StringDictionary ExpenseData = new StringDictionary();
				
				//
				//	Save the user SAM name (\\domain\username)
				//
				string name = ExpenseCommon.GetClientSamName();
				ExpenseData.Add("SamName", name);
				
				//
				//	Save the user Friendly Name
				//
				name = name.Substring((name.IndexOf(@"\")+1));
				ExpenseData.Add("User",name);

				//
				//	Save the transaction date
				//
				ExpenseData.Add("Date",Date.Text);

				//
				//	Save the expense description
				//
				ExpenseData.Add("Description",Description.Text);

				//
				//	Save the expense amount
				//
				ExpenseData.Add("Amount",Amount.Text);

				//
				// Attach any comments to the expense report
				//
				ExpenseData.Add("Comment",Comment.Text);

				//
				//	Save the transaction
				//
				ExpenseCommon.SaveTransaction(ExpenseCommon.AssignNextTransaction(), ExpenseData);
				
				//
				//	Show link to submit a new expense or
				//	to return to the main page
				//
				MSG.Text="Submission Sucessful.<p><a href='Submit.aspx'>Submit new expense</a> | <a href='../index.aspx'>Return to Main Menu</a></p>";

				//
				//	Clear form for new entry
				//
				Description.Text="";
				Amount.Text="";
				Date.Text="";
				Comment.Text="";

				SubmitGroup.Visible=false;
			}
		}

		#region Web Form Designer generated code
		override protected void OnInit(EventArgs e)
		{
			//
			// CODEGEN: This call is required by the ASP.NET Web Form Designer.
			//
			InitializeComponent();
			base.OnInit(e);
		}
		
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{    

		}
		#endregion
	}
}