CC=gcc
CFLAGS=-g -Wall
FLEX=flex

targets = jzero lab2_2
all: $(targets)

	
jzero: jzero.l
	$(FLEX) $@.l

	
	
# Lab 2: Part 2
lab2_2: jzero_main.o lex.yy.o
	$(CC) $(CFLAGS) $^ -o $@

jzero_main.o: jzero_main.c
	$(CC) $(CFLAGS) -c $^
	
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^
	

.PHONY: clean
clean:
	rm -rf *.o $(targets)
	rm lex.yy.c
