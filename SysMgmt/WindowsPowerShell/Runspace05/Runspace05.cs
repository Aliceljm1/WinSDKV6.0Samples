//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Management.Automation;
using System.Management.Automation.Runspaces;
using System.IO;

namespace Microsoft.Samples.PowerShell.Runspaces
{
    class Runspace05
    {
        /// <summary>
        /// This sample uses a console file to create Runspace. It invokes
        /// a command from PowerShell snap-in present in the console file.
        /// </summary>
        /// <param name="args">Unused</param>
        /// <remarks>
        /// This sample assumes that user has the PowerShell snap-in "GetProcPSSnapIn01" 
        /// produced in sample GetProcessSample01 installed on the machine. 
        /// Console file Runspacesample05.psc1, which ships with the sample has,
        /// PSSnapIn "GetProcPSSnapIn01" listed in it. 
        /// Runspacesample05.psc1 is created using following steps:
        /// 1) Start powershell.exe.
        /// 2) Add GetProcPSSnapIn01 PowerShell snap-in to console
        ///    PS> Add-PSSnapin GetProcPSSnapIn01
        /// 3) Export the console to a file
        ///    PS> export-console Runspacesample05.psc1
        /// 
        /// 	This sample demonstrates the following:
	    ///         1. Creating an instance of RunspaceConfiguration using configuration from the console file
        ///         2. Using the configuration to create a runspace
        ///         3. Create a pipeline with the get-proc cmdlet available in the PowerShell snap-in
        ///         4. Using PSObject to extract and display properties from the objects
        ///            returned by this command
        /// </remarks>
        static void Main(string[] args)
        {
            //Create RunspaceConfiguration using Runspacesample05.psc1
            PSConsoleLoadException warnings;
            RunspaceConfiguration config;
            try
            {
                config = RunspaceConfiguration.Create("Runspacesample05.psc1", out warnings);
            }
            catch (FileNotFoundException)
            {
                System.Console.WriteLine("Runspacesample05.psc1 is not present in the current directory. Copy Runspacesample05.psc1 to the current directory and run the sample again.");
                return;
            }

            if (warnings != null)
            {
                //We don't expect any warning so write the warning and return
                System.Console.Write(warnings.Message);
                return;
            }
            
            // Create a runspace. 
            // (Note that no PSHost instance is supplied to the CreateRunspace
            // function so the default PSHost implementation is used. See the 
            // Hosting topics for more information on creating your own PSHost class.)

            Runspace myRunSpace = RunspaceFactory.CreateRunspace(config);
            myRunSpace.Open();

            // Create a pipeline with get-proc command. 
            Pipeline pipeLine = myRunSpace.CreatePipeline("GetProcPSSnapIn01\\get-proc");
            Collection<PSObject> results = pipeLine.Invoke();

            Console.WriteLine("Process              HandleCount");
            Console.WriteLine("--------------------------------");

            // Print out each result object...
            foreach (PSObject result in results)
            {
                Console.WriteLine("{0,-20} {1}",
                    result.Members["ProcessName"].Value,
                    result.Members["HandleCount"].Value);
            }

            // Finally close the runspace
            // up any resources.
            myRunSpace.Close();

            System.Console.WriteLine("Hit any key to exit...");
            System.Console.ReadKey();
        }
    }
}

