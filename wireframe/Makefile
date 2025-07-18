
all: cube

tables.h: make_tables.c
	gcc make_tables.c -o make_tables -lm
	./make_tables

test: cube
	xvic -memory 8k cube

cube: cube.c tables.h
	cl65 -I . -I /usr/share/cc65/include --asm-include-dir /usr/share/cc65/asminc -L /usr/share/cc65/lib -D DYN_DRV=0 -t vic20 -C /usr/share/cc65/cfg/vic20-tgi.cfg cube.c -o cube 

clean:
	rm -f cube plots.h make_tables
