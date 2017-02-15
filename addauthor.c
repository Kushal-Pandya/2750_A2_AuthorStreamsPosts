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


int removeCharFromString(char * string, char c);


void performAdd(char *token, char *name) {

	char *temp = malloc(sizeof(char)*100);
	char *filename = malloc(sizeof(char)*100);		
	int size;

	strcpy(temp, "messages/");
	strcat(temp, token);
	removeCharFromString(temp, '\n');
	strcpy(filename, strcat(temp, "StreamUsers"));
	FILE *fptr = fopen(filename, "a+");

	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	
	/*Read file to check if user exists*/
	
	if (size == 0) {
		fprintf(fptr, "%s 0\n", name);
	}
	else {
		char buffer[255];	
		int duplicate = 0;			

		while (fgets(buffer, 255, fptr) != NULL) {
			if (strstr(buffer, name) != NULL)
				duplicate = 1;
		}				
		if (duplicate)
			printf("ERROR %s already exists in %s\n", name, filename);
		else
			fprintf(fptr, "%s 0\n", name);
	}

	fclose(fptr);
	free(temp);
	free(filename);
}

void performRemove(char *token, char *name) {

	char *temp = malloc(sizeof(char)*100);
	char *filename = malloc(sizeof(char)*100);		
	int size;

	strcpy(temp, "messages/");
	strcat(temp, token);
	removeCharFromString(temp, '\n');
	strcpy(filename, strcat(temp, "StreamUsers"));
	FILE *fptr = fopen(filename, "a+");

	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	if (size != 0) {
		FILE *outFile = fopen("messages/temp.txt", "w+");
		char buffer[255];	

		while (fgets(buffer, 255, fptr) != NULL) {

			if (strstr(buffer, name) == NULL)
				fprintf(outFile, "%s", buffer);
		}
		printf("Removing %s from %s\n", name, filename);
		remove(filename);	
		fclose(fptr);
		fclose(outFile);
		rename("messages/temp.txt", filename); 
	}

	free(temp);
	free(filename);
}


void addUser(char *username, char*list) {

	char *streamName = malloc(sizeof(char)*100);
	char *token;

	if (strchr(list, ',') != NULL) {
		token = strtok(list, ",");
		while(token != NULL) {
			strcpy(streamName, token);			
			performAdd(streamName, username);
			token = strtok(NULL, ",");
		}
	}
	else {
		performAdd(list, username);
	}
	free(streamName);
}

void removeUser(char *username, char *list) {
	
	char *streamName = malloc(sizeof(char)*100);
	char *token;

	if (strchr(list, ',') != NULL) {
		token = strtok(list, ",");
		while(token != NULL) {
			strcpy(streamName, token);			
			performRemove(streamName, username);
			token = strtok(NULL, ",");
		}
	}
	else {
		performRemove(list, username);
	}
	free(streamName);
}


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

int getIndexOfChar(char * string, char c) {
	const char *ptr = strchr(string, c);
	int index;

	if (ptr) 
		return index = ptr - string;
	return -1;
}

int removeCharFromString(char * string, char c) {
	int result = getIndexOfChar(string, c);

	if (result >= 0) {
		memmove(&string[result], &string[result + 1], strlen(string) - result);
		return 1;
	}
	return 0;
}