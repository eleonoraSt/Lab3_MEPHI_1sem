.PHONY: clean

stack.o: stack.c
	gcc -c -o stack.o stack.c

sort.o: sort.c
	gcc -c -o sort.o sort.c

io.o: io.c
	gcc -c -o io.o io.c

main.o: main.c
	gcc -c -o main.o main.c

stack: stack.o sort.o io.o main.o
	gcc -o stack stack.o sort.o io.o main.o

clean:
	del *.o *.exe test.txt