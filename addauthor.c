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


void performOperation(char *token, int removal, char *argv[]) {

	char *temp = malloc(sizeof(char)*100);
	char *filename = malloc(sizeof(char)*100);		
	int size;

	strcpy(temp, "messages/");
	strcat(temp, token);
	removeCharFromString(temp, '\n');
	strcpy(filename, strcat(temp, "StreamUsers.txt"));
	FILE *fptr = fopen(filename, "a+");

	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);

	/*Perform the addition or removal here*/
	if (removal) {
		if (size != 0) {
			FILE *outFile = fopen("messages/temp.txt", "w+");
			char buffer[255];	

			while (fgets(buffer, 255, fptr) != NULL) {

				if (strstr(buffer, argv[2]) == NULL)
					fprintf(outFile, "%s", buffer);
			}
			printf("Removing %s from %s\n", argv[2], filename);
			remove(filename);	
			fclose(fptr);
			fclose(outFile);
			rename("messages/temp.txt", filename); 
		}
	}
	else {
		/*Read file to check if user exists*/
		
		if (size == 0) {
			fprintf(fptr, "%s 0\n", argv[1]);
		}
		else {
			char buffer[255];	
			int duplicate = 0;			

			while (fgets(buffer, 255, fptr) != NULL) {
				if (strstr(buffer, argv[1]) != NULL)
					duplicate = 1;
			}				
			if (duplicate)
				printf("ERROR %s already exists in %s\n", argv[1], filename);
			else
				fprintf(fptr, "%s 0\n", argv[1]);
		}
		fclose(fptr);
	}

	free(temp);
	free(filename);
}


int main(int argc, char *argv[]) {

	char *inputBuffer = malloc(sizeof(char)*100);
	char *streamName = malloc(sizeof(char)*100);
	char *token;
	int removal = 0; /*Boolean indicating if author is to be removed, Default is add*/

	if (argc > 3 || argc < 2) {
		printf("Not correct arguments\n");
		exit(0);
	}

	printf("List streams: ");
	fgets(inputBuffer, 100, stdin);

	if (strcmp(argv[1], "-r") == 0) 
		removal = 1; 	/*Assume remove author from lists*/
	/*ELSE Assume add author to lists*/
	

	if (strchr(inputBuffer, ',') != NULL) {
		token = strtok(inputBuffer, ",");
		while(token != NULL) {
			strcpy(streamName, token);			
			performOperation(streamName, removal, argv);
			token = strtok(NULL, ",");
		}
	}
	else {
		performOperation(inputBuffer, removal, argv);
	}

	free(inputBuffer);
	free(streamName);

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