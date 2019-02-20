/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) Microsoft Corporation.  All rights reserved.

Module Name:

    form1.cs
 
Abstract:

	This module contains the skeleton code for a Windows managed 
	code Winform application that is Restart Manager aware and 
    supports a simple document recovery scenario

Notes:
    This application requires Windows Vista

    This application builds ontop of the visual studio 2005 C# winform
    template

    This sample relies on being shutdown and restarted by either a 
	Restart Manager API enabled application or by the Windows Error 
    Reporting service

    This sample provides a simple data storage / restoration mechanism 
    to enable a document recovery experience

 History:

     23-Nov-2005    a-paruss

        Creation of this sample application

--*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace rmwinformapp
{
    public partial class Form1 : Form
    {

        //
        // Winuser.h constants needed for the WndProc override method call
        //
        private const int WM_QUERYENDSESSION = 0x11;
        private const int ENDSESSION_CLOSEAPP = 0x1;
        private const int ENDSESSION_LOGOFF = 0x8;
        private const int WM_CREATE = 0x1;

        //
        // global commandline value
        //
        public string CommandLineParameter = "";

        /*++

        Routine Description:

        This routine provides the Form1 Winform primary entry point

        --*/
        public Form1()
        {
            InitializeComponent();
        }

        /*++

        Routine Description:

        This routine provides the Form1 load routine

        --*/
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        /*++

        Routine Description:

        This routine provides the WncProc message handler

        --*/
        protected override void WndProc(ref Message m)
        {
            bool ret;


            if (m.Msg == WM_CREATE)
            {
                if (App.ArgumentCount == 1)
                {
                    LoadData();
                }
            }
            else if (m.Msg == WM_QUERYENDSESSION)
            {
                switch ((int)m.LParam)
                {
                    //
                    // This application is being told to shutdown by Restart Manager (an installer 
                    // needs to replace files this app has in use)
                    //
                    case ENDSESSION_CLOSEAPP:

                        //
                        // check the rich text box control for the text length.
                        // - if length > 0 we know there is text so we will then 
                        //   save data
                        //
                        if (richTextBox1.Text.Length > 0)
                        {
                            //
                            // - serialize data that the user typed in the edit control	
                            //
                            ret = SaveData();
                            if (ret != true)
                            {
                                // 
                                // log an event or do some error handling
                                //
                            }
                        }

                        // - register for restart
                        ret = RegisterForRecovery();
                        if (ret != true)
                        {
                            //
                            // log an event or do some error handling
                            //
                        }
                        else
                        {
                            m.Result = (System.IntPtr)1; //Sets the value to true that is to be returned to Windows in response to handling the message.
                        }
                        return; 

                    //
                    // This application is being told to shutdown by the system (the user 
                    // is logging off or the system is shutting down)
                    //
                    case ENDSESSION_LOGOFF:
                        //
                        // system shutdown/user logoff is not covered in this sample.
                        //

                        //
                        // TIP: do some "is dirty" kind of checks on the edit control and 
                        // if true prompt user for to save data ... save / ignore based upon user input
                        //

                        return;
                    default:
                        return;
                }

            }
            else
            {
                base.WndProc(ref m);
            }
        }

        /*++

        Routine Description:

        This routine provides the common registeration recovery calls

        --*/
        bool RegisterForRecovery()
        {
            int Result;

            //
            // Register to be restarted
            //
            Result = Win32.RegisterApplicationRestart(CommandLineParameter, 0);

            if (Result != 0)
            {
                //MessageBox.Show("regier failed");
                return false;
            }
            //MessageBox.Show("regier succeeded");
            return true;
        }

        /*++

        Routine Description:

        This routine provides data recovery support

        --*/
        bool SaveData()
        {
            //
            // obtain a temporary file name
            //
            CommandLineParameter = Path.GetTempFileName();

            // create a file stream to the temporary file
            using (FileStream fs = File.Create(CommandLineParameter))
            {
                //
                // initialize document byte object and copy the text from the text box control
                //
                Byte[] DocumentText = new UTF8Encoding(true).GetBytes(richTextBox1.Text);

                //
                // flush edit control data to the recovery file
                //
                fs.Write(DocumentText, 0, DocumentText.Length);
            }

            return true;
        }


        /*++

        Routine Description:

        This routine provides data restoration support

        --*/
        bool LoadData()
        {
            //
            // open a stream reader against the recovery file
            //
            using (StreamReader sr = new StreamReader(App.CommandLine))
            {
                //
                // read the stream and populate the rich text control
                //
                string s = "";
                while ((s = sr.ReadLine()) != null)
                {
                    richTextBox1.Text = richTextBox1.Text + s + "\n";
                }
            }
            return true;
        }
    }
}