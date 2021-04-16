all: P1

clean:
	rm *.o
	rm p1

sistema.o:
	gcc moduls/sistema.c -c

fat.o:
	gcc moduls/fat.c -c

ext.o:
	gcc moduls/ext.c -c

P1.o:
	gcc P1.c -c

P1: sistema.o fat.o ext.o P1.o
	gcc sistema.o fat.o ext.o P1.o -o p1
