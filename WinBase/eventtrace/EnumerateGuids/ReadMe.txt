========================================================================
    CONSOLE APPLICATION : EnumGuid Project Overview
========================================================================

enumguid.vcproj
    This is a Visual Studio file which contains information about the version of Visual Studio 
    that generated the file, and information about the platforms, configurations, and project features.

enumguid.cpp
    This is the application's main source file.

enumguid.ncb
    This is a cache file used by Visual Studio.

enumguid.sln
    This is the solution file for enumguid sample generated using Visual Studio. This is the
    solution file which should be loaded into Visual Studio to generate the executable for this sample.

/////////////////////////////////////////////////////////////////////////////

Build process
==============
There are two ways to build this process: (1) Use Visual Studio, or (2) use makefile


Visual Studio Build Process
------------------------------
Open the solution file in Visual Studio. The project properties need to be modified to reflect your local
environment. The include path needs to point to the Visual Studio include and Microsoft SDK include paths. The
library path will point to the Microsoft SDK library path. There are two ways to do this. The first is to directly 
modify the include paths and library paths in enumguid.vcproj. The second is to do so from Visual Studio. Go to the 
Project Properties dialog, and expand the C/C++ tab. Modify the "Additional Include Directories" to point to the 
correct Visual Studio and Microsoft SDK include paths. Next, expand the Linker tab, and modify the 
"Additional Library Directories" to point to the correct Microsoft SDK library path. 
Also make sure to update the Configuration Manager to include the correct platform.
Once this is done, then build the solution.


SDK Makefile Build Process
----------------------------
Use the SDK build environment window and type "nmake all" in your sample code directory.



Sample Program Usage
======================
This sample demonstrates the EnumerateTraceGuids() and EnumerateTraceGuidsEx() functions which are part of the
ETW tracing facility. Both of these functions return information about currently registered providers, including
enablment information. In this sample there are two switches: -enumguid demonstrates the use of EnumerateTraceGuids(),
while -enumguidex demonstrates the use of EnumerateTraceGuidsEx().


EnumerateTraceGuids switch
===================================
Command: enumguid.exe -enumguid

This demonstrates the use of the EnumerateTraceGuids() function which is declared in evntrace.h. This returns the GUIDs of all
of the registered trace providers. For each GUID, it returns provider enablement information. More specifically, it
returns whether the provider is enabled (True or False), the LoggerId to which events are being logged by that provider (non-zero
value for enabled provider), the Level at which the provider is enabled, and the flags. For a provider using the Windows Vista ETW
APIs, the flag value is a bitwise-OR between the MatchAnyKeyword and MatchAllKeyword masks.



EnumerateTraceGuidsEx switch
===================================
Command: enumguid.exe -enumguidex
	 enumguid.exe -enumguidex #<guid>


This demonstrates the use of the EnumerateTraceGuidsEx() function, which is d in evntrace.h, and extends the functionality
of EnumerateTraceGuids(). When this switch is called without a provider GUID, then the behavior is very similar to 
EnumerateTraceGuids(), except that only the GUIDs of the registered providers are returned; no information regarding provider 
enablement is returned. However, detailed provider enablement information is returned when this sample is run with the
-enumguidex #<guid> switch, where <guid> corresponds to a provider GUID. The information returned includes the provider enablement
information such as MatchAnyKeyword and MatchAllKeyword masks (this is for Vista providers only; for providers using the 
Windows 2000 and above ETW APIs, the MatchAnyKeyword mask will contain the Flags, while the MatchAllKeyword mask will be zero), the
level, any extended property collection (only for Vista providers), and the session to which the provider is logging. This information
is shown for each instance of the registered provider. 
