#======================================================================
# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
# KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
# PURPOSE.
#
# Copyright(c) Microsoft Corporation. All Right Reserved.
#
# File: MAKEFILE
#
# NMAKE description file for the DataCollectorSer.cpp PLA sample
#
#======================================================================



!include <Win32.Mak>

all: $(OUTDIR) $(OUTDIR)\datacollectorset.exe

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

conlibs = $(conlibs) ole32.lib oleaut32.lib
 
$(OUTDIR)\datacollectorset.obj: datacollectorset.cpp
    $(cc) $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" datacollectorset.cpp

$(OUTDIR)\datacollectorset.exe: $(OUTDIR)\datacollectorset.obj
    $(link) $(conflags) -out:$(OUTDIR)\datacollectorset.exe $(OUTDIR)\datacollectorset.obj $(conlibs)

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
