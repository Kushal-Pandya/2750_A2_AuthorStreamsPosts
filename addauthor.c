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
#include <time.h>


int removeCharFromString(char * string, char c);


int main(int argc, char *argv[]) {

	char *inputBuffer = malloc(sizeof(char)*100);
	char *temp = malloc(sizeof(char)*100);
	char *filename = malloc(sizeof(char)*100);
	char *token;	
	int removal = 0; /*Boolean indicating if author is to be removed, Default is add*/
	int size;

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

			/*Appending file here*/
			strcpy(temp, "messages/");
			strcat(temp, token);
			removeCharFromString(temp, '\n');
			strcpy(filename, strcat(temp, "StreamUsers.txt"));
			FILE *fptr = fopen(filename, "a+");

			/*Perform the addition or removal here*/
			if (removal) {
				printf("Removing %s from %s\n", argv[2], filename);
			}
			else {
				/*Read file to check if user */

				char buffer[255];
				char name[50];
				while (fgets(buffer, 255, fptr) != NULL) {
					strcpy(name, strtok(buffer, " "));
					printf("NAME%s\n", name);
				}

				printf("Adding %s to %s\n", argv[1], filename);
				fprintf(fptr, "%s 0\n", argv[1]);
			}
			token = strtok(NULL, ",");
			fclose(fptr);
		}
	}
	else {
		strcpy(temp, "messages/");
		strcat(temp, inputBuffer);
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
					char *name = malloc(sizeof(char)*50);
					strcpy(temp, buffer);
					strcpy(name, strtok(temp, " "));

					printf("[%s]\n", name);
					if (strcmp(name, argv[2]) != 0) {
						fprintf(outFile, "%s", buffer);
					}
					free(name);
				}
				printf("Removing %s from %s\n", argv[2], filename);
				remove(filename);	
				fclose(fptr);
				fclose(outFile);
				rename("messages/temp.txt", filename); 
			}
		}
		else {
			/*Read file to check if user */

			if (size == 0) {
				fprintf(fptr, "%s 0\n", argv[1]);
			}
			else {
				char buffer[255];	
				int duplicate = 0;			

				while (fgets(buffer, 255, fptr) != NULL) {
					char *name = malloc(sizeof(char)*50);
					strcpy(name, strtok(buffer, " "));

					printf("[%s]\n", name);
					if (strcmp(name, argv[1]) == 0) 
						duplicate = 1;
					free(name);
				}				
				if (duplicate)
					printf("ERROR %s already exists in %s\n", argv[1], filename);
				else
					fprintf(fptr, "%s 0\n", argv[1]);
			}
			fclose(fptr);
		}
	}

	free(inputBuffer);
	free(filename);

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