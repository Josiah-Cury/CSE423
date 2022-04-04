CC=gcc
CFLAGS=-g -Wall
FLEX=flex
BISON=bison

targets =bison flex  ns
all: $(targets)


# Bison Object
bison: ns.y
	$(BISON) ns.y
	$(BISON) -d ns.y


# Flex Object
flex: nnws.l
	$(FLEX) nnws.l
	$(CC) $(CFLAGS) -c lex.yy.c



# Lab 3 Executable
ns: main.o lex.yy.o ns.tab.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $^
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^
ns.tab.o: ns.tab.c ns.y
	$(CC) $(CFLAGS) -c ns.tab.c
	

.PHONY: clean
clean:
	rm -rf *.o $(targets)
	rm lex.yy.c
	rm -rf ns.tab.*
