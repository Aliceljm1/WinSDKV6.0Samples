// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//	This page can list all expenses in the application
//	data store and sort them by their status, i.e. approved, rejected, etc.
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
    /// List Page Class
    ///	Contains all methods, events and properties of the List page
    /// </summary>
    public partial class List : System.Web.UI.Page
    {
        #region Page Variables
        /// <summary>
        /// Message Label - Displays any messages for the user
        /// </summary>

        /// <summary>
        /// Transaction List - Displays the transactions
        /// filtered by the ModeSelect drop down listbox
        /// </summary>

        /// <summary>
        /// Mode Select Drop Down ListBox - Filters which
        /// expense transactions are displayed in the transaction list
        /// </summary>

        /// <summary>
        /// Mode Label - Displays the filter on the transaction list
        /// </summary>

        /// <summary>
        /// Logo Link - Displays the company logo and links
        /// back to the main page of the application
        /// </summary>

        /// <summary>
        /// Title Label - Displays the page title
        /// </summary>

        /// <summary>
        /// Return Link - Displays a link back to the main
        /// page of the application
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
                //	listing an expense
                //	In this case the list operation
                //
                object[] operations = new Object[1];
                operations[0] = ExpenseCommon.AzopList;

                //
                //	Contains all the parameter names. These are organized as
                //	name-value pairs and passed to the business rule
                //	if one is defined
                //
                //	THEY MUST BE IN ALPHABETICAL ORDER (A-Z)
                //
                Object[] BRNames = new Object[3];
                BRNames[0] = (object)ExpenseCommon.ParamUserName;
                BRNames[1] = (object)ExpenseCommon.ParamDate;
                BRNames[2] = (object)ExpenseCommon.ParamUserName;

                //
                //	Contains all the paramenter values

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
                    //	Run the access check on the list operation
                    //	Passing the audit text, scope, operations and business rule parameters
                    //
                    results = (object[])AzClient.AccessCheck("List Expense Reports", (object)scope,(object)operations,BRNames,BRValues,null,null,null);
                }
                catch(Exception ex)
                {
                    MSG.Text = "There was an error running AccessCheck: " + ex.Message;
                    throw(ex);
                }

                //
                //	Check for success of the access check
                //
                bool bAuthorized = true;
                foreach(int iResCode in results)
                {
                    //
                    //	Check for access denied for each operation
                    //
                    if(iResCode == ExpenseCommon.AccessDenied)
                    {
                        string errorMessage = AzClient.GetBusinessRuleString();
                        if(errorMessage != "")
                        {
                            MSG.Text = "List Denied. " + errorMessage;
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
                        Win32Exception ex = new Win32Exception( iResCode,"There was an error performing the AccessCheck: ");
                    }
                }

                if(bAuthorized)
                {
                    //
                    //	List the expense reports
                    //
                    ListTransactions();
                }
                else
                {
                    //
                    //	Access Check failed so display an error message to the user
                    //
                    MSG.Text = "Error Access Denied: " + AzClient.GetBusinessRuleString();
                    return;
                }
            }
        }

        /// <summary>
        /// Lists all transactions in the application data store
        /// according to the filter set by the ModeSelect drop down listbox
        /// </summary>
        private void ListTransactions()
        {
            //
            //	remove previous transactions from list
            //
            TransList.Rows.Clear();

            //
            //	Create the header row of the table
            //
            TableRow trow = new TableRow();
            trow.BackColor = System.Drawing.Color.LightSteelBlue;
            trow.Font.Bold = true;
            TableCell tcell = new TableCell();
            tcell.Text = "Select an expense";
            trow.Cells.Add(tcell);
            tcell = new TableCell();
            tcell.Text = "Status";
            trow.Cells.Add(tcell);
            TransList.Rows.Add(trow);

            //
            //	Get the number of transactions in the 
            //	application data store
            //
            int numTrans = ExpenseCommon.GetNextTransaction();
            
            //
            //	Check for a valid number of transactions
            //
            if(numTrans > 0)
            {
                //
                //	Check the transaction status filter	
                //
                if(mode.Text == "ALL")
                {
                    //
                    // Show all transactions
                    //
                    for(int i = 1; i <= numTrans; i++)
                    {
                        //
                        //	Create a new transaction entry
                        //
                        TableRow row = new TableRow();
                        TableCell cell = new TableCell();

                        //
                        //	Display a link to the transaction data
                        //
                        cell.Text=string.Concat("<a href='display.aspx?transactionId=",
                            i.ToString(), "'>Expense ", i.ToString());
                        row.Cells.Add(cell);
                        cell = new TableCell();

                        //
                        //	Display the transaction status
                        //
                        cell.Text=string.Concat(ExpenseCommon.GetTransactionStatus( i ), " ", ExpenseCommon.GetTransactionDecisionTime( i ));
                        row.Cells.Add(cell);
                        TransList.Rows.Add(row);
                    }
                }
                else 
                {
                    //
                    //	Only show transactions that match the status filter
                    //
                    for(int i = 1; i <= numTrans; i++)
                    {
                        //
                        //	only show transactions of the specified type 
                        //	(ie approved, denied, pending)
                        if(string.Concat(ExpenseCommon.GetTransactionStatus(i),"") == mode.Text)
                        {
                            //
                            //	Create a new transaction entry
                            //
                            TableRow row = new TableRow();
                            TableCell cell = new TableCell();
                            
                            //
                            //	Display a link to the transaction data
                            //
                            cell.Text=string.Concat("<a href='display.aspx?transactionId=",
                                i.ToString(), "'>Expense ", i.ToString());
                            row.Cells.Add(cell);
                            cell = new TableCell();

                            //
                            //	Display the transaction status
                            //
                            cell.Text=string.Concat(ExpenseCommon.GetTransactionStatus( i ), " ", ExpenseCommon.GetTransactionDecisionTime( i ));
                            row.Cells.Add(cell);
                            TransList.Rows.Add(row);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Mode Select Selected Index Changed
        /// When the user selects a new transaction status filter
        /// refresh the list of transactions according to the new filter
        /// </summary>
        protected void ModeSelect_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            //
            //	Change the filter label to the new filter (ie approved, denied, pending, all)
            //
            mode.Text = ModeSelect.SelectedValue;
            
            //
            //	Relist the transactions according to the 
            //	new filter
            //
            ListTransactions();
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
