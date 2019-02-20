Network Access protection Sample
================================
     Demonstrates Nap System health agent, Nap System Health Validator, and Nap Enforcement Client


Sample Language Implementations
===============================
     This sample is available in the following language implementations:
     C++


To build the sample using the command prompt:
=============================================
     1. Open the Command Prompt window and navigate to the  directory.
     2. Type msbuild [Solution Filename].


To build the sample using Visual Studio 2005 (preferred method):
================================================================
     1. Open Windows Explorer and navigate to the  directory.
     2. Double-click the icon for the .sln (solution) file to open the file in Visual Studio.
     3. In the Build menu, select Build Solution.
     The application will be built in the default \Debug or \Release directory.


To run the sample:
=================
     1. Navigate to the directory that contains the new executable, using the command prompt or Windows Explorer.
     2. Type [ExecutableFile] at the command line, or double-click the icon for [SampleExecutable] to launch it from Windows Explorer.


Additional Run Steps:
=====================
     1. regsvr32 /s sdkshv.dll on the NAP server.
     2. Run SdkSha.exe /? on the NAP client for further instructions.
     3. Run SdkQec.exe /? on the NAP client for further instructions.
