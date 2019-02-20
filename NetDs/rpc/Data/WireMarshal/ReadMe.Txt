WMarsh


The WMarsh program demonstrates the type-conversion attribute transmit_as.

FILES
=====

The directory samples\rpc\data\WMarsh contains the following files for
building the sample distributed application WMarsh:

File          Description

README.TXT    Readme file for the WMarsh sample
WMarsh.IDL      Interface definition language file
WMarsh.ACF      Attribute configuration file
WMarshC.C       Client main program
WMarshS.C       Server main program
WMarshP.C       Remote procedures
WMarshU.C       Utility functions for client, server
WMarshU.H       Utility function prototypes
MAKEFILE      Nmake file to build for Windows XP and above
MAKEFILE.DOS  Nmake file to build for MS-DOS

-------------------------------------------
BUILDING CLIENT AND SERVER APPLICATIONS FOR
MICROSOFT WINDOWS XP and above
-------------------------------------------

The following environment variables should be set for you already.

  set CPU=i386
  set INCLUDE=%SDKROOT%\h
  set LIB=%SDKROOT%\lib
  set PATH=%SDKROOT%\system32;%SDKROOT%\bin;

Where %SDKROOT% is the root directory for the 32-bit Windows SDK.


Build the sample distributed application:
  nmake cleanall
  nmake

This builds the executable programs WMarshc.exe
(client) and WMarshs.exe (server).


------------------------------------------
RUNNING THE CLIENT AND SERVER APPLICATIONS
------------------------------------------

On the server, enter

  WMarshs

On the client, enter

  net start workstation
  WMarshc

Note:  The client and server applications can run on
the same Microsoft Windows NT computer when you use
different screen groups.

Several command line switches are available to change
settings for this program. For a listing of the switches
available from the client program, enter

  WMarshc -?

For a listing of switches available from the server
program, enter

  WMarshs -?
