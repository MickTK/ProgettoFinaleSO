output: main.o gioco.o astronave.o navicelle.o
	gcc -o Space_Invaders main.o gioco.o astronave.o navicelle.o -lncurses -lpthread
	
main.o: main.c header.h
	gcc -c -o main.o main.c

gioco.o: gioco.c header.h
	gcc -c -o gioco.o gioco.c

astronave.o: astronave.c header.h
	gcc -c -o astronave.o astronave.c

navicelle.o: navicelle.c header.h
	gcc -c -o navicelle.o navicelle.c

clean:
	rm Space_Invaders
	rm *.o
