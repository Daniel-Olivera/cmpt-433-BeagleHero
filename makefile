SOURCES = guitar.c
OUTFILE_CLIENT = guitarToTCP
OUTFILE_SERVER = server
OUTDIR = $(HOME)/cmpt433/public/project

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread

LFLAGS = -L$(HOME)/cmpt433/public/wiimote_lib_BBG

all: client server

client:
	$(CC_C) $(CFLAGS) guitar.c -o $(OUTDIR)/$(OUTFILE_CLIENT)  $(LFLAGS) -lwiiuse -lbluetooth

server:
	$(CC_C) $(CFLAGS) main2.c server.c led.c -o $(OUTDIR)/$(OUTFILE_SERVER)

clean:
	rm $(OUTDIR)/$(OUTFILE_CLIENT)
	rm $(OUTDIR)/$(OUTFILE_SERVER)
