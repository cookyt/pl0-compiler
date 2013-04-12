CFLAGS = -g 

OBJECTS  = printtoken.o tokenizer.o
COMMON = ../common/debug.o ../common/ioutils.o ../common/buffer.o \
		 ../common/tokenbuffer.o ../common/filebuffer.o ../common/util.o

INCLUDES = -I ../common

plscan: main.o $(OBJECTS) $(COMMON)
	gcc $(CFLAGS) $(INCLUDES) $^ -o $@
	-chmod 755 $@
	@-pause

.PHONY: all
all: $(OBJECTS)

clean:
	-rm *.o
	-rm plscan

main.o: main.c ../common/ioutils.h ../common/token.h ../common/debug.h
	gcc $(CFLAGS) $(INCLUDES)  -c $<

tokenizer.o: tokenizer.c tokenizer.h ../common/debug.h
	gcc $(CFLAGS) $(INCLUDES)  -c $<

printtoken.o: printtoken.c printtoken.h ../common/token.h
	gcc $(CFLAGS) $(INCLUDES)  -c $<

%.o: %.c

$(COMMON): common

.PHONY: common
common:
	$(MAKE) -C ../common $(notdir $(COMMON))
