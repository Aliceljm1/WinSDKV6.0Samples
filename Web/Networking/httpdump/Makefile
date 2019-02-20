all:headdump.exe

!include <win32.mak>

#lflags= /INCREMENTAL:NO /PDB:NONE /RELEASE /NOLOGO -subsystem:console,$(APPVER) -entry:mainCRTStartup
headdump.obj: headdump.cpp
    $(cc) $(cflags) $(cvars) $(cdebug) headdump.cpp

headdump.exe: headdump.obj
    $(link) $(lflags) $(conlflags) $(ldebug) -out:headdump.exe headdump.obj wininet.lib  $(winlibs)

clean:
    del *.exe
    del *.obj

