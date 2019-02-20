========================================================================
    CONSOLE APPLICATION : TDHSample Project Overview
========================================================================

TDHSample.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

TDHSample.cpp
    This is the main application source file.

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named TDHSample.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////

Build process
==============
There are two ways to build this process: (1) Use Visual Studio, or (2) use makefile

Visual Studio Build Process
------------------------------
Open the solution file (TDHSample.sln) in Visual Studio. The project properties need to be modified to reflect
your local environment. The include path will point to the Visual Studio include and Microsoft SDK
include paths.  The library path will point to the Microsoft SDK library path. 
Go the Project Properties dialog, and expand the C/C++ tab. Modify the "Additional Include Directories" to point
to the correct Visual Studio and Microsoft SDK include paths. Next, expand the Linker tab, and modify the
"Additional Library Directories" to point to the correct Microsoft SDK library path. Be sure to set the correct
Target Machine setting in the Advanced tab under Linker. Also make sure to update the Configuration
Manager to include the correct platform. Then build a clean solution.

SDK Makefile Build Process
----------------------------
Use the SDK build environment window and type "nmake" in your sample code directory.

Sample Program Usage
======================
There are two parts in this sample program (see details in the following section): payload parsing and 
provider enumeration.
"TDHSample.exe foo.etl" will first print out the event and payload information in the event trace file.
Then it enumerate all providers registered in the machine.


Trace Data Helper (TDH) API Samples
===================================

This readme describes the sample implementations of using the TDH API's supplied with the SDK 
and a brief description of event tracing. 

For more details please refer the Platform SDK documentation on Trace Data Helper APIs.

OVERVIEW
========

The TDH APIs provide a programmatic way to abstract the event description languages and 
provide the payload information in a binary form for cracking of ETW events. Furthermore, 
TDH APIs also provide the event provider information in a binary form for programmatic browsing of 
ETW event providers.  This way, ETW consumer and event provider enumeration can be immune to changes 
to the way events are published in the future.

Description of TDH APIs
====================================

There are two parts of TDH API sample code included - Payload Parsing and Proivder Enumeration.

Payload Parsing
-----------------

The ETW consumption API (ProcessTrace) returns a pointer to the header and a pointer to the payload. 
The trace consumer calls the TDH API with the event to be cracked and TDH API returns the event payload information.
There are 4 APIs about payload parsing in TDH API set:

TdhGetEventInformation() - Event header and payload information retrieval
TdhGetPropertySize() and TdhGetProperty() - ETW Event Property Retrieval
TdhGetEventMapInformation() - ETW Event Map Retrieval

Provider Enumeration
--------------------

The TDH APIs return a list of registed event providers with their names and guids.
Furthermore, ETW events have several metadata fields other than event payload itself.  
TDH supports the enumeration of all the following metadata fields, including keyword, level, and channel.
ETW supports a finer control for event providers that events can be grouped into logical components, 
which are identified by user-defined values, and use these values to enable or disable their generation.  
These values include keywords and levels.  This API is used to retrieve the field information of a particular 
event provider identified a by GUID.  In addition, ETW event has other types metadata fields, 
including task, opcode, and channel.  This field information can also be retrieved by this API.

TdhEnumerateProviders - ETW Event Provider Enumeration
TdhEnumerateProviderFieldInformation - ETW Event Provider Field Enumeration
TdhQueryProviderFieldInformation - ETW Event Provider Field Retrieving

Description of TDH API samples
====================================

This sample code shows how to use payload parsing APIs and provider enumeation APIs.
It prints out event and payload properties information as well as all registered provider information.