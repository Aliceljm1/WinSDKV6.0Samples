#----- set the project name --------------------------------------------------------------
Proj = PropSheetHost

#----- Include the PSDK's WIN32.MAK to pick up defines -----------------------------------
!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory ------
all: $(OUTDIR)\$(Proj).exe

LINK_OBJS= \
	$(OUTDIR)\StdAfx.obj \
	$(OUTDIR)\Main.obj \
	$(OUTDIR)\DataObj.obj \
	$(OUTDIR)\PropSheetHost.obj
        
#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

#----- add the preprocessor definitions needed by this project ---------------------------
mycflags = $(cflags) -GX -DUNICODE -D_UNICODE /EHsc

#----- set the libraries needed by this project ------------------------------------------
LINK_LIBS = $(olelibs) adsiid.lib activeds.lib urlmon.lib comctl32.lib credui.lib

#--------------------- EXE ---------------------------------------------------------------
#PLEASE REFER TO WIN32.MAK for the different Linker options WIN32.MAK provides

# Build rule for EXE
$(OUTDIR)\$(Proj).exe:  $(OUTDIR) $(LINK_OBJS) 
    $(link) $(ldebug) $(conlflags) /PDB:$(OUTDIR)\$(Proj).pdb -out:$(OUTDIR)\$(Proj).exe $(LINK_OBJS) $(LINK_LIBS) 

#--------------------- Compiling C/CPP Files ---------------------------------------------
#PLEASE REFER TO WIN32.MAK for the different Compiler options WIN32.MAK provides

FILE=StdAfx

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(mycflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Main

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(mycflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=DataObj

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(mycflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=PropSheetHost

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(mycflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
