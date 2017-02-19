/*CIS 2750
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

struct PostEntry {
	void (*PreadInput)();
	void (*PformatEntry)();
	void (*PgetTimeDate)();
	void (*PsubmitPost)();
};
void PgetTimeDate(struct userPost *newPost, char *formatTime) {
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(formatTime, 80, "%x-%I:%M%p", info);
	strcpy(newPost->date, formatTime);
}
void PformatEntry(struct userPost *newPost, char *name, char *stream, char *text) {
	strcpy(newPost->username, name);
	strcpy(newPost->streamname, stream);
	strcpy(newPost->text, text);
}
void PreadInput(char *name, char *stream, char *text) {
	char *textBuffer = malloc(sizeof(char)*100);
	char ifEOF[2];
	ifEOF[1] = '\0'
	;printf("Stream: ");
	fgets(stream, 100, stdin);
	printf("Enter Text: ");
	while ((ifEOF[0] = getchar()) != EOF) {
		printf("- ");
		fgets(textBuffer, 100, stdin);
		strcat(text, ifEOF);
		strcat(text, textBuffer);
	}
	free(textBuffer);
}
void PsubmitPost(struct userPost *st) {
	updateStream(st);
}
void constructorPostEntry(struct PostEntry *self) {
	self->PreadInput = &PreadInput;
	self->PformatEntry = &PformatEntry;
	self->PgetTimeDate = &PgetTimeDate;
	self->PsubmitPost = &PsubmitPost;
}
int main(int argc, char *argv[]) {
	struct userPost *newPost = malloc(sizeof(*newPost));
	char *name = malloc(sizeof(char)*100);
	char *stream = malloc(sizeof(char)*100);
	char *text = calloc(1, sizeof(char)*1000);
	char *formatTime = calloc(80, sizeof(char));
	struct PostEntry p;
	constructorPostEntry(&p);
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
	p.PreadInput(name, stream, text);
	newPost->username = malloc(strlen(name)+1);
	newPost->streamname = malloc(strlen(stream)+1);
	newPost->date = malloc(81);
	newPost->text = malloc(strlen(text)+1);
	p.PformatEntry(newPost, p, name, stream, text);
	p.PgetTimeDate(newPost, formatTime);
	p.PsubmitPost(newPost);
	free(newPost->username);
	free(newPost->streamname);
	free(newPost->date);
	free(newPost->text);
	free(newPost);
	free(name);
	free(stream);
	free(text);
	free(formatTime);
	return 0;
}
