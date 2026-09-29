# LitesOut -- OpenWatcom 2.0 wmake file for OS/2 32-bit PM

WATCOM  = $(%WATCOM)

CC      = wcc386
LINK    = wlink
RC      = wrc

OUT     = bin

CFLAGS  = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0 -i=src

LFLAGS  = system os2v2_pm &
          option stack=65536 &
          option heap=4096 &
          option map=$(OUT)\litesout.map &
          option quiet

all: $(OUT)\litesout.exe

$(OUT):
	@if not exist $(OUT) mkdir $(OUT)

$(OUT)\litesout.obj: src\litesout.c src\litesout.h src\lang.h $(OUT)
	$(CC) $(CFLAGS) -fo=$@ src\litesout.c

$(OUT)\litesout.res: src\litesout.rc src\litesout.h $(OUT)
	$(RC) -r -fo=$@ -i=src src\litesout.rc

$(OUT)\litesout.exe: $(OUT)\litesout.obj $(OUT)\litesout.res
	$(LINK) $(LFLAGS) &
	  file $(OUT)\litesout.obj &
	  name $(OUT)\litesout.exe
	$(RC) -q $(OUT)\litesout.res $(OUT)\litesout.exe

clean: .SYMBOLIC
	@if exist $(OUT)\litesout.obj del $(OUT)\litesout.obj >nul
	@if exist $(OUT)\litesout.res del $(OUT)\litesout.res >nul
	@if exist $(OUT)\litesout.map del $(OUT)\litesout.map >nul
	@if exist $(OUT)\litesout.exe del $(OUT)\litesout.exe >nul
