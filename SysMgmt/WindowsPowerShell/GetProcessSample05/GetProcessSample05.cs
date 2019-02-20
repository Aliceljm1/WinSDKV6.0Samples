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
using Win32Exception = System.ComponentModel.Win32Exception;
using System.Diagnostics;
using System.Security.Permissions;
//Windows PowerShell namespace
using System.Management.Automation;
using System.ComponentModel;

// This sample is a complete implementation of the get-proc Cmdlet.
namespace Microsoft.Samples.PowerShell.Commands
{
    #region GetProcCommand

    /// <summary>
   /// This class implements the get-proc cmdlet
   /// </summary>
   [Cmdlet(VerbsCommon.Get, "Proc", 
      DefaultParameterSetName="ProcessName")]
   public class GetProcCommand: PSCmdlet
   {
      #region Parameters

      /// <summary>
      /// The list of process names on which this cmdlet will work
      /// </summary>
      [Parameter(
         Position = 0,
         ParameterSetName = "ProcessName",
         ValueFromPipeline = true,
         ValueFromPipelineByPropertyName = true)]
      [ValidateNotNullOrEmpty]
      public string[] Name
      {
         get { return processNames; }
         set { processNames = value; }
      }

      /// <summary>
      /// gets/sets an array of process IDs
      /// </summary>
      [Parameter(
         ParameterSetName = "Id",
         Mandatory = true,
         ValueFromPipeline = true,
         ValueFromPipelineByPropertyName = true,
         HelpMessage = "The unique id of the process to get.")]
      public int[] Id
      {
         get { return processIds; }
         set { processIds = value; }
      }

      /// <summary>
      /// If the input is a stream of [collections of] Process 
      /// objects, we bypass the ProcessName and Id parameters and 
      /// read the Process objects directly.  This allows us to deal 
      /// with processes which have wildcard characters in their name.
      /// <value>Process objects</value>
      /// </summary>
      [Parameter(
         ParameterSetName = "InputObject",
         Mandatory = true,
         ValueFromPipeline = true)]
      public Process[] Input
      {
         get { return inputObjects; }
         set { inputObjects = value; }
      }

      #endregion Parameters

      #region Cmdlet Overrides
      /// <summary>
      /// For each of the requested processnames, retrieve and write
      /// the associated processes.
      /// </summary>
      protected override void ProcessRecord()
      {
         List<Process> matchingProcesses;
         
         WriteDebug("Obtaining list of matching process objects.");

         switch (ParameterSetName)
         {
            case "Id":
               matchingProcesses = GetMatchingProcessesById();
               break;
            case "ProcessName":
               matchingProcesses = GetMatchingProcessesByName();
               break;
            case "InputObject":
               matchingProcesses = GetProcessesByInput();
               break;
            default:
               ThrowTerminatingError(
                   new ErrorRecord(
                       new ArgumentException("Bad ParameterSetName"),
                       "UnableToAccessProcessList",
                       ErrorCategory.InvalidOperation,
                       null));
               return;
         } // switch (ParameterSetName)

         WriteDebug("Outputting matching process objects.");

         matchingProcesses.Sort(ProcessComparison);

         foreach (Process process in matchingProcesses)
         {
            WriteObject(process);
         }
      } // ProcessRecord

      #endregion Overrides

      #region protected Methods and Data
      /// <summary>
      /// Retrieves the list of all processes matching the ProcessName
      /// parameter.
      /// Generates a non-terminating error for each specified
      /// process name which is not found even though it contains
      /// no wildcards.
      /// </summary>
      /// <returns></returns>
      [EnvironmentPermissionAttribute(
         SecurityAction.LinkDemand, 
         Unrestricted = true)]
      private List<Process> GetMatchingProcessesByName()
      {
         new EnvironmentPermission(
            PermissionState.Unrestricted).Assert();
         
         List<Process> allProcesses = 
            new List<Process>(Process.GetProcesses());

         // The keys dictionary will be used for rapid lookup of 
         // processes already in the matchingProcesses list.
         Dictionary<int, byte> keys = new Dictionary<int, byte>();

         List<Process> matchingProcesses = new List<Process>();

         if (null == processNames)
            matchingProcesses.AddRange(allProcesses);
         else
         {
            foreach (string pattern in processNames)
            {
               WriteVerbose("Finding matches for process name \""
                  + pattern + "\".");

               // WildCard serach on the available processes
               WildcardPattern wildcard =
                  new WildcardPattern(pattern, 
                     WildcardOptions.IgnoreCase);

               bool found = false;

               foreach (Process process in allProcesses)
               {
                  if (!keys.ContainsKey(process.Id))
                  {
                     string processName = SafeGetProcessName(process);

                     // Remove the process from the allProcesses list 
                     // so that it's not tested again.
                     if(processName.Length == 0)
                        allProcesses.Remove(process);

                     // Perform a wildcard search on this particular 
                     // process and check whehter this matches the 
                     // pattern specified.
                     if (!wildcard.IsMatch(processName))
                        continue;

                     WriteDebug("Found matching process id " 
                        + process.Id + ".");

                     // We have found a match.
                     found = true;
                     // Store the process ID so that we don't add the 
                     // same one twice.
                     keys.Add(process.Id, 0);
                     // Add the process to the processes list.
                     matchingProcesses.Add(process);
                  }
               } // foreach (Process...

               if (!found &&
                 !WildcardPattern.ContainsWildcardCharacters(pattern))
               {
                  WriteError(new ErrorRecord(
                     new ArgumentException("Cannot find process name "
                        + "\"" + pattern + "\"."),
                     "ProcessNameNotFound",
                     ErrorCategory.ObjectNotFound,
                     pattern));
               }
            } // foreach (string...
         } // if (null...
         
         return matchingProcesses;
      } // GetMatchingProcessesByName

