/*
	CIS 2750
	A2
	Author: Kushal Pandya
	Due Date: February 17 2017

	ADDAUTHOR PROGRAM
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "stream.h"

int main(int argc, char *argv[]) {

	char *name = malloc(sizeof(char)*100);
	char *inputBuffer = malloc(sizeof(char)*100);
	int i;
	int removal = 0;


	if (argc < 2) {
		printf("Not correct arguments\n");
		exit(0);
	}

	if (strcmp(argv[1], "-r") == 0) {
		removal = 1;
		strcpy(name, argv[2]);
		if (argc > 3) {
			for (i=3; i<argc; i++) {
				strcat(name, " ");
				strcat(name, argv[i]);
			}
		}
	}
	else {
		strcpy(name, argv[1]);
		if (argc > 2) {
			for (i=2; i<argc; i++) {
				strcat(name, " ");
				strcat(name, argv[i]);
			}
		}
	}

	printf("List streams: ");
	fgets(inputBuffer, 100, stdin);

	if (removal == 1) 
		removeUser(name, inputBuffer);
	else
		addUser(name, inputBuffer);

	free(inputBuffer);
	free(name);
	return 0;
}

