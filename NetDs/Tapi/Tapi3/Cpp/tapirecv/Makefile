# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1997-2003  Microsoft Corporation.  All Rights Reserved.
#
#


# Nmake macros for building Windows 32-Bit apps
!include <win32.mak>
!if ("$(TARGETOS)" == "WINNT") && ("$(APPVER)" != "4.0") \
    && ("$(CPU)" != "IA64") && ("$(CPU)" != "AMD64")

PROJ = TAPIRecv

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe 

$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

# Define project specific macros
PROJ_OBJS  = $(OUTDIR)\TAPIRecv.obj $(OUTDIR)\AVIFileWriter.obj $(OUTDIR)\Processing.obj $(OUTDIR)\WorkerThread.Obj
BASE_OBJS  =

!if ("$(DXSDK_DIR)" == "")
!message !!!See Readme.Txt about setting the DXSDK_DIR enviroment var!!!
!endif

INCLUDE  = $(DXSDK_DIR)\Include;$(MSSDK)\samples\Multimedia\DirectShow\baseclasses;$(INCLUDE)


DXDBGOUTDIR = Debug
DXRELOUTDIR = Release

!IFDEF NODEBUG
STRMBASE="$(MSSDK)\samples\Multimedia\DirectShow\baseclasses\$(DXRELOUTDIR)\strmbase.lib" /NODEFAULTLIB:libcmt.lib
!else
STRMBASE="$(MSSDK)\samples\Multimedia\DirectShow\baseclasses\$(DXDBGOUTDIR)\strmbasd.lib" /NODEFAULTLIB:libcmtd.lib
!endif

EXTRA_LIBS = $(STRMBASE) winMM.lib vfw32.lib user32.lib uuid.lib

# Inference rule for updating the object files
.cpp{$(OUTDIR)}.obj:
  $(cc) $(cdebug) $(cflags) -EHsc $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**


# Build rule for EXE
$(OUTDIR)\$(PROJ).EXE: $(BASE_OBJS) $(PROJ_OBJS)
    $(link) $(ldebug) $(conlflags) \
    $(BASE_OBJS) $(PROJ_OBJS) \
    $(EXTRA_LIBS) -out:$(OUTDIR)\$(PROJ).exe $(MAPFILE) $(olelibsdll)
    if exist $(OUTDIR)\$(PROJ).EXE.manifest mt.exe -manifest $(OUTDIR)\$(PROJ).EXE.manifest -outputresource:$(OUTDIR)\$(PROJ).EXE;1

# Rules for cleaning out old files
clean:
        $(CLEANUP)
!else
!if ("$(CPU)" == "IA64") || ("$(CPU)" == "AMD64")
!message Sorry, this sample is not supported on the 64-bit platforms.
!else
!message Sorry this sample is only supported on the Windows 2000 and above (32-bit).
!endif
!endif