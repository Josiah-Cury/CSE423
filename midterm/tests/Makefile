CC=gcc
CFLAGS=-g -Wall
FLEX=flex

targets = flex testing
all: $(targets)


flex: comments.l
	$(FLEX) $^



# Lab 2: Part 2
testing: testing.o lex.yy.o
	$(CC) $(CFLAGS) $^ -o $@

testing.o: testing.c
	$(CC) $(CFLAGS) -c $^

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^


.PHONY: clean
clean:
	rm -rf *.o
	rm lex.yy.c
