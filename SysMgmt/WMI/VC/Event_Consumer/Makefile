#----- Include the PSDK's WIN32.MAK to pick up defines-------------------
!include <win32.mak>

TARGET  = SdkSample
OUTFILE = SdkSample.exe

#
#obj list.
#

#if DEBUG  :)
!ifndef NODEBUG

OUTDIR=.\DebugA   # where to put the exe.
INTDIR=.\DebugA   # where to put the objs.

LINK = oleaut32.lib ole32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\SdkSample.pdb" /debug\
 /out:"$(OUTDIR)\SdkSample.exe"

CFLAGS = /nologo /MDd /W3 /Gm /EHsc /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_WIN32_DCOM" /Fp"$(INTDIR)\SdkSample.pch" \
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c  /I . /I include -D_WIN32_WINNT=$(NMAKE_WINVER) -DNTDDI_VERSION=$(NMAKE_WINVER)0000

!else

OUTDIR=.\ReleaseA
INTDIR=.\ReleaseA

LINK = oleaut32.lib ole32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\SdkSample.pdb"\
 /out:"$(OUTDIR)\SdkSample.exe"
  
CFLAGS = /nologo /MD /W3 /O2 /EHsc /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_WIN32_DCOM" /Fp"$(INTDIR)\SdkSample.pch" \
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c  /I . /I include -D_WIN32_WINNT=$(NMAKE_WINVER) -DNTDDI_VERSION=$(NMAKE_WINVER)0000

!endif #NODEBUG

OBJS =  Globals.obj \
	Main.obj \
	Events.obj 

LINK_OBJS = $(INTDIR)\Globals.obj \
	$(INTDIR)\Main.obj\
	$(INTDIR)\Events.obj

LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib \
 advapi32.lib uuid.lib wbemuuid.lib

.SUFFIXES: .h .obj .dll .cpp  

!ifdef CLEAN
goal:   clean
!else
goal:   $(OUTFILE)
!endif #CLEAN

clean:
    if exist *.pch del *.pch
    if exist *.obj del *.obj
    if exist *.exe del *.exe
    if exist *.dll del *.dll
    if exist *.pdb del *.pdb
    if exist *.exp del *.exp
    if exist *.lib del *.lib
    if exist *.ilk del *.ilk
    if exist *.map del *.map
    if exist *.vcp del *.vcp

.cpp.obj:
	if not exist $(INTDIR) md $(INTDIR)
    cl $(CFLAGS) $*.cpp

$(OUTFILE) : $(OBJS) $(DEFFILE)
    link @<<    
    $(LINK)
    $(LINK_OBJS)
    $(LIBS) 
<<

##### Dependencies #####

Globals.obj : Globals.cpp Globals.h Precomp.h
Main.obj : Main.cpp Globals.h Events.h Precomp.h
Events.obj : Events.cpp Globals.h Events.h Precomp.h



