   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
   TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE

   Copyright (C) 2005. Microsoft Corporation. All rights reserved.

========================================================================
    CONSOLE APPLICATION : ETW Provider Sample Project Overview
========================================================================

etwphils.c
    This is the source file which implements the ETW provider. This sourece file contains all of the ETW Provider function calls
    in order to register the provider, write events, and unregister the provider.

etwphils.h
    Header file for the sample, containing variable and structure declarations.

etwphils.xml
    This is the event manifest which contains all the event metadata, and describes the event payload.

////////////////////////////////////////////////////////////////////////


Platform Support
================
    Windows Vista


Build Process
========================
    Use the nmake command in Microsoft Platform SDK build environment.



Sample Program Usage
========================
    This sample code demonstrates the use of the ETW Provider APIs. It will show how to register a provider, write events 
    to an ETW session, and unregister a provider. It will also show how to control the ActivityId.
    The code implements the classic "Dining Philosophers" problem. 

    In this problem, there are N philosophers sitting around a table with only N chopsticks, and each philosopher spends time in a 
    think-eat cycle. In order to eat, the philosopher must pick-up the chopstick on the left and the chopstick on the right. 
    If either is in use, the the philosopher must wait for the neighboring philosopher to finish eating. The problem that must be 
    avoided is deadlock: each philosopher picks-up the chopstick to their right, and no one can proceed further. In order to make 
    the problem more intricate, after a specific number of think-eat cycles, all the philosophers stop eating and have a discussion.  

    All of the actions of the dining philosophers will be logged to an ETW session, and then written out to an event trace file. 
    The following ETW APIs are used in the sample:
      
    EventRegister
    EventUnregister
    EventDataDescCreate
    EventWrite
    EventActivityIdControl



Running the Sample
=========================



    Once the sample has been correctly built in the Microsoft SDK build environment, then the following steps must be completed:
    
    1) The manifest, which defines the event metadata, must be installed. This uses the wevtutil tool to install provider
       information into the registry such as the provider name, and the names of the provider resource and message files.

	wevtutil im etwphils.xml

    2) Once the manifest has been installed, then the ETW session to which the etwphils event provider will log events to must be started.
       Since the manifest describing the etwphils provider was installed in the previous step, we can use the friendly name of the
       provider to enable it to an ETW session.

       logman start EtwPhilSession -p ETWPhils 0 0 -o etwphils.etl -ets

       The above command starts a session called "EtwPhilSession", and enables the provider "ETWPhils" to that ETW session.
       The two zeros following the provider name indicate the level and keywords at which the provider "ETWPhils" is enabled; since they
       are both zero, all events will be written to the session. The -o switch defines the trace file to write the events from 
       the ETW session.

    3) Now the simulation can begin. Run the etwphils.exe provider. The callback routine which was passed to ETW when the provider
       registered will be called. This will display the SourceId of the application which enabled the ETWPhils provider, whether 
       the proivder is enabled, the level at which it is enabled, and the keyword masks.

    4) Once the simulation has been stopped, the ETW session needs to be stopped. Note that this can also be done without stopping
       the execution of the ETWPhils provider. The following logman command will disable the "EtwPhilSession" ETW session:

       logman stop EtwPhilSession -ets

    5) Now that the events have been written to a trace file, they need to be decoded. Tracerpt is a command-line tool which will
       correctly parse the event trace file using the metadata specified in the event manifest. It can also generate reports in order
       to better analyze the events. The following command will generate a comma-separated file of the event information:

       tracerpt etwphils.etl -o etwphils.csv

       Note that an XML-formatted list of the logged events can also be created: tracerpt etwphils.etl -of XML -f etwphils.xml

       To generate a simple summary of the events logged:

       tracerpt etwphils.etl -summary

       This will create a file called summary.txt.


Command line options
====================

There are several command line options available. Run etwphils /? for more information.