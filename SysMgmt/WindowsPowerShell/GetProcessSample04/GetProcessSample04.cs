//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//
using System;
using System.Diagnostics;
using System.Management.Automation;             //Windows PowerShell namespace
using System.ComponentModel;

// This sample introduces error handling.
namespace Microsoft.Samples.PowerShell.Commands
{
   #region GetProcCommand

    /// <summary>
   /// Class that implements the get-proc cmdlet
   /// </summary>
   [Cmdlet(VerbsCommon.Get, "Proc")]
   public class GetProcCommand: Cmdlet
   {
      #region Parameters

      /// <summary>
      /// The list of process names on which this cmdlet will work
      /// </summary>
      [Parameter(
         Position = 0,
         ValueFromPipeline = true,
         ValueFromPipelineByPropertyName = true
      )]
      [ValidateNotNullOrEmpty]
      public string[] Name
      {
         get { return processNames; }
         set { processNames = value; }
      }
      private string[] processNames;

      #endregion Parameters

      #region Cmdlet Overrides

      /// <summary>
      /// For each of the requested process names, retrieve and write
      /// the associated processes.
      /// </summary>
      protected override void ProcessRecord()
      {        
         // If no name parameter passed to cmdlet, get all processes.
          if (processNames == null)
          {
              WriteObject(Process.GetProcesses(), true);
          }
          else
          {
              // If a name parameter is passed to cmdlet, get and write 
              // the associated processes.
              // Write a non-terminating error for failure to retrieve
              // a process.
              foreach (string name in processNames)
              {
                  Process[] processes;

                  try
                  {
                      processes = Process.GetProcessesByName(name);
                  }
                  catch (InvalidOperationException ex)
                  {
                      WriteError(new ErrorRecord(
                         ex,
                         "UnableToAccessProcessByName",
                         ErrorCategory.InvalidOperation,
                         name));
                      continue;
                  }

                  WriteObject(processes, true);
              } // foreach (...
          } // else
      } // ProcessRecord

      #endregion Overrides

    } //GetProcCommand

   #endregion GetProcCommand

    #region PowerShell snap-in

    /// <summary>
    /// Create this sample as an PowerShell snap-in
    /// </summary>
    [RunInstaller(true)]
    public class GetProcPSSnapIn04 : PSSnapIn
    {
        /// <summary>
        /// Create an instance of the GetProcPSSnapIn04
        /// </summary>
        public GetProcPSSnapIn04()
            : base()
        {
        }

        /// <summary>
        /// Get a name for this PowerShell snap-in. This name will be used in registering
        /// this PowerShell snap-in.
        /// </summary>
        public override string Name
        {
            get
            {
                return "GetProcPSSnapIn04";
            }
        }

        /// <summary>
        /// Vendor information for this PowerShell snap-in.
        /// </summary>
        public override string Vendor
        {
            get
            {
                return "Microsoft";
            }
        }

        /// <summary>
        /// Gets resource information for vendor. This is a string of format: 
        /// resourceBaseName,resourceName. 
        /// </summary>
        public override string VendorResource
        {
            get
            {
                return "GetProcPSSnapIn04,Microsoft";
            }
        }

        /// <summary>
        /// Description of this PowerShell snap-in.
        /// </summary>
        public override string Description
        {
            get
            {
                return "This is a PowerShell snap-in that includes the get-proc cmdlet.";
            }
        }
    }

    #endregion PowerShell snap-in

}

