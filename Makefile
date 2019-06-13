# Makefile for *NIX systems
# This requires WINE and MinGW to be installed properly
CC=wine gcc

all:	fix_survival.exe

fix_survival.exe:	fix_survival.c
	$(CC) -o$@ $^

clean:
	rm fix_survival.exe
