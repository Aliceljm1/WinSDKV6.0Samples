!include <Win32.Mak>

SOURCES=     \
    pnm.c    \

OBJS=                         \
    $(OUTDIR)\pnm.obj   \

LIBS=               \
    p2p.lib         \
    Rpcrt4.lib      \

ALL: $(OUTDIR) $(OUTDIR)\pnm.exe

"$(OUTDIR)":
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

.c{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) /DUNICODE /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

$(OUTDIR)\pnm.exe: $(OBJS)
    $(link) $(ldebug) $(conflags) /PDB:$(OUTDIR)\pnm.pdb -out:$(OUTDIR)\pnm.exe $(OBJS) $(conlibs) $(LIBS)

clean:
        $(CLEANUP)
