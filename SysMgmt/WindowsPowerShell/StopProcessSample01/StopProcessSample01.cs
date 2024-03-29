//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
using System;
using System.Diagnostics;
using System.Collections;
using Win32Exception = System.ComponentModel.Win32Exception;
using System.Management.Automation;             //Windows PowerShell namespace
using System.ComponentModel;
using System.Globalization;

// This sample introduces PassThru, ShouldProcess and
// ShouldContinue
namespace Microsoft.Samples.PowerShell.Commands
{
   #region StopProcCommand

    /// <summary>
   /// Class that implements the stop-proc cmdlet.
   /// </summary>
   [Cmdlet(VerbsLifecycle.Stop, "Proc",
       SupportsShouldProcess = true)]
   public class StopProcCommand : Cmdlet
   {
       #region Parameters

       /// <summary>
       /// The list of process names on which this cmdlet will work.
       /// </summary>
       [Parameter(
          Position = 0,
          Mandatory = true,
          ValueFromPipeline = true,
          ValueFromPipelineByPropertyName = true
       )]
       public string[] Name
       {
           get { return processNames; }
           set { processNames = value; }
       }
       private string[] processNames;

       /// <summary>
       /// Overrides the ShouldContinue check to force stop operation.
       /// This option should always be used with caution.
       /// </summary>
       [Parameter]
       public SwitchParameter Force
       {
           get { return force; }
           set { force = value; }
       }
       private bool force;
      
       /// <summary>
       /// Common parameter to determine if the process should pass the
       /// object down the pipeline after the process has been stopped.
       /// </summary>
       [Parameter]
       public SwitchParameter PassThru
       {
           get { return passThru; }
           set { passThru = value; }
       }
       private bool passThru;

       #endregion Parameters

       #region Cmdlet Overrides

       /// <summary>
       /// For each of the requested processnames:
       /// 1) check it's not a critical process
       /// 2) attempt to stop that process.
       /// If no process requested, then nothing occurs.
       /// </summary>     
       protected override void ProcessRecord()
       {         
           foreach (string name in processNames)
           {
               // For every name parameter passed to cmdlet, get the associated 
               // process(es). 
               // Write a non-terminating error for failure to retrieve 
               // a process
               Process[] processes;

               try 
               { 
                   processes = Process.GetProcessesByName(name); 
               }
               catch (InvalidOperationException ioe)
               {
                   WriteError(new ErrorRecord(ioe,"UnableToAccessProcessByName",
                       ErrorCategory.InvalidOperation, name));
   
                   continue;
               }

               // Try to stop the process(es) that have been retrieved for a name            
               foreach (Process process in processes)
               {
                   string processName;

                   try 
                   { 
                       processName = process.ProcessName; 
                   }
                   catch (Win32Exception e)
                   { 
                      WriteError(new ErrorRecord(e, "ProcessNameNotFound", 
                                           ErrorCategory.ReadError, process));
                      continue;
                   }

                   // Confirm the operation first.
                   // This is always false if WhatIf is set.
                   if (!ShouldProcess(string.Format(CultureInfo.CurrentCulture,"{0} ({1})", processName,
                               process.Id)))
                   {
                       continue;
                   }

                   // Make sure the user really wants to stop a critical
                   // process and possibly stop the machine.
                   bool criticalProcess = 
                       criticalProcessNames.Contains(processName.ToLower(CultureInfo.CurrentCulture));

                   if (criticalProcess &&!force)
                   {
                       string message = String.Format
                           (CultureInfo.CurrentCulture, 
                                "The process \"{0}\" is a critical process and should not be stopped. Are you sure you wish to stop the process?",
                                    processName);

                       // It is possible that ProcessRecord is called multiple 
                       // when objects are recieved as inputs from a pipeline.
                       // So, to retain YesToAll and NoToAll input that the 
                       // user may enter across mutilple calls to this 
                       // function, they are stored as private members of the 
                       // Cmdlet.
                       if (!ShouldContinue(message, "Warning!",
                                               ref yesToAll, ref noToAll))
                       {
                           continue;
                       }
                   } // if (cricicalProcess...

                   // Stop the named process.
                   try 
                   { 
                       process.Kill(); 
                   }
                   catch (Exception e)
                   {
                       if ((e is Win32Exception) || (e is SystemException) ||
                          (e is InvalidOperationException))
                       {
                           // This process could not be stopped so write
                           // a non-terminating error.
                           WriteError(new ErrorRecord(e, "CouldNotStopProcess", 
                                           ErrorCategory.CloseError, process));
                           continue;
                       } // if ((e is...
                       else throw;
                   } // catch

                   // If the -PassThru command line argument is
                   // specified, pass the terminated process on.
                   if (passThru)
                   {
                       WriteObject(process);
                   }
               } // foreach (Process...
           } // foreach (string...
       } // ProcessRecord

       #endregion Cmdlet Overrides

       #region Private Data      
      
       private bool yesToAll, noToAll;

       /// <summary>
       /// Partial list of critical processes that should not be 
       /// stopped.  Lower case is used for case insensitive matching.
       /// </summary>
       private ArrayList criticalProcessNames = new ArrayList(
          new string[] { "system", "winlogon", "spoolsv" }
       );

       #endregion Private Data

   } // StopProcCommand

   #endregion StopProcCommand

   #region PowerShell snap-in

   /// <summary>
   /// Create this sample as an PowerShell snap-in
   /// </summary>
   [RunInstaller(true)]
   public class StopProcPSSnapIn01 : PSSnapIn
   {
       /// <summary>
       /// Create an instance of the StopProcPSSnapIn01
       /// </summary>
       public StopProcPSSnapIn01()
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
               return "StopProcPSSnapIn01";
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
               return "StopProcPSSnapIn01,Microsoft";
           }
       }

       /// <summary>
       /// Description of this PowerShell snap-in.
       /// </summary>
       public override string Description
       {
           get
           {
               return "This is a PowerShell snap-in that includes the stop-proc cmdlet.";
           }
       }
   }

   #endregion PowerShell snap-in

} 

