#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                   WMarsh Application                    **#
#**        						    **#
#**                                                         **#
#*************************************************************#

!include <Win32.Mak>

!if "$(CPU)" == "i386"
cflags = $(cflags) -D_CRTAPI1=_cdecl -D_CRTAPI2=_cdecl 
!else
cflags = $(cflags) -D_CRTAPI1= -D_CRTAPI2= 
!endif

cflags = $(cflags) -GS 

!if "$(CPU)" == "i386"
link = $(link) -SAFESEH
!endif

all : WMarshc WMarshs

# Make the client
WMarshc : WMarshc.exe
WMarshc.exe : WMarshc.obj WMarshu.obj WMarsh_c.obj
    $(link) $(linkdebug) $(conflags) -out:WMarshc.exe -map:WMarshc.map \
      WMarshc.obj WMarshu.obj WMarsh_c.obj \
      rpcrt4.lib $(conlibsdll)

# WMarsh client main program
WMarshc.obj : WMarshc.c WMarsh.h WMarshu.h
   $(cc) $(cdebug) $(cflags) $(cvarsdll) /W3 $*.c

WMarshu.obj : WMarshu.c WMarsh.h WMarshu.h
   $(cc) $(cdebug) $(cflags) $(cvarsdll) /W3 $*.c

# WMarsh client stub
WMarsh_c.obj : WMarsh_c.c WMarsh.h
   $(cc) $(cdebug) $(cflags) $(cvarsdll) /W3 $*.c


# Make the server executable
WMarshs : WMarshs.exe
WMarshs.exe : WMarshs.obj WMarshu.obj WMarshp.obj WMarsh_s.obj
    $(link) $(linkdebug) $(conflags) -out:WMarshs.exe -map:WMarshs.map \
      WMarshs.obj WMarshu.obj WMarsh_s.obj WMarshp.obj \
      rpcrt4.lib $(conlibsdll)

# WMarsh server main
WMarshs.obj : WMarshs.c WMarsh.h WMarshu.h
   $(cc) $(cdebug) $(cflags) $(cvarsdll) /W3 $*.c

# remote procedures
WMarshp.obj : WMarshp.c WMarsh.h
   $(cc) $(cdebug) $(cflags) $(cvarsdll) /W3 $*.c

# WMarsh server stub file
WMarsh_s.obj : WMarsh_s.c WMarsh.h
   $(cc) $(cdebug) $(cflags) $(cvarsdll) /W3 $*.c

# Stubs and header file from the IDL file
WMarsh.h WMarsh_c.c WMarsh_s.c : WMarsh.idl WMarsh.acf
    midl $(MIDL_OPTIMIZATION) -oldnames -no_cpp  WMarsh.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del WMarsh_c.c
    -del WMarsh_s.c
    -del WMarsh.h
