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

// This sample introduces parameter sets, the input object and
// DefaultParameterSet.
namespace Microsoft.Samples.PowerShell.Commands
{
   #region StopProcCommand

    /// <summary>
   /// Class that implements the stop-proc cmdlet.
   /// </summary>
   [Cmdlet(VerbsLifecycle.Stop, "Proc",
       DefaultParameterSetName = "ProcessId",
       SupportsShouldProcess = true)]
   public class StopProcCommand : PSCmdlet
   {
       #region Parameters

       /// <summary>
       /// The list of process names on which this cmdlet will work.
       /// </summary>
       [Parameter(
          Position = 0,
          ParameterSetName = "ProcessName",
          Mandatory = true,
          ValueFromPipeline = true,
          ValueFromPipelineByPropertyName = true,
          HelpMessage = "The name of one or more processes to stop. Wildcards are permitted."
       )]
       [Alias("ProcessName")]
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
       [Parameter(
          HelpMessage = "If set the process(es) will be passed to the pipeline after stopped."
       )]
       public SwitchParameter PassThru
       {
           get { return passThru; }
           set { passThru = value; }
       }
       private bool passThru;

       /// <summary>
       /// The list of process IDs on which this cmdlet will work.
       /// </summary>
       [Parameter(
          ParameterSetName = "ProcessId",
          Mandatory = true,
          ValueFromPipelineByPropertyName = true,
          ValueFromPipeline = true
       )]
       [Alias("ProcessId")]
       public int[] Id
       {
           get { return processIds; }
           set { processIds = value; }
       }
       private int[] processIds;

       /// <summary>
       /// An array of Process objects from the stream to stop.
       /// </summary>
       /// <value>Process objects</value>
       [Parameter(
           ParameterSetName = "InputObject",
           Mandatory = true,
           ValueFromPipeline = true)]
       public Process[] InputObject
       {
           get { return inputObject; }
           set { inputObject = value; }
       }
       private Process[] inputObject;

       #endregion Parameters

       #region CmdletOverrides
       /// <summary>
       /// For each of the requested processnames:
       /// 1) check it's not a special process
       /// 2) attempt to stop that process.
       /// If no process requested, then nothing occurs.
       /// </summary>
       protected override void ProcessRecord()
       {
           switch (ParameterSetName)
           {
               case "ProcessName":
                   ProcessByName();
               break;

               case "ProcessId":
                   ProcessById();
                   break;

               case "InputObject":
                   foreach (Process process in inputObject)
                   {
                       SafeStopProcess(process);
                   }
                   break;

               default:
                   throw new ArgumentException("Bad ParameterSet Name");
           } // switch (ParameterSetName...
       } // ProcessRecord

       #endregion Cmdlet Overrides

       #region Helper Methods

       /// <summary>
       /// Returns all processes with matching names.
       /// </summary>
       /// <param name="processName">
       /// The name of the process(es) to return
       /// </param>
       /// <param name="allProcesses">An array of all 
       /// machine processes.</param>
       /// <returns>An array of matching processes.</returns>
       internal ArrayList SafeGetProcessesByName(string processName, 
                                ref ArrayList allProcesses)
       {
           // Create and array to store the matching processes.
           ArrayList matchingProcesses = new ArrayList();

           // Create the wildcard for pattern matching.
           WildcardOptions options = WildcardOptions.IgnoreCase |
                                     WildcardOptions.Compiled;
           WildcardPattern wildcard = new WildcardPattern(processName, options);           

           // Walk all of the machine processes.
          foreach(Process process in allProcesses)
          {
               string processNameToMatch = null;
               try
               {
                   processNameToMatch = process.ProcessName;
               }
               catch (Win32Exception e)
               {
                   // Remove the process from the list so that it is not 
                   // checked again.
                   allProcesses.Remove(process);

                   string message =
                         String.Format(CultureInfo.CurrentCulture, "The process \"{0}\" could not be found",
                                             processName);
                   WriteVerbose(message);
                   WriteError(new ErrorRecord(e, "ProcessNotFound",
                                    ErrorCategory.ObjectNotFound, processName));

                   continue;
               }

               if (!wildcard.IsMatch(processNameToMatch))
               {
                   continue;
               }

               matchingProcesses.Add(process); 
           } // foreach(Process...

           return matchingProcesses;
       } // SafeGetProcessesByName

       /// <summary>
       /// Safely stops a named process.  Used as standalone function
       /// to declutter ProcessRecord method.
       /// </summary>
       /// <param name="process">The process to stop.</param>
       private void SafeStopProcess(Process process)
       {
           string processName = null;
           
           try
           {
               processName = process.ProcessName;
           }
           catch (Win32Exception e)
           {
               WriteError(new ErrorRecord(e, "ProcessNotFound",
                                ErrorCategory.OpenError, processName));

               return;
           }          

           // Confirm the operation first.
           // This is always false if WhatIf is set.
           if (!ShouldProcess(string.Format(CultureInfo.CurrentCulture, 
                    "{0} ({1})", processName, process.Id)))
           {
               return;
           }

           // Make sure the user really wants to stop a critical
           // process and possibly stop the machine.
           bool criticalProcess = criticalProcessNames.Contains(processName.ToLower(CultureInfo.CurrentCulture));

           string message = null;
           if (criticalProcess && !force)
           {
               message = String.Format(CultureInfo.CurrentCulture, 
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
                   return;
               }
           } // if (criticalProcess...

           // Display a warning information if stopping a critical 
           // process
           if (criticalProcess)
           {
               message =
                 String.Format(CultureInfo.CurrentCulture,
                                "Stopping the critical process \"{0}\".",
                                    processName);
               WriteWarning(message);
           } // if (criticalProcess...

           try
           {
               // Stop the process.
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
                                    ErrorCategory.CloseError,
                                    process)
                              );

