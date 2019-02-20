#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>

PROJ = CATLPwr
all:    $(OUTDIR) $(OUTDIR)\$(PROJ).dll

$(OUTDIR):
     if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

EXTRA_LIBS =

LINK32_OBJS = $(OUTDIR)\stdafx.obj \
              $(OUTDIR)\power.obj \
              $(OUTDIR)\context.obj \
              $(OUTDIR)\catlpwr.obj \
              $(OUTDIR)\catlpwr.res

$(OUTDIR)\catlpwr.h: catlpwr.idl
     $(midl) /h $(OUTDIR)\catlpwr.h /tlb $(OUTDIR)\catlpwr.tlb /iid $(OUTDIR)\catlpwr_i.c catlpwr.idl

$(OUTDIR)\stdafx.obj:     stdafx.cpp
     $(cc) $(cflags) $(cdebug) $(cvarsdll) /D_USRDLL /D_MBCS /D_WINDLL /Yc"stdafx.h" /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /I$(OUTDIR) stdafx.cpp

.cpp{$(OUTDIR)}.obj:
     $(cc) $(cflags) $(cdebug) $(cvarsdll) /D_USRDLL /D_MBCS /D_WINDLL /Yu"stdafx.h" /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" /I$(OUTDIR) $**

$(OUTDIR)\catlpwr.res:
     $(rc) $(rcflags) $(rcvars) /Fo$(OUTDIR)\catlpwr.res /I$(OUTDIR) catlpwr.rc

$(OUTDIR)\$(PROJ).dll:   $(OUTDIR)\catlpwr.h $(LINK32_OBJS)
     $(link) $(ldebug) $(dlllflags) /MACHINE:$(CPU) /PDB:$(OUTDIR)\$(PROJ).pdb -out:$(OUTDIR)\$(PROJ).dll /DEF:catlpwr.def  $(LINK32_OBJS) $(EXTRA_LIBS) $(winlibs)

clean:
     $(CLEANUP)
