all: P1

clean:
	rm *.o
	rm p1

utils.o:
	gcc moduls/utils.c -c

fat.o:
	gcc moduls/fat.c -c

ext.o:
	gcc moduls/ext.c -c

P1.o:
	gcc P1.c -c

P1: utils.o fat.o ext.o P1.o
	gcc utils.o fat.o ext.o P1.o -o p1
