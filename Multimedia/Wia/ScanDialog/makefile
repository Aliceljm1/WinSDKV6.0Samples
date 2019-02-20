!IF ("$(TARGETOS)" == "WINNT") && ("$(APPVER)" >= "6.0")

!IF ("$(CPU)" == "IA64") 
!MESSAGE Sorry, this sample is not supported on the IA64 platform!

!ELSE
# Include the common makefile
!include <win32.mak>

PROJECT = ScanDialog

# OUTDIR is the output directory, and is defined in win32.mak.
all: $(OUTDIR) $(OUTDIR)\ScanDialog.exe
 
# If OUTDIR does not exist, create it
"$(OUTDIR)" :
	if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)
 
# Build rule for the EXE
$(OUTDIR)\ScanDialog.exe:  $(OUTDIR) $(OUTDIR)\ScanDialog.obj  
	$(link) $(ldebug) $(conflags) /PDB:$(OUTDIR)\ScanDialog.pdb -out:$(OUTDIR)\ScanDialog.exe $(OUTDIR)\ScanDialog.obj $(conlibs) wiaguid.lib ole32.lib oleaut32.lib

# embedd the manifest
    if exist $@.manifest mt.exe -manifest $@.manifest -outputresource:$@;1

# Inference rule for building object files from cpp files
.cpp{$(OUTDIR)}.obj:
	$(cc) $(cdebug) $(cflags) $(cvarsdll) /WX /D_UNICODE /DUNICODE /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

# Remove created files
clean:
	$(CLEANUP)

!ENDIF

!ELSE
!MESSAGE Sorry this sample only builds for the Windows Vista Platform or higher!
!ENDIF
