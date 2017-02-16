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

	char *inputBuffer = malloc(sizeof(char)*100);

	if (argc > 3 || argc < 2) {
		printf("Not correct arguments\n");
		exit(0);
	}

	printf("List streams: ");
	fgets(inputBuffer, 100, stdin);

	if (strcmp(argv[1], "-r") == 0) 
		removeUser(argv[2], inputBuffer);
	else
		addUser(argv[1], inputBuffer);

	free(inputBuffer);
	return 0;
}

