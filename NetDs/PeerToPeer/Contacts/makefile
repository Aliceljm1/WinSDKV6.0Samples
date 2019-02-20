!include <Win32.Mak>

SOURCES=        \
    Contacts.c  \

OBJS=                         \
    $(OUTDIR)\Contacts.obj   \

LIBS=               \
    p2p.lib         \

ALL: $(OUTDIR) $(OUTDIR)\Contacts.exe

"$(OUTDIR)":
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

.c{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) /DUNICODE /Fp"$(OUTDIR)\\" /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

$(OUTDIR)\Contacts.exe: $(OBJS)
    $(link) $(ldebug) $(conflags) /PDB:$(OUTDIR)\Contacts.pdb -out:$(OUTDIR)\Contacts.exe $(OBJS) $(conlibs) $(LIBS)

clean:
        $(CLEANUP)