                   return;
               } // if ((e is...
               else throw;
           } // catch 

           // Write a user-level message to the pipeline. These are 
           // intended to give the user detailed information on the 
           // operations performed by the Cmdlet. These messages will
           // appear with the -Verbose option.
           message = String.Format(CultureInfo.CurrentCulture, 
                                        "Stopped process \"{0}\", pid {1}.",
                                            processName, process.Id);

           WriteVerbose(message);

           // If the -PassThru command line argument is
           // specified, pass the terminated process on.
           if (passThru)
           {
               // Write a debug message to the host which will be helpful
               // in troubleshooting a problem. All debug messages
               // will appear with the -Debug option
               message =
                   String.Format(CultureInfo.CurrentCulture, 
                                    "Writing process \"{0}\" to pipeline",
                                        processName);
               WriteDebug(message);
               WriteObject(process);
           } // if (passThru..
       } // SafeStopProcess


       /// <summary>
       /// Stop processes based on their names (using the
       /// ParameterSetName as ProcessName)
       /// </summary>
       private void ProcessByName()
       {
           ArrayList allProcesses = null;

           // Get a list of all processes.
           try
           {
               allProcesses = new ArrayList(Process.GetProcesses());
           }
           catch (InvalidOperationException ioe)
           {
               base.ThrowTerminatingError(new ErrorRecord(
                    ioe, "UnableToAccessProcessList",
                    ErrorCategory.InvalidOperation, null));
           }

           // If a name parameter is passed to cmdlet, get 
           // the associated process(es). 
           // Write a non-terminating error for failure to
           // retrieve a process
           foreach (string name in processNames)
           {
               // The allProcesses array list is passed as a reference because 
               // any process whose name cannot be obtained will be removed
               // from the list so that its not compared the next time.
               ArrayList processes =
                   SafeGetProcessesByName(name, ref allProcesses);

               // If no processes were found write a non-
               // terminating error.
               if (processes.Count == 0)
               {
                   WriteError(new ErrorRecord(
                       new Exception("Process not found."),
                       "ProcessNotFound",
                       ErrorCategory.ObjectNotFound,
                       name));
               } // if (processes...
               // Otherwise terminate all processes in the list.
               else
               {
                   foreach (Process process in processes)
                   {
                       SafeStopProcess(process);
                   } // foreach (Process...
               } // else
           } // foreach (string...
       } // ProcessByName

       /// <summary>
       /// Stop processes based on their ids (using the
       /// ParameterSetName as ProcessIds)
       /// </summary>
       internal void ProcessById()
       {
           foreach (int processId in processIds)
           {
               Process process = null;
               try
               {
                   process = Process.GetProcessById(processId);

                   // Write a debug message to the host which will be helpful
                   // in troubleshooting a problem. All debug messages
                   // will appear with the -Debug option
                   string message =
                       String.Format(CultureInfo.CurrentCulture, 
                                        "Acquired process for pid : {0}",
                                            process.Id);
                   WriteDebug(message);
               }
               catch (ArgumentException ae)
               {
                   string
                       message = String.Format(CultureInfo.CurrentCulture, 
                                            "The process id {0} could not be found",
                                                processId);
                   WriteVerbose(message);
                   WriteError(new ErrorRecord(ae, "ProcessIdNotFound",
                                    ErrorCategory.ObjectNotFound, processId));
                   continue;
               }

               SafeStopProcess(process);
           } // foreach (int...
       } // ProcessById

       #endregion Helper Methods

       #region Private Data

       private bool yesToAll, noToAll;

       /// <summary>
       /// Partial list of critical processes that should not be 
       /// stopped.  Lower case is used for case insensitive matching.
       /// </summary>
       private ArrayList criticalProcessNames = new ArrayList(
          new string[] { "system", "winlogon", "spoolsv", "calc" }
       );

       #endregion Private Data

   } // StopProcCommand

   #endregion StopProcCommand

   #region PowerShell snap-in

   /// <summary>
   /// Create this sample as an PowerShell snap-in
   /// </summary>
   [RunInstaller(true)]
   public class StopProcPSSnapIn04 : PSSnapIn
   {
       /// <summary>
       /// Create an instance of the StopProcPSSnapIn04
       /// </summary>
       public StopProcPSSnapIn04()
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
               return "StopProcPSSnapIn04";
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
               return "StopProcPSSnapIn04,Microsoft";
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

