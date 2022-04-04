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


#jzero stuff
j0: jzero_main.o lex.yy.o j0gram.tab.o token.o tree.o error.o symt.o type.o
	$(CC) $(CFLAGS) $^ -o $@

jzero_main.o: jzero_main.c
	$(CC) $(CFLAGS) -c $^

token.o: token.c token.h
	$(CC) $(CFLAGS) -c $^

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c $^

error.o: error.c error.h
	$(CC) $(CFLAGS) -c $^

lex.yy.o: lex.yy.c tree.h
	$(CC) $(CFLAGS) -c $^

j0gram.tab.o: j0gram.tab.c j0gram.y tree.h
	$(CC) $(CFLAGS) -c j0gram.tab.c

symt.o: symt.c symt.h
	$(CC) $(CFLAGS) -c $^

type.o: type.c type.h
	$(CC) $(CFLAGS) -c $^

.PHONY: clean
clean:
	rm -rf *.o $(targets)
	rm lex.yy.c
	rm -rf j0gram.tab.*
	rm *.h.gch
