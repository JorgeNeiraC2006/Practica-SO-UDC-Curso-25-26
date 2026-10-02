
# Nombre                
# Jorge Neira Cociña      

CC=gcc
CFLAGS=-Wall -g

all: shell

shell: dynamic_list.o p0.o p1.o main.o file_list.o mem_list.o p2.o p3.o proc_list.o
	$(CC) $(CFLAGS) -o shell main.o p0.o p1.o p2.o dynamic_list.o file_list.o mem_list.o proc_list.o p3.o

dynamic_list.o: dynamic_list.c
	$(CC) $(CFLAGS) -c dynamic_list.c

file_list.o: file_list.c
	$(CC) $(CFLAGS) -c file_list.c

mem_list.o: mem_list.c
	$(CC) $(CFLAGS) -c mem_list.c

proc_list.o: proc_list.c
	$(CC) $(CFLAGS) -c proc_list.c

p0.o: p0.c dynamic_list.h file_list.h mem_list.h proc_list.h
	$(CC) $(CFLAGS) -c p0.c

p1.o: p1.c dynamic_list.h file_list.h mem_list.h proc_list.h
	$(CC) $(CFLAGS) -c p1.c

p2.o: p2.c dynamic_list.h file_list.h mem_list.h proc_list.h
	$(CC) $(CFLAGS) -c p2.c

p3.o: p3.c dynamic_list.h file_list.h mem_list.h proc_list.h
	$(CC) $(CFLAGS) -c p3.c

main.o: main.c commands.h
	$(CC) $(CFLAGS) -c main.c

run:shell
	./shell

valgrind: shell
	valgrind --show-reachable=yes --leak-check=full ./shell

clean:
	rm -f *.o shell
