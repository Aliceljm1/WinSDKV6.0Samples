# Nmake macros for building Windows 32-Bit apps

!include <win32.mak>

all: D3D9ExSample.exe


# Inference rule for updating the object files
.c.obj:
  $(cc) /D "_UNICODE" /D "UNICODE" $(cdebug) $(cflags) $(cvars) $*.c

.cpp.obj:
  $(cc) /D "_UNICODE" /D "UNICODE" $(cdebug) $(cflags) $(cvars) $*.cpp

# Update the executable file if necessary, and if so, add the resource back in.
D3D9ExSample.exe: D3D9ExSample.obj BackgroundThread.obj 
    $(link) $(linkdebug) $(guiflags) -out:D3D9ExSample.exe \
       D3D9ExSample.obj BackgroundThread.obj  d3d9.lib d3dx9.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib


# Clean up everything except .exe
clean:
    del *.obj *.res
