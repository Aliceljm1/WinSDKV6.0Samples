#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>

PROJ = WindowsSideShowPictures

all:    $(OUTDIR) $(OUTDIR)\$(PROJ).exe postbuild

$(OUTDIR):
     if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

EXTRA_LIBS = WindowsSideShowGuids.lib gdiplus.lib shell32.lib

cflags = $(cflags) /D "UNICODE"

LINK32_OBJS = $(OUTDIR)\BaseClient.obj \
              $(OUTDIR)\BaseContent.obj \
              $(OUTDIR)\BaseEvents.obj \
              $(OUTDIR)\ImageContent.obj \
              $(OUTDIR)\Picture.obj \
              $(OUTDIR)\PictureClient.obj \
              $(OUTDIR)\PictureContent.obj \
              $(OUTDIR)\Pictures.obj \
              $(OUTDIR)\XmlDocument.obj \
              $(OUTDIR)\XmlElement.obj \
              $(OUTDIR)\XmlNode.obj \
              $(OUTDIR)\XmlNodeList.obj \
              $(OUTDIR)\stdafx.obj

$(OUTDIR)\precomp.pch: stdafx.cpp
     @$(cc) $(cflags) $(cdebug) $(cvars) /FD /EHsc /TP /D "WIN32" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /Yc /Fp"$(OUTDIR)\precomp.pch" $**
        
$(PROJ).res: Pictures.rc
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\$(PROJ).res Pictures.rc

.cpp{$(OUTDIR)}.obj:
     @$(cc) $(cflags) $(cdebug) $(cvars) /FD /EHsc /TP /D "WIN32" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\"  /Fp"$(OUTDIR)\precomp.pch" /Yustdafx.h $**

$(OUTDIR)\$(PROJ).exe: $(OUTDIR)\precomp.pch  $(LINK32_OBJS) $(PROJ).res
     $(link) $(ldebug) $(gdiplus) $(shell32) $(olelibs)  $(OUTDIR)\$(PROJ).res /subsystem:windows,6.00 /MACHINE:$(CPU) /PDB:$(OUTDIR)\$(PROJ).pdb -out:$(OUTDIR)\$(PROJ).exe $(LINK32_OBJS) $(EXTRA_LIBS)

postbuild: $(OUTDIR)
        copy /y $(OUTDIR)\$(PROJ).exe .

clean:
     $(CLEANUP)
