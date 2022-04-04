CC=gcc
CFLAGS=-g -Wall

targets = lab1_1 lab1_2 lab1_3 
all: $(targets)

lab1_1: lab1_1.c
	$(CC) $(CFLAGS) $@.c -o $@

lab1_2: lab1_2.c
	$(CC) $(CFLAGS) $@.c -o $@

lab1_3: lab1_3.o yylex.o
	$(CC) $(CFLAGS) $^ -o $@

yylex.o: yylex.c
	$(CC) $(CFLAGS) -c $^
	
lab1_3.o: lab1_3.c
	$(CC) $(CFLAGS) -c $^


.PHONY: clean
clean:
	rm -rf *.o $(targets)
