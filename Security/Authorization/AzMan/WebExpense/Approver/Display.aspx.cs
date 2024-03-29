// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//
//	The page displays the contents of an expense report
//	The expense report is selected from List.aspx and passed to
//	this page via a POST
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
	/// Display Page Class
	///	Contains all methods, events and properties of the Display page
	/// </summary>
	public partial class Display : System.Web.UI.Page
	{
		#region Page Variables
		/// <summary>
		/// Approval Decision Radio Button List - User selects either
		/// approve or reject an expense
		/// </summary>
		
		/// <summary>
		/// Submit Desicion Button - Users click this button to submit
		/// their decision to either approve or reject the expense
		/// </summary>
		
		/// <summary>
		/// Decision Group Panel - Contains all the UI controls
		///	to submit or reject a decision
		/// </summary>
		
		/// <summary>
		/// Comment TextBox - Textbox for a user to attach a comment
		/// with the expense report when it is submitted
		/// </summary>

		/// <summary>
		/// Transaction Data Table - Contains all the data
		/// related to the expese report being reviewed
		/// </summary>
		
		/// <summary>
		/// Status Label - Displays the status (approved, pending, etc)
		/// of the expense report
		/// </summary>
		
		/// <summary>
		/// Logo Link - Displays the company logo and links back to the
		/// main page of the application
		/// </summary>
		
		/// <summary>
		/// Title Label - Displays the title of the page
		/// </summary>
		
		/// <summary>
		/// Comment Label - Identifies the comment textbox
		/// </summary>
		
		/// <summary>
		/// Return Link - Link to the main page of the application
		/// </summary>
		
		/// <summary>
		/// Expense Report Data - Name-Value collection that stores
		/// all the information related to the expense report
		/// </summary>
		protected StringDictionary ExpenseData;

		/// <summary>
		/// Message Label - Displays any messages to the user
		/// </summary>
		
		/// <summary>
		/// Transaction ID - The ID of the current expense report being
		/// reviewed
		/// </summary>
		protected int transID;
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
			//	Get the transaction ID from the posted variables
			//	if no transaction ID was posted, i.e. the user
			//	went directly to this page and not from the List.aspx page,
			//	show an error message
			//
			try
			{
				transID = Convert.ToInt32(Request.QueryString["transactionId"]);
			}
			catch(System.ArgumentNullException)
			{
				Message.Text = "There was an error retrieving the transaction ID.";
				return;
			}

			//
			//	Get the expense report data from the transaction ID
			//	and check that the transaction exists
			//
			ExpenseData = ExpenseCommon.GetTransData( transID );
			if(ExpenseData == null)
			{
				Message.Text = "There was an error retrieving the transaction data. <br>The specified transaction does not exist.";
				return;
			}

			//		
			//	Check for this is the first time the page is being loaded
			//	only fill in the form if this is the first time otherwise
			//	any user changes will be lost
			//
			if(!Page.IsPostBack)
			{
				//
				//	Check if the approver is also the owner of the
				//	expense report and either display the report
				//	or a message depending on the Self Approval setting
				//	of the application
				//

				//
				//	Get the status of the transaction
				//
				string transStatus = string.Concat(ExpenseCommon.GetTransactionStatus(transID),"");
			
				//
				//	Check for the transaction has not been approved
				//
				if(transStatus == "")
				{
					//	
					//	Check if the current user is the owner of the expense report
					//
					if((ExpenseCommon.GetClientSamName() == ExpenseData["SamName"]))
					{
						//
						//	Check for Self Approval is allowed
						//
						if(ExpenseCommon.GetSelfApproval()==false)
						{
							//
							//	Self approval is not allowed so do not display the
							//	decision buttons and show an error message
							//
							Status.Text=string.Concat(Status.Text,"PENDING","<P>You are not authorized to approve your own expense.</p>");
							DecisionGroup.Visible=false;
						}
						else
						{
							//
							//	Self Approval is allowed so display the 
							//	decision buttons and it's status
							//
							Status.Text=string.Concat(Status.Text,"PENDING");
							DecisionGroup.Visible=true;
						}
					}
					else
					{
						//
						//	The current user is not the owner of the expense report so
						//	display the decision buttons
						//
						Status.Text=string.Concat(Status.Text,"PENDING");
						DecisionGroup.Visible=true;
					}
				}
				else
				{
					//
					//	The expense report has been reviewed already
					//	Display it's status and do not show the decision buttons
					//
					Status.Text=string.Concat(Status.Text,transStatus);
					DecisionGroup.Visible=false;
				}

				//
				//	Get the client context
				//
				IAzClientContext AzClient = ExpenseCommon.GetAzClientContext();

				//
				//	Check if the user has permission to list expenses
				//

				//
				//	Contains the scope of the access check request
				//	which is set to the application level (null)
				//
				object[] scope = new Object[1];
				scope[0] = (object)"";

				//
				//	Contains all the operations associated with 
				//	reading an expense
				//	In this case the readExpense operation
				//
				object[] operations = new Object[1];
				operations[0] = ExpenseCommon.AzopRead;

				//
				//	Contains all the parameter names associated with
				//	approving an expense.  These are organized as
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
				//	approving an expense.
				//
				Object[] BRValues = new Object[3];
                BRValues[0] = (object)0;
                BRValues[1] = (object)DateTime.Now.ToShortDateString();
                BRValues[2] = (object)ExpenseCommon.GetClientSamName();

				//	Contains the results from AccessCheck
				object[] results;
				try
				{
					//
					//	Run the access check on the administer operation
					//	Passing the audit text, scope, operations and business rule parameters
					//
					results = (object[])AzClient.AccessCheck("Read Expense Report", (object)scope,(object)operations,BRNames,BRValues,null,null,null);
				}
				catch(Exception ex)
				{
					Message.Text = "There was an error running AccessCheck: " + ex.Message;
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
							Message.Text = "Read Denied. " + errorMessage;
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

				if(bAuthorized)
				{
					//
					//	Display the expense report
					//
					DisplayExpense();
				}
				else
				{
					//
					//	Access Check failed so display an error message to the user
					//
					Message.Text = "Error Access Denied: " + AzClient.GetBusinessRuleString();
					return;
				}
			}
		}

		/// <summary>
		/// Displays the expense report data
		/// </summary>
		private void DisplayExpense()
		{
			try
			{
				//
				//	Fill in the expense report table with 
				//	the data from the transaction
				//

				//
				//	Fill in the user name / owner of the expense report
				//
				TableRow row = new TableRow();
				TableCell cell = new TableCell();
				cell.Text = ExpenseData["User"];
				row.Cells.Add(cell);
				
				//
				//	Fill in the description of the expense
				//
				cell = new TableCell();
				cell.Text = ExpenseData["Description"];
				row.Cells.Add(cell);
				
				//
				//	Fill in the date of the expense
				//
				cell = new TableCell();
				cell.Text = ExpenseData["Date"];
				row.Cells.Add(cell);
				
				//
				//	Fill in the amount of the expense
				//
				cell = new TableCell();
				cell.Text = ExpenseData["Amount"];
				row.Cells.Add(cell);
				
				//
				//	Fill in any comments attached to the expense report
				//
				cell = new TableCell();
				cell.Text = string.Concat(ExpenseData["Comment"],"&nbsp;");
				row.Cells.Add(cell);
				
				//
				//	Add the expense data to the table
				//
				TransData.Rows.Add(row);
			}
			catch(Exception)
			{
				this.Message.Text = "There was an error displaying the expense report.";
				return;
			}
		}


		/// <summary>
		/// Submit Decision Click - When the user clicks the Submit button,
		/// this submits the descision of the user, approve or reject,
		/// to the application data store.
		/// </summary>
		protected void SubmitDecision_Click(object sender, System.EventArgs e)
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
			//	In this case the approve operation
			//
			object[] operations = new Object[1];
			operations[0] = ExpenseCommon.AzopApprove;

			//
			//	Contains all the parameter names associated with
			//	approving an expense.  These are organized as
			//	name-value pairs and passed to the business rule
			//	if one is defined
			//
			Object[] BRNames = new Object[3];
			BRNames[0] = (object)ExpenseCommon.ParamAmount;
			BRNames[1] = (object)ExpenseCommon.ParamDate;
			BRNames[2] = (object)ExpenseCommon.ParamUserName;

			//
			//	Contains all the paramenter values associted with
			//	approving an expense.
			//
			Object[] BRValues = new Object[3];
			BRValues[0] = (object)ExpenseData["Amount"];
			BRValues[1] = (object)ExpenseData["Date"];
            BRValues[2] = (object)ExpenseCommon.GetClientSamName();


			//
			//	Run the access check on the administer operation
			//	Passing the audit text, scope, operations and business rule parameters
			//
			object[] results = (object[])AzClient.AccessCheck("Approve Expense Report", (object)scope,(object)operations,BRNames,BRValues,null,null,null);

			//
			//	Check for success of the access check
			//
			bool bAuthorized = true;
			foreach(int iResCode in results)
			{
				if(iResCode != ExpenseCommon.NoError)
				{
					bAuthorized = false;
					break;
				}
			}

			if(bAuthorized)
			{
				//
				//	Check for the user has added comments
				//
				if(Comment.Text.Length > 0)
				{
					//
					//	Attach the comment to the transaction
					//
					ExpenseData["Comment"] = string.Concat(ExpenseData["Comment"],
						"<p><b>Approver Comment: </b></p>", Comment.Text);
					ExpenseCommon.SaveTransaction(transID,ExpenseData);
				}

				//
				//	Check for the user approved or rejected the expense
				//
				if(Decision.SelectedValue=="approve")
				{
					//
					//	Approve the expense
					//
					ExpenseCommon.ApproveTransaction(transID);
				}
				else
				{
					//
					//	Reject the expense
					//
					ExpenseCommon.RejectTransaction(transID);
				}

				//
				//	Redirect the user to pending expenses list
				//
				Response.Redirect("List.aspx",true);
			}
			else
			{
				//
				//	If the access check failed, display an error message to the user
				//
				Message.Text = "Error Access Denied: " + AzClient.GetBusinessRuleString();
				return;
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