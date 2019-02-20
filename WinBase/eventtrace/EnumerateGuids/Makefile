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
# NMAKE description file for the MsnFilter sample
#
#======================================================================
!IF "$(TARGETOS)" == "WINNT"

!include <win32.mak>

PROJ = enumguid

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"


#======================================================================
# Define project specific macros
#======================================================================
PROJ_OBJS  = $(OUTDIR)\$(PROJ).obj
EXTRA_LIBS = kernel32.lib  wbemuuid.lib shell32.lib user32.lib ole32.lib oleaut32.lib

#======================================================================
# Define UNICODE if you want this.  UNICODE does not work on Win9x
#======================================================================
# EXTRA_DEFS = -DUNICODE -D_UNICODE

#======================================================================
# Inference rule for updating the object files
#======================================================================
$(OUTDIR)\$(PROJ).obj : $(PROJ).cpp
   $(cc) $(cdebug) $(cflags) $(cvarsdll) $(EXTRA_DEFS) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $(PROJ).cpp


#======================================================================
# Build Rules for EXE
#======================================================================
$(OUTDIR)\$(PROJ).EXE : $(BASE_OBJS) $(PROJ_OBJS)
   $(link) $(ldebug) $(conlflags) $(PROJ_OBJS)\
   $(conlibs) $(EXTRA_LIBS) -out:$(OUTDIR)\$(PROJ).exe /PDB:$(OUTDIR)\$(PROJ).PDB

clean:
	$(CLEANUP)
!ELSE
!MESSAGE Sorry this sample only builds for the Windows NT Platform
!ENDIF
