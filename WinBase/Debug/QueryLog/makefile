# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (c) Microsoft Corporation.  All Rights Reserved.
#
#
# Processor independent makefile

!IF "$(TARGETOS)" != "WINNT" || ("$(TARGETOS)" == "WINNT" && "$(APPVER)" < "6.0")
!MESSAGE  Sorry, QueryLog is supported only on Windows Vista of higher!
!ELSE

# Nmake macros for building makefile
!include <win32.mak>

PROJ = QueryLog

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"
    
#--------------------------------------------------------------------#
    
OBJS=$(OUTDIR)\QueryLog.obj

.cpp{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) /EHsc /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**
    
$(OUTDIR)\$(PROJ).exe: $(OBJS)
	$(link) $(ldebug) $(guilibs) $(conflags) /MACHINE:$(CPU) -out:$(OUTDIR)\$(PROJ).exe $(OBJS) /PDB:$(OUTDIR)\$(PROJ).PDB wevtapi.lib

clean:
	$(CLEANUP)

!ENDIF
