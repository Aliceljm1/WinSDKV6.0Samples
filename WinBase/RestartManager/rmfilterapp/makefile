# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1993-1999.  Microsoft Corporation.  All rights reserved.
#
# Nmake macros for building Windows 32-Bit apps
#

!include <Win32.Mak>

all: rmfilterapp.exe


# Update the object file if necessary

rmfilterapp.obj: rmfilterapp.cpp
    $(cc) $(cdebug) $(cflags) $(cvars) rmfilterapp.cpp

# Update the executable file if necessary, and if so, add the resource back in.

rmfilterapp.exe: rmfilterapp.obj
    $(link) $(ldebug) $(cuilflags) -out:rmfilterapp.exe rmfilterapp.obj $(cuilibs) advapi32.lib kernel32.lib rstrtmgr.lib