CFLAGS = -g 

OBJECTS = main.o
COMMON = common/debug.o common/ioutils.o common/buffer.o common/tokenbuffer.o \
		common/filebuffer.o common/util.o common/symbol.o common/symboltable.o \
		common/symboltree.o common/instructionbuffer.o
SCANNER  = scanner/printtoken.o scanner/tokenizer.o
PARSER =  parser/syntax.o
VM = vm/isa.o vm/memory.o 

INCLUDES = -I common/

plrun: main.o $(OBJECTS) $(COMMON) $(SCANNER) $(PARSER) $(VM)
	gcc $(CFLAGS) $(INCLUDES) $^ -o $@
	-chmod 755 $@
	@echo SUCCESS
	@-pause

main.o: main.c
	gcc $(CFLAGS) $(INCLUDES) -c $<

clean:
	-rm *.o

clean-all: clean
	$(MAKE) -e -C common  clean
	$(MAKE) -e -C scanner clean
	$(MAKE) -e -C parser  clean
	$(MAKE) -e -C vm      clean

%.o: %.c

$(COMMON): common
$(SCANNER): scanner
$(PARSER): parser
$(VM): vm

.PHONY: common
common:
	$(MAKE) -C $@ $(notdir $(COMMON))

.PHONY: scanner
scanner:
	$(MAKE) -C $@ $(notdir $(SCANNER))

.PHONY: parser
parser:
	$(MAKE) -C $@ $(notdir $(PARSER))

.PHONY: vm
vm:
	$(MAKE) -C $@ $(notdir $(VM))
