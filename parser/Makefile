CFLAGS = -g 

OBJECTS  = syntax.o
COMMON = ../common/debug.o ../common/ioutils.o ../common/util.o \
		../common/instructionbuffer.o ../common/symbol.o \
		../common/symboltable.o ../common/symboltree.o \
		../common/tokenbuffer.o ../common/debug.o 

INCLUDES = -I ../common

plparse: main.o $(OBJECTS) $(COMMON)
	gcc $(CFLAGS) $(INCLUDES) $^ -o $@
	-chmod 755 $@
	@-pause

.PHONY: all
all: $(OBJECTS)

.PHONY: clean
clean:
	-rm *.o
	-rm plparse

main.o: main.c ../common/ioutils.h ../common/debug.h
	gcc $(CFLAGS) $(INCLUDES)  -c $<

syntax.o: syntax.c syntax.h ../common/tokenbuffer.h ../common/instructionbuffer.h
	gcc $(CFLAGS) $(INCLUDES)  -c $<

%.o: %.c

$(COMMON): common

.PHONY: common
common:
	$(MAKE) -C ../common $(notdir $(COMMON))
