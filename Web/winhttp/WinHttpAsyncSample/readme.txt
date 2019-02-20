WinHttpAsyncSample
Copyright (c) Microsoft Corporation. All rights reserved. 

The WinHttp Async Sample is a simple application which, based on configuration, would 
concurrently "browse" multiple web sites and their embedded links. It demonstrates the following:

1) use the WinHttp API asynchronously
2) how to manage multiple concurrent downloads
3) how to abort downloads gracefully
4) how to apply timeout for the overall transaction
5) how to use automatic proxy discovery to detect proxy settings
6) how to handle proxy failover
7) how to retry upon certain failures

The configuration is control by a static array in SimpleBrowser.cpp file. 
The app can be aborted (gracefully) by Ctrl-C.


  To build this sample, initialize the sample build environment with "setenv.bat" and then compile by running nmake in this directory.  This sample only builds for an NT environment.
