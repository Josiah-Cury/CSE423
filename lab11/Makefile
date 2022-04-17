tac: lab11.o tac.o
	gcc lab11.o tac.o -o tac

lab11.o: lab11.c tac.h
	gcc -c lab11.c

tac.o: tac.c tac.h
	gcc -c tac.c

clean:
	rm tac
	rm *.o
