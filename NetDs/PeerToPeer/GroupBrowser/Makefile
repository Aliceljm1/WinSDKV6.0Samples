!include <Win32.Mak>

SOURCES=         \
    GroupBrowser.c  \

OBJS=                         \
    $(OUTDIR)\GroupBrowser.obj   \
    $(OUTDIR)\GroupBrowser.res   \

LIBS=               \
    comctl32.lib    \
    p2p.lib         \

ALL: $(OUTDIR) $(OUTDIR)\GroupBrowser.exe

"$(OUTDIR)":
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(OUTDIR)\GroupBrowser.res: GroupBrowser.rc
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\GroupBrowser.res GroupBrowser.rc

.c{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) /DUNICODE /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

$(OUTDIR)\GroupBrowser.exe: $(OBJS)
    $(link) $(ldebug) $(guilflags) /PDB:$(OUTDIR)\GroupBrowser.pdb -out:$(OUTDIR)\GroupBrowser.exe $(OBJS) $(guilibs) $(LIBS)

clean:
        $(CLEANUP)
