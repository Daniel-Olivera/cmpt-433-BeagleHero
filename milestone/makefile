OUTFILE_CLIENT = client
OUTFILE_SERVER = server
OUTDIR = $(HOME)/CMPT433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread
NOWORKY_FLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: client server

client:
	$(CC_C) $(CFLAGS) main.c client.c -o $(OUTDIR)/$(OUTFILE_CLIENT)

server:
	$(CC_C) $(CFLAGS) main2.c server.c led.c -o $(OUTDIR)/$(OUTFILE_SERVER)

clean:
	rm $(OUTDIR)/$(OUTFILE)