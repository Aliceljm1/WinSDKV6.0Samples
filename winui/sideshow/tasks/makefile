#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>

!if "$(APPVER)" == "6.0"

PROJ = WindowsSideShowTasks
all:    $(OUTDIR) $(OUTDIR)\$(PROJ).dll $(OUTDIR)\TaskList.xml $(OUTDIR)\Tasks.ini
INCLUDE=$(INCLUDE);$(OUTDIR)

CPP_PROJ=/I"$(OUTDIR)" /Fo"$(OUTDIR)\\" /Fp"$(OUTDIR)\stdafx.pch" /Fd"$(OUTDIR)\\" /FD /c
CPP_ALL=$(cc) $(cflags) $(cvars) $(cdebug) $(CPP_PROJ) /EHsc /D "UNICODE"
MIDL_ALL=MIDL /robust /nologo /error all /D "MIDL_PASS" /out $(OUTDIR) /win32

$(OUTDIR):
     if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

EXTRA_LIBS = advapi32.lib shell32.lib psapi.lib WindowsSideShowGuids.lib

Tasks.h : Tasks.idl
    $(MIDL_ALL) /tlb Tasks.tlb /h Tasks.h Tasks.idl

LINK32_OBJS = $(OUTDIR)\stdafx.obj \
              $(OUTDIR)\BaseClient.obj \
              $(OUTDIR)\BaseContent.obj \
              $(OUTDIR)\BaseEvents.obj \
              $(OUTDIR)\GlanceContent.obj \
              $(OUTDIR)\Task.obj \
              $(OUTDIR)\Tasks.obj \
              $(OUTDIR)\TasksClient.obj \
              $(OUTDIR)\TasksContent.obj \
              $(OUTDIR)\TasksEvents.obj \
              $(OUTDIR)\XmlDocument.obj \
              $(OUTDIR)\XmlElement.obj \
              $(OUTDIR)\XmlNode.obj \
              $(OUTDIR)\XmlNodeList.obj \
              $(OUTDIR)\TasksPropPage.obj \
              $(OUTDIR)\Tasks.res

Tasks_i.c : Tasks.h

$(OUTDIR)\stdafx.obj:     stdafx.cpp Tasks.h Tasks_i.c
     $(CPP_ALL) stdafx.cpp

.cpp{$(OUTDIR)}.obj:
     $(CPP_ALL) $**

$(OUTDIR)\Tasks.res:
     $(rc) $(rcflags) $(rcvars) /Fo$(OUTDIR)\Tasks.res Tasks.rc

$(OUTDIR)\TaskList.xml: TaskList.xml
        copy /y $** $@

$(OUTDIR)\Tasks.ini: Tasks.ini
        copy /y $** $@

$(OUTDIR)\$(PROJ).dll:   $(LINK32_OBJS)
     $(link) $(ldebug) $(dlllflags) /MACHINE:$(CPU) /PDB:$(OUTDIR)\$(PROJ).pdb -out:$(OUTDIR)\$(PROJ).dll /DEF:WindowsSideShowTasks.def   $(LINK32_OBJS) $(EXTRA_LIBS) $(baselibs) $(olelibs) 

clean:
     $(CLEANUP)

!else

!message Sorry this sample only builds for the Windows Vista Platform

!endif