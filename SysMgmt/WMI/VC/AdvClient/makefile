#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>

PROJ = AdvClient
all:    $(OUTDIR) $(OUTDIR)\$(PROJ).exe

$(OUTDIR):
     if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

#!ifndef NODEBUG

#!if "$(MSVCVER)" == "7.0"
#EXTRA_LIBS = mfc70d.lib 
#!elseif "$(MSVCVER)" == "7.1"
#EXTRA_LIBS = mfc71d.lib 
#!elseif "$(MSVCVER)" == "8.0"
#EXTRA_LIBS = mfc80d.lib 
#!else
#EXTRA_LIBS = mfco42d.lib 
#!endif

#!else
#!if "$(MSVCVER)" == "7.0"
#EXTRA_LIBS = mfc70.lib 
#!elseif "$(MSVCVER)" == "7.1"
#EXTRA_LIBS = mfc71.lib 
#!elseif "$(MSVCVER)" == "8.0"
#EXTRA_LIBS = mfc80.lib 
#!else
#EXTRA_LIBS = mfc42.lib 
#!endif

#!endif #NODEBUG

EXTRA_LIBS = $(EXTRA_LIBS) wbemuuid.lib

LINK32_OBJS = $(OUTDIR)\stdafx.obj \
              $(OUTDIR)\ontemp.obj \
              $(OUTDIR)\onregister.obj \
              $(OUTDIR)\onperm.obj \
              $(OUTDIR)\onenumsvcs.obj \
              $(OUTDIR)\onenumdisks.obj \
              $(OUTDIR)\ondiskpropsdescriptions.obj \
              $(OUTDIR)\ondiskdetails.obj \
              $(OUTDIR)\onconnect.obj \
              $(OUTDIR)\onasync.obj \
              $(OUTDIR)\onaddequipment.obj \
              $(OUTDIR)\officedlg.obj \
              $(OUTDIR)\advclientdlg.obj \
              $(OUTDIR)\advclient.obj \
              $(OUTDIR)\advclient.res

$(OUTDIR)\stdafx.obj:     stdafx.cpp
     $(cc) $(cflags) $(cdebug) $(cvarsdll) /D_WIN32_DCOM /D_AFXDLL /Yc"stdafx.h" /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" stdafx.cpp

.cpp{$(OUTDIR)}.obj:
     $(cc) $(cflags) $(cdebug) $(cvarsdll) /D_WIN32_DCOM /D_AFXDLL /Yu"stdafx.h" /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

$(OUTDIR)\advclient.res:
     $(rc) $(rcflags) $(rcvars) /Fo$(OUTDIR)\advclient.res advclient.rc

$(OUTDIR)\$(PROJ).exe:   $(LINK32_OBJS)
     $(link) $(ldebug) $(guilflags) /MACHINE:$(CPU) /PDB:$(OUTDIR)\$(PROJ).pdb -out:$(OUTDIR)\$(PROJ).exe $(LINK32_OBJS) $(EXTRA_LIBS) $(olelibs) 

clean:
     $(CLEANUP)
