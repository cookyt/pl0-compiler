DEBUG_FLAGS = -g
OBJECTS = debug.o ioutils.o buffer.o util.o instructionbuffer.o filebuffer.o \
		  tokenbuffer.o symboltree.o symboltable.o symbol.o

.PHONY: all
all: $(OBJECTS)
	@-pause

.PHONY: clean
clean:
	-rm *.o

symboltree.o: symboltree.c symboltree.h symboltable.h symbol.h util.h debug.h
	gcc $(DEBUG_FLAGS) -c $<

symboltable.o: symboltable.c symboltable.h symbol.h token.h debug.h util.h
	gcc $(DEBUG_FLAGS) -c $<

symbol.o: symbol.c symbol.h token.h util.h
	gcc $(DEBUG_FLAGS) -c $<

debug.o: debug.c debug.h
	gcc $(DEBUG_FLAGS) -c $<

ioutils.o: ioutils.c debug.h ioutils.h util.h
	gcc $(DEBUG_FLAGS) -c $<

buffer.o: buffer.c buffer.h debug.h token.h
	gcc $(DEBUG_FLAGS) -c $<

util.o: util.c util.h debug.h
	gcc $(DEBUG_FLAGS) -c $<

instructionbuffer.o: instructionbuffer.c debug.h
	gcc $(DEBUG_FLAGS) -c $<

filebuffer.o: filebuffer.c debug.h util.h filebuffer.h buffer.h
	gcc $(DEBUG_FLAGS) -c $<

tokenbuffer.o: tokenbuffer.c tokenbuffer.h debug.h util.h token.h
	gcc $(DEBUG_FLAGS) -c $<
