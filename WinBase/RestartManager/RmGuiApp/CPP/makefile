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

all: rmguiapp.exe


# Update the object file if necessary

rmguiapp.obj: rmguiapp.cpp
    $(cc) $(cdebug) $(cflags) $(cvars) rmguiapp.cpp

# Update the executable file if necessary, and if so, add the resource back in.

rmguiapp.exe: rmguiapp.obj
    $(link) $(ldebug) $(guilflags) -out:rmguiapp.exe rmguiapp.obj $(guilibs) advapi32.lib kernel32.lib
