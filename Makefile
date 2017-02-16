CC = gcc
CFLAGS = -Wall -ansi -g

all: addauthor post

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
	rm -f *.o a2 assets.txt addauthor messages/* post libstream.a