all: filesFinder 

filesFinder: gestionFichier.o main.o
	gcc -o filesFinder gestionFichier.o main.o

gestionFichier.o: gestionFichier.c
	gcc -o gestionFichier.o -c gestionFichier.c 

main.o: main.c gestionFichier.h
	gcc -o main.o -c main.c 

clean:
	rm -rf *.o

mrproper: clean
	rm -rf filesFinder
