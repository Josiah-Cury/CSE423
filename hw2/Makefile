CC=gcc
CFLAGS=-g -Wall
FLEX=flex

targets = jzero j0
all: $(targets)

	
jzero: jzero.l
	$(FLEX) $@.l


j0: jzero_main.o lex.yy.o token.o
	$(CC) $(CFLAGS) $^ -o $@

jzero_main.o: jzero_main.c
	$(CC) $(CFLAGS) -c $^
	
token.o: token.c
	$(CC) $(CFLAGS) -c $^
	
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^
	

.PHONY: clean
clean:
	rm -rf *.o $(targets)
	rm lex.yy.c
