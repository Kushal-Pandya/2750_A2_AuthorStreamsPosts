/*
	CIS 2750
	A2
	Author: Kushal Pandya
	Due Date: February 17 2017

	POST PROGRAM
*/

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


struct userPost *formatEntry(char *name, char *stream, char *text) {

	struct userPost *newPost = malloc(sizeof(struct userPost));
	newPost->username = malloc(strlen(name)+1);
	newPost->streamname = malloc(strlen(stream)+1);
	newPost->text = malloc(strlen(text)+1);

	strcpy(newPost->username, name);
	strcpy(newPost->streamname, stream);
	strcpy(newPost->text, text);

	return newPost;
}


void readInput(char *name) {

	struct userPost *newPost;
	char *stream = malloc(sizeof(char)*100);
	char *text = malloc(sizeof(char)*1000);
	char *textBuffer = malloc(sizeof(char)*100);
	char ifEOF[2];

	printf("Stream: ");
	fgets(stream, 100, stdin);

	printf("Enter Text: ");
	while ((ifEOF[0] = getchar()) != EOF) {
		printf("- ");
		fgets(textBuffer, 100, stdin);
		strcat(text, ifEOF);
		strcat(text, textBuffer);
	}

	newPost = formatEntry(name, stream, text);

	printf("NAME:%s\n", newPost->username);
	printf("STREAM:%s\n", newPost->streamname);
	printf("TEXT:%s\n", newPost->text);
}



int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Not correct arguments\n");
		exit(0);
	}

	readInput(argv[1]);

/*	printf("NAME:%s\n", newPost->username);
	printf("STREAM:%s\n", newPost->streamname);
	printf("TEXT:%s\n", newPost->text);*/


	return 0;
}
