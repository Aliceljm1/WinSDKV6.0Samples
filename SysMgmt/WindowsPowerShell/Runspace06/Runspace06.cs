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

namespace Microsoft.Samples.PowerShell.Runspaces
{
    class Runspace06
    {
        /// <summary>
        /// This sample introduces how to add a PSSnapin to RunspaceConfiguration and
        /// then use modified RunspaceConfiguration to create the Runspace.
        /// </summary>
        /// <param name="args">Unused</param>
        /// <remarks>
        /// This sample assumes that user has the PowerShell snap-in "GetProcPSSnapIn01" 
        /// produced in sample GetProcessSample01 installed on the machine. 
        /// 
        /// 	This sample demonstrates the following:
	    ///         1. Creating an instance of RunspaceConfiguration
        ///         2. Adding a PowerShell snap-in to this configuration
        ///         3. Using the configuration to create a runspace
        ///         4. Create a pipeline with the get-proc cmdlet available in the PowerShell snap-in
        ///         5. Using PSObject to extract and display properties from the objects
        ///            returned by this command
        /// </remarks>
        static void Main(string[] args)
        {
            //Create a default RunspaceConfiguration 
            RunspaceConfiguration config = RunspaceConfiguration.Create();
            
            //Add GetProcPSSnapIn01 to config.
            PSSnapInException warning;
            config.AddPSSnapIn("GetProcPSSnapIn01", out warning);
            if (warning != null)
            {
                //We don't expect any warning so write the warning and return
                System.Console.Write(warning.Message);
                return;
            }
            
            // Create a runspace. 
            // (Note that no PSHost instance is supplied in the constructor so the
            // default PSHost implementation is used. See the Hosting topics for
            // more information on creating your own PSHost class.)

            Runspace myRunSpace = RunspaceFactory.CreateRunspace(config);
            myRunSpace.Open();

            // Create a pipeline with get-proc command from PowerShell snap-in GetProcPSSnapIn01.
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

