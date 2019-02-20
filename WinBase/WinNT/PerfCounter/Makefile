# Nmake macros for building Windows 32-Bit apps



!include <Win32.Mak>

all: $(OUTDIR) $(OUTDIR)\PerflibV2SampleProvider.exe

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

$(OUTDIR)\Main.obj: Main.c
    $(cc) $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" Main.c 

$(OUTDIR)\PerflibV2SampleProvider.obj: PerflibV2SampleProvider.c
    $(cc) $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" PerflibV2SampleProvider.c Main.c 

$(OUTDIR)\PerflibV2SampleProvider.res:	
	 $(rc) $(rcflags) /Fo$(OUTDIR)\PerflibV2SampleProvider.res PerflibV2SampleProvider.rc

$(OUTDIR)\PerflibV2SampleProvider.exe: $(OUTDIR)\PerflibV2SampleProvider.obj $(OUTDIR)\Main.obj $(OUTDIR)\PerflibV2SampleProvider.res
    $(link) $(conflags) -out:$(OUTDIR)\PerflibV2SampleProvider.exe $(OUTDIR)\PerflibV2SampleProvider.obj $(OUTDIR)\Main.obj $(OUTDIR)\PerflibV2SampleProvider.res $(conlibs) advapi32.lib user32.lib pdh.lib 

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
