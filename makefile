default: project

main.o : main.c sharedMem.h sem.h
	gcc -g -c main.c -o main.o

HashTable.o : HashTable.c HashTable.h
	gcc -g -c HashTable.c -o HashTable.o

sharedMem.o : sharedMem.c sharedMem.h
	gcc -g -c sharedMem.c -o sharedMem.o

sem.o : sem.c sem.h
	gcc -g -c sem.c -o sem.o

project: main.o HashTable.o sharedMem.o sem.o
	gcc main.o HashTable.o sharedMem.o sem.o -o ergasia2

clean:
	-rm -f project main.o HashTable.o sharedMem.o sem.o
