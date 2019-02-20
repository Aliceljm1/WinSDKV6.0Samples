/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) Microsoft Corporation.  All rights reserved.

Module Name:

    assemblyinfo.cs
 
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
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("rmwinformapp")]
[assembly: AssemblyDescription("")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("")]
[assembly: AssemblyProduct("rmwinformapp")]
[assembly: AssemblyCopyright("Copyright ©  2005")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible 
// to COM components.  If you need to access a type in this assembly from 
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("702550b6-de35-419e-960b-b3c83ae5ed6f")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]
