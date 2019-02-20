# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (c) Microsoft Corporation.  All Rights Reserved.
#
#
# Processor independent makefile for Platform SDK
#

# Nmake macros for building Win32 apps
!include <win32.mak>



PROJ = subscription

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe

# Define project specific macros
PROJ_OBJS  = $(OUTDIR)\AddEventSource.obj \
             $(OUTDIR)\CreateSubscription.obj \
             $(OUTDIR)\EnumerateSubscriptions.obj \
             $(OUTDIR)\GetArrayProperty.obj \
             $(OUTDIR)\GetProperty.obj \
             $(OUTDIR)\GetRuntimeStatus.obj \
             $(OUTDIR)\GetSubscription.obj \
             $(OUTDIR)\Main.obj \
             $(OUTDIR)\RetrySubscription.obj \
             $(OUTDIR)\Utils.obj \

EXTRA_LIBS = wecapi.lib



#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

# Inference rule for updating the object files
.cpp{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) /EHsc /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $** /GS

# Build rule for EXE
$(OUTDIR)\$(PROJ).EXE: $(PROJ_OBJS) 
    $(link) $(linkdebug) $(conlflags) \
    $(BASE_OBJS) $(PROJ_OBJS) $(conlibs) $(EXTRA_LIBS)  /SAFESEH \
    -out:$(OUTDIR)\$(PROJ).exe 

# Rules for cleaning out those old files
clean:
        $(CLEANUP)
