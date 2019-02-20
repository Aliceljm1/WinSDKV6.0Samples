/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) Microsoft Corporation.  All rights reserved.

Module Name:

    program.cs
 
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
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace rmwinformapp
{

    /*++

    Class Description:

    This class provides pInvoke support to wrap RegisterApplicationRestart 
    Windows native code APIs into managed code calls.

    --*/

    public class Win32
    {
        [DllImport("Kernel32.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RegisterApplicationRestart([MarshalAs(UnmanagedType.LPWStr)] string CommandLine, uint dwFlags);
    }



    /*++

    Class Description:

    This helper class simply provides a way to expose the CommandLine arguments to
    be available globally to rest of the application

    --*/

    public class App
    {
        public static string CommandLine;
        public static int ArgumentCount = 0;
    }

    /*++

    Class Description:

    This class provides the primary entry point for this application.

    --*/

    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {

            if (args.Length == 1)
            {
                App.CommandLine = args[0].ToString();
                App.ArgumentCount = 1;
            }


            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}