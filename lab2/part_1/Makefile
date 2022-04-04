lCC=gcc
CFLAGS=-g -Wall
FLEX=flex

targets = flex-wc lab2_1
all: $(targets)


flex-wc: flex-wc.l
	$(FLEX) $@.l

	
# Lab 2: Part 1	

lab2_1: main.o lex.yy.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $^
	
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^

.PHONY: clean
clean:
	rm -rf *.o $(targets)
	rm lex.yy.c
	
#lab2_1: main.c lex.yy.c
#	$(CC) $(CFLAGS) $^ -o $@ 
