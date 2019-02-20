//
// Copyright (c) 2006 Microsoft Corporation. All rights reserved.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//

using System.Management.Automation;
using System.Management.Automation.Provider;
using System.ComponentModel;


namespace Microsoft.Samples.PowerShell.Providers
{
   #region AccessDBProvider

    /// <summary>
   /// Simple provider.
   /// </summary>
   [CmdletProvider("AccessDB", ProviderCapabilities.None)]
   public class AccessDBProvider : CmdletProvider
   {

   }

   #endregion AccessDBProvider

   #region PowerShell snap-in

   /// <summary>
   /// Create this sample as an PowerShell snap-in
   /// </summary>
   [RunInstaller(true)]
   public class AccessDBProviderPSSnapIn01 : PSSnapIn
   {
       /// <summary>
       /// Create an instance of the AccessDBProviderPSSnapIn01
       /// </summary>
       public AccessDBProviderPSSnapIn01()
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
               return "AccessDBProviderPSSnapIn01";
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
               return "AccessDBProviderPSSnapIn01,Microsoft";
           }
       }

       /// <summary>
       /// Description of this PowerShell snap-in.
       /// </summary>
       public override string Description
       {
           get
           {
               return "This is a PowerShell snap-in that includes the MS-Access DB PowerShell Provider .";
           }
       }
   }

   #endregion PowerShell snap-in

}

