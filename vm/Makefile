CFLAGS = -g 

OBJECTS  = isa.o memory.o
COMMON = ../common/debug.o ../common/ioutils.o ../common/util.o

INCLUDES = -I ../common/

plvm: main.o $(OBJECTS) $(COMMON)
	gcc $(CFLAGS) $(INCLUDES) $^ -o $@
	-chmod 755 $@
	@-pause

.PHONY: all
all: $(OBJECTS)

main.o: main.c memory.h isa.h register.h ../common/instruction.h ../common/debug.h ../common/ioutils.h
	gcc $(CFLAGS) $(INCLUDES) -c $<

isa.o: isa.c isa.h register.h ../common/debug.h ../common/instruction.h
	gcc $(CFLAGS) $(INCLUDES) -c $<

memory.o: memory.c memory.h isa.h register.h ../common/debug.h ../common/instruction.h
	gcc $(CFLAGS) $(INCLUDES) -c $<

clean:
	-rm *.o
	-rm plvm

%.o: %.c

$(COMMON): common

.PHONY: common
common:
	$(MAKE) -C ../common $(notdir $(COMMON))
