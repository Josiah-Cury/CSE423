CC=gcc
CFLAGS=-g -Wall
FLEX=flex
BISON=bison

targets = bison j0lex j0
all: $(targets)

# Flex Object
j0lex: j0lex.l
	$(FLEX) $@.l
	$(CC) $(CFLAGS) -c lex.yy.c

# Bison Object
bison: j0gram.y
	$(BISON) -v j0gram.y
	$(BISON) -d j0gram.y


j0: jzero_main.o lex.yy.o j0gram.tab.o token.o tree.o
	$(CC) $(CFLAGS) $^ -o $@

jzero_main.o: jzero_main.c
	$(CC) $(CFLAGS) -c $^

token.o: token.c
	$(CC) $(CFLAGS) -c $^

tree.o: tree.c
	$(CC) $(CFLAGS) -c $^

lex.yy.o: lex.yy.c tree.h
	$(CC) $(CFLAGS) -c $^

j0gram.tab.o: j0gram.tab.c j0gram.y tree.h
	$(CC) $(CFLAGS) -c j0gram.tab.c


.PHONY: clean
clean:
	rm -rf *.o $(targets)
	rm lex.yy.c
	rm -rf j0gram.tab.*
