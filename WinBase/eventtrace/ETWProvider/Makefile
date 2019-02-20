# Copyright (C) Microsoft Corporation.  All Rights Reserved.

!IF "$(TARGETOS)" != "WINNT" || ("$(APPVER)" < "5.01") && ("$(APPVER)" < "5.0")
!MESSAGE  Sorry, etwphils is not supported on non NT platforms
!MESSAGE  and is only supported on Windows 2000 or higher
!ELSE

!include <win32.mak>

PROJ = etwphils
EVENTS = $(PROJ).xmlEvents

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe

$(OUTDIR):
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

PROJ_OBJS = $(OUTDIR)\$(PROJ).obj $(OUTDIR)\$(EVENTS).res

$(OUTDIR)\$(PROJ).exe: $(OUTDIR)\$(EVENTS).rc	\
	$(OUTDIR)\$(PROJ).obj	


$(OUTDIR)\$(EVENTS).rc:
   $(mc) -b -W "$(MSSDK)\Include\winmeta.xml" \
   -h $(OUTDIR)                               \
   -r $(OUTDIR)                               \
   -z $(EVENTS)                               \
   $(PROJ).xml

$(OUTDIR)\$(PROJ).obj: $(PROJ).c
   $(cc) $(cflags) $(cdebug) $(cvars)         \
   /Fo$(OUTDIR)\\                             \
   /Fd$(OUTDIR)\\                             \
   /I$(OUTDIR)                                \
   $(PROJ).c                                  \

$(OUTDIR)\$(PROJ).exe: $(PROJ_OBJS)
   $(link) $(conlflags) $(linkdebug)          \
   $(PROJ_OBJS)                               \
   $(conlibs) shell32.lib user32.lib          \
   -out:$(OUTDIR)\$(PROJ).exe                   


clean:
        $(CLEANUP)


!ENDIF
