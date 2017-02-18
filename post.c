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
#include <time.h>

#include "stream.h"


char *getTimeDate() {
	time_t rawtime;
   	struct tm *info;
   	char *formatTime = calloc(80, sizeof(char));

   	time(&rawtime);
   	info = localtime(&rawtime);

   	strftime(formatTime, 80, "%x-%I:%M%p", info);  
   	return formatTime;
}


struct userPost *formatEntry(char *name, char *stream, char *text) {

	struct userPost *newPost = malloc(sizeof(*newPost));
	char *temp;

	newPost->username = malloc(strlen(name)+1);
	newPost->streamname = malloc(strlen(stream)+1);
	newPost->date = malloc(81);
	newPost->text = malloc(strlen(text)+1);

	strcpy(newPost->username, name);
	strcpy(newPost->streamname, stream);
	strcpy(newPost->text, text);

	temp = getTimeDate();
	strcpy(newPost->date, temp);

	free(temp);

	return newPost;
}


struct userPost *readInput(char *name) {

	struct userPost *toReturn;
	char *stream = malloc(sizeof(char)*100);
	char *text = calloc(1, sizeof(char)*1000);
	char *textBuffer = malloc(sizeof(char)*100);
	char ifEOF[2];

	ifEOF[1] = '\0';
	printf("Stream: ");
	fgets(stream, 100, stdin);

	printf("Enter Text: ");
	while ((ifEOF[0] = getchar()) != EOF) {
		printf("- ");
		fgets(textBuffer, 100, stdin);
		strcat(text, ifEOF);
		strcat(text, textBuffer);
	}

	toReturn = formatEntry(name, stream, text);

	free(textBuffer);
	free(stream);
	free(text);

	return toReturn;
}

void submitPost(struct userPost *st) {
	updateStream(st);
}


int main(int argc, char *argv[]) {

	struct userPost *newPost;
	char *name = malloc(sizeof(char)*100);
	int i;

	if (argc < 2) {
		printf("Not correct arguments\n");
		exit(0);
	}

	strcpy(name, argv[1]);
	if (argc > 2) {
		for (i=2; i<argc; i++) {
			strcat(name, " ");
			strcat(name, argv[i]);
		}
	}	

	newPost = readInput(name);
	submitPost(newPost);

	free(newPost->username);
	free(newPost->streamname);
	free(newPost->date);
	free(newPost->text);
	free(newPost);
	free(name);

	return 0;
}
