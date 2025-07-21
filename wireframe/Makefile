CC65 = /usr/share/cc65

.PHONY: all
all: cube pyr3

make_tables: make_tables.c
	gcc make_tables.c -o make_tables -lm
	./make_tables

cube.h: make_tables

pyr3.h: make_tables

.PHONY: test-cube
test-cube: cube
	xvic -memory 8k cube

.PHONY: test-pyr3
test-pyr3: pyr3
	xvic -memory 8k pyr3

cube: cube.c cube.h
	cl65 -I . -I ${CC65}/include --asm-include-dir ${CC65}/asminc -L $(CC65)/lib -D DYN_DRV=0 -t vic20 -C ${CC65}/cfg/vic20-tgi.cfg $@.c -o $@

pyr3: pyr3.c pyr3.h
	cl65 -I . -I ${CC65}/include --asm-include-dir ${CC65}/asminc -L $(CC65)/lib -D DYN_DRV=0 -t vic20 -C ${CC65}/cfg/vic20-tgi.cfg $@.c -o $@

.PHONY: clean
clean:
	rm -f cube cube.h make_tables cube.c.*.s pyr3 pyr3.h pyr3.c.*.s
