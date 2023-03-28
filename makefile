TARGET = beagleHero
SOURCES = main.c guitar.c
OUTDIR = $(HOME)/cmpt433/public/project

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread

LFLAGS = -L$(HOME)/cmpt433/public/wiimote_lib_BBG

app:
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lwiiuse -lbluetooth

clean:
	rm $(OUTDIR)/$(TARGET)

all: app