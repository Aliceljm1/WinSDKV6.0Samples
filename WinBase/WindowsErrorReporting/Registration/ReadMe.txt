Registration API Sample
===========================

This sample demonstrates how to use the registration APIs provided for 
Application Restart, Application Recovery, files, and
memory blocks.

This console application will cause an access violation after:
 - registering for application recovery
 - registering for application restart
 - registering a file
 - registering a block of memory
 
The access violation will be picked up by Windows Error Reporting and will show the
error reporting user experience, including application recovery and application restart.

It should be run from a console window with no command line arguments.  

Please refer to the comments in the code for more explanation on what this sample does.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All rights reserved.
