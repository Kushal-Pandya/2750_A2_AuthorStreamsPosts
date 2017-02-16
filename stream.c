
#include "stream.h"

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

void updateStream(struct userPost *st) {

	char *streamFilename = malloc(sizeof(char)*100);
	char *streamDataFilename = malloc(sizeof(char)*100);
	char *streamUsersFilename = malloc(sizeof(char)*100);
	int size;
	int usersSize;
	int hasPermission = 0;

	strcpy(streamFilename, "messages/");
	strcat(streamFilename, st->streamname);
	removeCharFromString(streamFilename, '\n');
	strcat(streamFilename, "Stream");

	strcpy(streamDataFilename, "messages/");
	strcat(streamDataFilename, st->streamname);
	removeCharFromString(streamDataFilename, '\n');
	strcat(streamDataFilename, "StreamData");

	strcpy(streamUsersFilename, "messages/");
	strcat(streamUsersFilename, st->streamname);
	removeCharFromString(streamUsersFilename, '\n');
	strcat(streamUsersFilename, "StreamUsers");

	FILE *usersfile = fopen(streamUsersFilename, "a+");
	fseek(usersfile, 0, SEEK_END);
	usersSize = ftell(usersfile);
	fseek(usersfile, 0, SEEK_SET);

	if (usersSize != 0) {
		char buffer[255];
		char temp[255];
		char *token;
		char *name = calloc(strlen(st->username)+1, sizeof(char));
		strcpy(name, st->username);
		removeCharFromString(name, '\n');	

		while (fgets(buffer, 255, usersfile) != NULL) {
			strcpy(temp, buffer);
			token = strtok(temp, " ");
			if (strcmp(token, name) == 0)
				hasPermission = 1;
		}
		free(name);
	}
	
	if (hasPermission == 1) {
		FILE *streamfile = fopen(streamFilename, "a+");
		fprintf(streamfile, "Sender: %s\n", st->username);
		fprintf(streamfile, "Date: %s\n", st->date);
		fprintf(streamfile, "Text: %s", st->text);

		fseek(streamfile, 0, SEEK_END);
		size = ftell(streamfile);
		fseek(streamfile, 0, SEEK_SET);

		FILE *datafile = fopen(streamDataFilename, "a+");
		fprintf(datafile, "%d\n", size);

		fclose(streamfile);
		fclose(datafile);
	}
	else
		printf("\nAuthor does NOT have permission.. Aborting\n");

	free(streamFilename);
	free(streamDataFilename);
	free(streamUsersFilename);
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