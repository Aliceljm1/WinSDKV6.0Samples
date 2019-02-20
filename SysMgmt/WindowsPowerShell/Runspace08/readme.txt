Runspace Sample 08
==========================
     This sample uses the Runspace and Pipeline classes directly. It builds
     a pipeline that executes the get-process cmdlet piped into sort-object.
     The Command class is used to illustrate adding parameters to a command.

Sample Language Implementations
===============================
     This sample is available in the following language implementations:

     - C#

To build the sample using Visual Studio:
=======================================
     1. Open Windows Explorer and navigate to Runspace08 under the samples directory.
     2. Double-click the icon for the .sln (solution) file to open the file in Visual Studio.
     3. In the Build menu, select Build Solution.
     The library will be built in the default \bin or \bin\Debug directory.


To run the sample:
=================
     1. Start command prompt.
     2. Navigate to the folder containing the sample executable.
     3. Run the executable

Demonstrates
============
     This sample demonstrates the following:

     1. Using the RunspaceFactory class to create a runspace.
     2. Creating a Pipeline object
     3. Adding individual commands to that runspace
     4. Adding parameters to the commands before adding them to the pipeline
     5. Synchronously invoking the constructed pipeline.
     6. Working with PSObject to extract properties from the objects returned.

