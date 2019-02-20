!include <Win32.Mak>

SOURCES=     \
    invite.c    \

OBJS=                         \
    $(OUTDIR)\invite.obj   \

LIBS=               \
    p2p.lib         \
    rpcrt4.lib         \

ALL: $(OUTDIR) $(OUTDIR)\invite.exe

"$(OUTDIR)":
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

.c{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) /DUNICODE /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

$(OUTDIR)\invite.exe: $(OBJS)
    $(link) $(ldebug) $(conflags) /PDB:$(OUTDIR)\invite.pdb -out:$(OUTDIR)\invite.exe $(OBJS) $(conlibs) $(LIBS)

clean:
        $(CLEANUP)
