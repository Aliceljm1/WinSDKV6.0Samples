// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//
//	Admin
//	This page is used to administer application level setting for the demo.
//	An user defined as an administrator in the authorization policy store
//	can change the max number of transactions stored in the demo,
//	and allow or disallow users from approving their own expenses.
//
//
//	Carolyn Van Slyck 06/2003 - Created
//  DaveMM - Updates 06/2005 - Tweaks, Updates, Fixes for SDK



using System;
using System.Collections;
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
	/// ExpenseWebAdministration Page Class
	///	Contains all methods, events and properties of the ExpenseWebAdministration page
	/// </summary>
	public partial class ExpenseWebAdministration : System.Web.UI.Page
	{
		#region Page Variables
		/// <summary>
		/// Self Approval CheckBox - Check this box to allow users who are both
		/// approvers and submitters to approve their own expense
		/// </summary>


		/// <summary>
		/// ExpenseWebAdministration Group Panel - Container for all of the administrative
		/// UI controls on the page
		/// </summary>


		/// <summary>
		/// Maximum Number of Transactions
		/// The max number of transactions that can occur in the demo
		/// before the tranasction are deleted and the demo is restarted
		/// </summary>


		/// <summary>
		/// OK Button - Submits the changes made on the form and
		/// redirects to the main page
		/// </summary>


		/// <summary>
		/// Cancel Button - Cancels any changes made on the form and
		/// redirects to the main page
		/// </summary>


		/// <summary>
		/// Logo Hyperlink - A picture of the company logo which links
		/// back to the main page
		/// </summary>


		/// <summary>
		/// Administration Label - Title of the page, Administration
		/// </summary>


		/// <summary>
		/// Message Label - Displays any messages to the user
		/// </summary>


		/// <summary>
		/// Maximum Number of Transactions Label - Identifies the
		/// max trans textbox
		/// </summary>
		#endregion

		/// <summary>
		/// Page Load - This is executed when the page is first requested
		/// by the user and additionally when the user clicks a button on
		/// the form
		/// </summary>
		protected void Page_Load(object sender, System.EventArgs e)
		{
			//
			//	To prevent users from by-passing the portal page (index.aspx)
			//	and going directly to this page, use URL Authorization
			//	See <url> for details.
			//

			//		
			//	Check for this is the first time the page is being loaded
			//	only fill in the form if this is the first time otherwise
			//	any user changes will be lost
			//
			if(!Page.IsPostBack)
			{
				//
				//	Lock the application to ensure settings integrity
				//
				Application.Lock();

				//
				//	Show the admin UI controls
				//
				AdminGroup.Visible=true;

				//
				//	Load application settings
				//

				//
				//	Load the Self Approval setting
				//
				self_approval.Checked = ExpenseCommon.GetSelfApproval();

				//	
				//	Load the Maximum Number of Transactions setting
				//
				max_trans.Text=ExpenseCommon.GetMaxTransaction().ToString();
			}
		}
		

		/// <summary>
		/// Page UnLoad occurs when the user leave the page
		/// </summary>
		private void Page_UnLoad()
		{
			//unlock the application
			Application.UnLock();
		}	

		/// <summary>
		/// Cancel Button Click - When a user clicks the Cancel
		/// button discard any changes and redirect them to the main page
		/// </summary>
		protected void Cancel_Click(object sender, System.EventArgs e)
		{
			Response.Redirect("../index.aspx",true);
		}


		/// <summary>
		/// OK Button Click - When a user clicks the OK button
		/// save any changes and rediret them to the main page
		/// </summary>
		protected void OK_Click(object sender, System.EventArgs e)
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
			operations[0] = ExpenseCommon.AzopAdministrater;

			//
			//	Contains all the parameter names associated with
			//	application settings.  These are organized as
			//	name-value pairs and passed to the business rule
			//	if one is defined.
			//
			//	THEY MUST BE IN ALPHABETICAL ORDER (A-Z)
			//
			Object[] BRNames = new Object[3];
            BRNames[0] = (object)ExpenseCommon.ParamAmount;
            BRNames[1] = (object)ExpenseCommon.ParamDate;
            BRNames[2] = (object)ExpenseCommon.ParamUserName;
            
            //
			//	Contains all the paramenter values associted with
			//	the application settings.
			//
			Object[] BRValues = new Object[3];
			BRValues[0] = (object)0;
            BRValues[1] = (object)DateTime.Now.ToShortDateString();
            BRValues[2] = (object)ExpenseCommon.GetClientSamName();

			//
			//	Run the access check on the administer operation
			//	Passing the audit text, scope, operations and business rule parameters
			//
			object[] results = (object[])AzClient.AccessCheck("Change Application Settings", (object)scope,(object)operations,BRNames,BRValues,null,null,null);

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
						Message.Text = "Admin Denied. " + errorMessage;
					}
					else
					{
						Message.Text = "Access Denied.  You do not have sufficient permissions to perform this operation.";
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
					Message.Text = "There was an error performing the AccessCheck: " + ex.Message;
				}
			}
		
			if(bAuthorized == true)
			{
				//
				//	Save the Self Approval setting
				//
				ExpenseCommon.SetApproval(self_approval.Checked);

				//
				//	Save the Maximum Number of Tranascitons setting
				//
				ExpenseCommon.SetMaxTransaction(Convert.ToInt32(max_trans.Text));
			
				//
				//	Redirect the user to the main page
				//
				Response.Redirect("../index.aspx",false);
			}
			else
			{
				//
				//	Display reason for the access check failure
				//
				Message.Text = "Error Access Denied:" + AzClient.GetBusinessRuleString();
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