      /// <summary>
      /// Returns the name of a process.  If an error occurs, a blank
      /// string will be returned.
      /// </summary>
      /// <param name="process">The process whose name will be 
      /// returned.</param>
      /// <returns>The name of the process.</returns>
      [EnvironmentPermissionAttribute(
         SecurityAction.LinkDemand, Unrestricted = true)]
      protected static string SafeGetProcessName(Process process)
      {
         new EnvironmentPermission(PermissionState.Unrestricted).Assert();
         string name = "";

         if (process != null)
         {
            try 
            { 
                return process.ProcessName; 
            }
            catch (Win32Exception) { }
            catch (InvalidOperationException) { }
         }

         return name;
     } // SafeGetProcessName

      #endregion Cmdlet Overrides

      #region Private Methods

     /// <summary>
      /// Function to sort by ProcessName first, then by Id
      /// </summary>
      /// <param name="x">first Process object</param>
      /// <param name="y">second Process object</param>
      /// <returns>
      /// returns less than zero if x less than y,
      /// greater than 0 if x greater than y, 0 if x == y
      /// </returns>
      private static int ProcessComparison(Process x, Process y)
      {
         int diff = String.Compare(
            SafeGetProcessName(x),
            SafeGetProcessName(y),
            StringComparison.CurrentCultureIgnoreCase);

         if (0 != diff)
            return diff;

         return x.Id - y.Id;
      }

      /// <summary>
      /// Retrieves the list of all processes matching the Id
      /// parameter.
      /// Generates a non-terminating error for each specified
      /// process ID which is not found.
      /// </summary>
      /// <returns>An array of processes that match the given id.
      /// </returns>
      [EnvironmentPermissionAttribute(
         SecurityAction.LinkDemand,
         Unrestricted = true)]
      private List<Process> GetMatchingProcessesById()
      {
         new EnvironmentPermission(
            PermissionState.Unrestricted).Assert();

         List<Process> matchingProcesses = new List<Process>();

         if (null != processIds)
         {
            // The keys dictionary will be used for rapid lookup of 
            // processes already in the matchingProcesses list.
            Dictionary<int, byte> keys = new Dictionary<int, byte>();

            foreach (int processId in processIds)
            {
               WriteVerbose("Finding match for process id "
                  + processId + ".");

               if (!keys.ContainsKey(processId))
               {
                  Process process;
                  try { process = Process.GetProcessById(processId); }
                  catch (ArgumentException ex)
                  {
                     WriteError(new ErrorRecord(
                        ex,
                        "ProcessIdNotFound",
                        ErrorCategory.ObjectNotFound,
                        processId));
                     continue;
                  }
                  
                  WriteDebug("Found matching process.");
                  
                  matchingProcesses.Add(process);
                  keys.Add(processId, 0);
               }
            }
         }
         
         return matchingProcesses;
      } // GetMatchingProcessesById

      /// <summary>
      /// Retrieves the list of all processes matching the Input
      /// parameter.
      /// </summary>
      [EnvironmentPermissionAttribute(
         SecurityAction.LinkDemand,
         Unrestricted = true)]
      private List<Process> GetProcessesByInput()
      {
         new EnvironmentPermission(
            PermissionState.Unrestricted).Assert();

         List<Process> matchingProcesses = new List<Process>();

         if (null != Input)
         {
            // The keys dictionary will be used for rapid lookup of 
            // processes already in the matchingProcesses list.
            Dictionary<int, byte> keys = new Dictionary<int, byte>();

            foreach (Process process in Input)
            {
               WriteVerbose("Refreshing process object.");

               if (!keys.ContainsKey(process.Id))
               {
                  try { process.Refresh(); }
                  catch (Win32Exception) { }
                  catch (InvalidOperationException) { }

                  matchingProcesses.Add(process);
               }
            }
         }
         return matchingProcesses;
      } // GetProcessesByInput

      #endregion Private Methods

      #region Private Data

      private string[] processNames;
      private int[] processIds;
      private Process[] inputObjects;

      #endregion Private Data

    } //GetProcCommand

    #endregion GetProcCommand

    #region PowerShell snap-in

    /// <summary>
    /// Create this sample as an PowerShell snap-in
    /// </summary>
    [RunInstaller(true)]
    public class GetProcPSSnapIn05 : PSSnapIn
    {
        /// <summary>
        /// Create an instance of the GetProcPSSnapIn05
        /// </summary>
        public GetProcPSSnapIn05()
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
                return "GetProcPSSnapIn05";
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
                return "GetProcPSSnapIn05,Microsoft";
            }
        }

        /// <summary>
        /// Description of this PowerShell snap-in.
        /// </summary>
        public override string Description
        {
            get
            {
                return "This is a PowerShell snap-in that includes the get-proc sample.";
            }
        }
    }

    #endregion PowerShell snap-in

}
//</snippet01>

