The RMFILTERAPP sample is a generic Win32-based console Restart Manager
conductor application

This sample shows the minimal amount of code your Restart Manager
conduct application should use during a servicing transaction.  
	
This sample also showcases Restart Manager filter APIs which
show how to set shutdown/restart filter behavior during a Restart
Manager session.

This sample uses notepad as the target process to be shutdown and
restarted.  It will launch notepad before actually starting the
Restart Manager transaction.  The parts of the sample which start
notepad are not required for RM.

The files included in this sample are:

RMFILTERAPP.CPP - The .CPP source file

Build Instructions:

To build this sample application type nmake from the sample directory.

You must be in the SDK Vista build environment window.