
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct userPost {
	char *username;
	char *streamname;
	char *date;
	char *text;
};

int removeCharFromString(char * string, char c);
void updateStream(struct userPost *st); 
void addUser(char *username, char *list);
void removeUser(char *username, char *list);  
void performAdd(char *token, char *name);
void performRemove(char *token, char *name);