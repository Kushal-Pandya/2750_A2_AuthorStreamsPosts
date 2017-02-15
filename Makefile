CC = gcc
CFLAGS = -Wall -ansi -g

all: addauthor post

addauthor: addauthor.o
	$(CC) $(CFLAGS) addauthor.o -o addauthor

addauthor.o: addauthor.c
	$(CC) $(CFLAGS) -c addauthor.c

post: post.o
	$(CC) $(CFLAGS) post.o -o post

post.o: post.c
	$(CC) $(CFLAGS) -c post.c		

A2: a2.o
	$(CC) $(CFLAGS) a2.o -o a2

a2.o: a2.c
	$(CC) $(CFLAGS) -c a2.c 

run:
	valgrind --leak-check=full --show-reachable=yes ./a2 

clean: 
	rm -f *.o a2 assets.txt addauthor messages/* post