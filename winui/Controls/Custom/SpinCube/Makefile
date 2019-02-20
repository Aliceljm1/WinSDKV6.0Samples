#----- Include the PSDK's WIN32.MAK to pick up defines------------------------------------
!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory-------
all: $(OUTDIR)\SpinTest.Exe $(OUTDIR)\SpinCube.Dll

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

SOURCE=.\spintest.c

$(OUTDIR)\SpinTest.Obj: $(OUTDIR) $(SOURCE)
	$(cc) $(cdebug) $(cflags) $(cvarsdll) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $(SOURCE)

SOURCE=.\spincube.c

$(OUTDIR)\SpinCube.Obj: $(OUTDIR) $(SOURCE) SpinCube.h
	$(cc) $(cdebug) $(cflags) $(cvarsdll) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $(SOURCE)

SOURCE=.\paint.c

$(OUTDIR)\Paint.Obj: $(OUTDIR) $(SOURCE) Paint.h SpinCube.h
	$(cc) $(cdebug) $(cflags) $(cvarsdll) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $(SOURCE)

SOURCE=.\SPINTEST.RC
# Build rule for resource files
$(OUTDIR)\SpinTest.Res: $(OUTDIR) SpinTest.Rc SpinTest.h SpinTest.Dlg
	$(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\SpinTest.res $(SOURCE)

SOURCE=.\SpinCube.rc

$(OUTDIR)\SpinCube.Res: $(OUTDIR) SpinCube.Rc
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\SpinCube.res $(SOURCE)

# Build rule for DLL
$(OUTDIR)\SpinCube.dll: $(OUTDIR)\spincube.obj $(OUTDIR)\paint.obj spincube.def $(OUTDIR)\SpinCube.res
    $(link) $(ldebug) $(dlllflags) $(OUTDIR)\spincube.obj $(OUTDIR)\paint.obj $(OUTDIR)\SpinCube.res $(guilibsdll) -out:$(OUTDIR)\SpinCube.dll -def:SpinCube.def
    mt -manifest $(OUTDIR)\SpinCube.dll.manifest -outputresource:$(OUTDIR)\spincube.dll;2


# Build rule for EXE
$(OUTDIR)\SpinTest.exe: $(OUTDIR) $(OUTDIR)\spincube.dll $(OUTDIR)\spintest.obj spintest.def $(OUTDIR)\SpinTest.res
    $(link) $(ldebug) $(guilflags) /PDB:$(OUTDIR)\SpinTest.pdb -out:$(OUTDIR)\SpinTest.exe $(OUTDIR)\spintest.obj $(OUTDIR)\spincube.lib $(OUTDIR)\SpinTest.res $(guilibs)


#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
