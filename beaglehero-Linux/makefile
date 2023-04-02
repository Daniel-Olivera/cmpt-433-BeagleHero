TARGET = beagleHero
SOURCES = main.c guitar.c timing.c memoryShare.c beatmap.c freeplay.c buzzer.c midiParser.c
OUTDIR = $(HOME)/cmpt433/public/project

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread
SMFFLAGS = `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -lsmf -lpcre


LFLAGS = -L$(HOME)/cmpt433/public/BeagleHero_Libs

app:
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) $(SMFFLAGS) -lwiiuse -lbluetooth
	mkdir -p $(OUTDIR)/midi-files
	cp ./midi-files/* $(OUTDIR)/midi-files/	

clean:
	rm $(OUTDIR)/$(TARGET)

all: app