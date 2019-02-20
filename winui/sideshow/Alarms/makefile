#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>

PROJ = WindowsSideShowAlarms
all:    $(OUTDIR) $(OUTDIR)\$(PROJ).exe

$(OUTDIR):
     if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

EXTRA_LIBS = WindowsSideShowGuids.lib

cflags = $(cflags) /D "UNICODE"

LINK32_OBJS = $(OUTDIR)\AlarmClient.obj \
              $(OUTDIR)\AlarmContent.obj \
              $(OUTDIR)\Alarms.obj
        
$(OUTDIR)\precomp.pch: stdafx.cpp
     @$(cc) $(cflags) $(cdebug) $(cvars) /FD /EHsc /TP /D "WIN32" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /Yc /Fp"$(OUTDIR)\precomp.pch" $**
        

.cpp{$(OUTDIR)}.obj:
     @$(cc) $(cflags) $(cdebug) $(cvars) /FD /EHsc /TP /D "WIN32" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /Fp"$(OUTDIR)\precomp.pch" /Yustdafx.h $**


$(PROJ).res: Alarms.rc
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\$(PROJ).res Alarms.rc


$(OUTDIR)\$(PROJ).exe:  $(OUTDIR)\precomp.pch  $(LINK32_OBJS) $(PROJ).res
     $(link) $(ldebug) $(guilflags) $(guilibs) $(olelibs) $(OUTDIR)\$(PROJ).res /MACHINE:$(CPU) /PDB:$(OUTDIR)\$(PROJ).pdb -out:$(OUTDIR)\$(PROJ).exe $(LINK32_OBJS) $(EXTRA_LIBS)

clean:
     $(CLEANUP)
