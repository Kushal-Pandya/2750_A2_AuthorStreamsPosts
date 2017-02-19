CC = gcc
CFLAGS = -Wall -ansi -g
LISTS_FILES = var.c func.c class.c list.c


all: A1 makePost A2



A1: a1.o list.o 
	$(CC) $(LISTS_FILES) $(CFLAGS) a1.o -o a1

a1.o: a1.c a1.h
	$(CC) $(CFLAGS) -c a1.c 

list.o: list.c a1.h
	$(CC) $(CFLAGS) -c list.c 

makePost:
	./a1 post.cc


A2: addauthor post

addauthor: addauthor.o
	$(CC) $(CFLAGS) addauthor.o -o addauthor -L. -lstream

addauthor.o: addauthor.c libstream.a
	$(CC) $(CFLAGS) -c addauthor.c -o addauthor.o

post: post.o
	$(CC) $(CFLAGS) post.o -o post -L. -lstream

post.o: post.c libstream.a 
	$(CC) $(CFLAGS) -c post.c -o post.o	

libstream.a: stream.c stream.h
	$(CC) $(CFLAGS)	stream.c -o stream.o -c ;\
	ar cr libstream.a stream.o

run:
	valgrind --leak-check=full --show-reachable=yes ./a2 

clean: 
	rm -f *.o a2 a1 assets.txt addauthor post libstream.a messages/*