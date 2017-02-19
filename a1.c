/*
	CIS 2750
	A1
	Author: Kushal Pandya
	Due Date: January 27 2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "a1.h"

#define TOKEN_LIMIT 100


int main(int argc, char * argv[]) {

	char * fileName;
	int tokens;

	if (argc != 2) {
		printf("Not Enough Command Line arguments\n\n");
		fileName = "infile.cpp";
		/* exit(0)*/
	}
	else fileName = argv[1];

	/* Check if file exists */
	if (access(fileName, F_OK) == -1) {
		printf("File doesn't exist or error opening\n");
		exit(0);
	}

	FILE *file = fopen(fileName, "r");
	tokens = parseFile(file);

	char ** array = createArray(tokens);
	readArray(array, tokens, fileName);

	freeArray(array, tokens); 

	fclose(file);

	return 0;
}



int parseFile(FILE * file) {

	int c;
	int tokens;

	/* Helper file to store tokens for easier reading */
	FILE * assetsFile = fopen("assets.txt", "w");

	/* Booleans*/
	int lastCharSpace = 0; /* Used to check if last char was a white space*/
	int lastCharNewLine = 0; /* print newline if last char was newline*/
	int startQuote = 0; /* Used to indicate start of */

	int possEscapeChar = 0; /*Possibility of escape character in string*/
	int escapeChar = 0; /* Indicates if last char was escape character*/

	int possComment = 0; /* Possible start of a comment*/
	int endComment = 0; /* possible end of comment*/
	int newComment = 0; /* Used to indicate if start of comment*/
	int newMultiLineComment = 0;

	/* Counters*/
	int tokenCounter = 0;

	while ((c = fgetc(file)) != EOF) {

		if (startQuote == 1) {
			lastCharSpace = 0;
			lastCharNewLine = 0;
			fprintf(assetsFile, "%c", c);

			if (c == '\n') {        /* Need to take into account that strings may be more than 1 line?*/
				lastCharNewLine = 1;
				tokenCounter++;
			} 

			if (c == '\\') possEscapeChar = 1;
			else if (c == '"'){
				if (possEscapeChar == 1) escapeChar = 1;
				else if (escapeChar == 0) startQuote = 0;
			} 
			else { 
				possEscapeChar = 0;
				escapeChar = 0;
			}
		} 
		else if (newComment == 1 || newMultiLineComment == 1) {
			lastCharSpace = 0;
			lastCharNewLine = 0;
			fprintf(assetsFile, "%c", c);

			if (c == '\n') { 
				newComment = 0;
				tokenCounter++;
				if (newMultiLineComment == 0) lastCharNewLine = 1;
			}
			else if (c == '*' && newMultiLineComment == 1) endComment = 1;
			else if (c == '/' && endComment == 1 && newMultiLineComment == 1) {
				newMultiLineComment = 0;	
			} 
		}
		else {
			switch(c) {
			case '"':
				startQuote = 1;
				lastCharSpace = 0;
				lastCharNewLine = 0;
				possComment = 0;
				fprintf(assetsFile, "%c", c);
				break;
			case '#':
				lastCharNewLine = 0;
				lastCharSpace = 0;
				possComment = 0;
				fprintf(assetsFile, "%c", c);
				break;
			case '\n':
				if (lastCharNewLine == 1) {
					fprintf(assetsFile, "\n");
					tokenCounter++;
				}
				lastCharNewLine = 1;
				newComment = 0; /* a single line comment would end once new line is found*/
			case ' ':
			case '\t':
			case '\r':
				if (lastCharSpace == 0) {
					fprintf(assetsFile, "\n");
					lastCharSpace = 1;
					tokenCounter++;   
				}
				possComment = 0;	
				break;
			case ',':
			case ';':
			case '(':
			case ')':
			case '{':
			case '}':
			case '=':
				fprintf(assetsFile, "%c\n", c);
				lastCharSpace = 1;
				lastCharNewLine = 0;
				possComment = 0;
				tokenCounter++;
				break;
			case '/':
				fprintf(assetsFile, "%c", c);
				if (possComment == 1) newComment = 1; /* if possible comment, it is one now*/
				else possComment = 1; /* else it may just be a possible comment*/
				lastCharSpace = 0;
				lastCharNewLine = 0;
				break;
			case '*':
				fprintf(assetsFile, "%c", c);
				if (possComment == 1) newMultiLineComment = 1; /* if there is a possible comment, there is one now*/
				else possComment = 1;
				lastCharSpace = 0;
				lastCharNewLine = 0;
				break;
			default:
				fprintf(assetsFile, "%c", c);
				lastCharSpace = 0;
				lastCharNewLine = 0;
				break;
			}
		}	
	}

	fclose(assetsFile);

	tokens = tokenCounter;
	return tokens;
}


/* Files below should be in list interface file*/

char ** createArray(int tokens) {

	int i;
	char *token = calloc(TOKEN_LIMIT, sizeof(char));
	FILE * assetsFile = fopen("assets.txt", "r");
	
	char **tokenArray = malloc(sizeof(char*)*tokens);

	while(fgets(token, TOKEN_LIMIT, assetsFile) != NULL) {
		tokenArray[i] = malloc(sizeof(char)*(strlen(token)+1));
		strcpy(tokenArray[i], token);
		i++;
	}

	free(token);
	fclose(assetsFile);
	return tokenArray;
}

int readClass(char ** array, int arraySize, int currentIndex, struct Class * classList) {

	int i; 
	int codeBlocks = 0;
	int noMoreVars = 0;

	/*if (strchr(array[currentIndex], '{') != NULL) { 
		return currentIndex;
	}
*/
	for (i=currentIndex+1; i<arraySize; i++) {
		
		char * name = malloc(sizeof(char)*20);
		char * type = malloc(sizeof(char)*15);

		if (strchr(array[i], '(') != NULL && codeBlocks == 0) { /*functions*/
			if (strcmp(array[i], "(\n") == 0) {
				strcpy(name, array[i-1]);
				removeCharFromString(name, '\n');

				if (compareTypes(array[i-2])) {
					strcpy(type, array[i-2]);
					removeCharFromString(type, '\n');
				}
				else 
					strcpy(type, "");
			}
			else {
				strcpy(name, strtok(array[i], "(")); 
				strcpy(type, array[i-1]);
				removeCharFromString(type, '\n');
			}
			noMoreVars = 1;
			classList->functions = addFuncToList(classList->functions, type, name);

			if (strcmp(array[i+1], ")\n") != 0) { /*storing parameters*/
				int temp = storeFuncParameters(array, classList->functions, i+1);
				i = temp;
			}

		}
		else if (compareTypes(array[i]) && codeBlocks == 0 && noMoreVars == 0) { /*local class vars*/
			if (strchr(array[i+1], '(') == NULL) { /*if ( not in array at i+1*/
				int temp = storeClassVariables(array, arraySize, classList, i);
				i = temp;
			}
		}


		if (strcmp(array[i], "{\n") == 0) {
			noMoreVars = 1;
			codeBlocks++;
			int temp = storeFuncVariables(array, arraySize, classList->functions, i+1);
			i = temp;
		}
		else if (strcmp(array[i], "}\n") == 0) {
			codeBlocks--;
			if (codeBlocks < 0) {
				free(name);
				free(type);
				return i;
			}
		}
	}
	return currentIndex;
/*	displayVarList(classList->variables);
	displayFuncList(classList->functions);
	displayVarList(classList->functions->parameters);
	displayVarList(classList->functions->variables);*/
}


int readArray(char ** array, int size, char *fileName) {

	int codeBlocks = 0; /* how many nested codeBlocks*/
	int noMoreVars = 0;
	int inFunc = 0;

	struct Class * classList = createClassList(""); /*Create empty list of class structs*/
	struct Func * funcList = createFuncList("", ""); /*Create empty list of func structs*/
	struct Var * varList = createVarList("", "", ""); /*Create empty list of var structs*/

	struct List * globalList = createList("");

	if (strcmp(fileName, "post.cc") == 0) {
		fileName = getNewFileName(fileName);
		FILE *postFile = fopen(fileName, "w");
		fprintf(postFile, "%s", getPostStruct());
		fprintf(postFile, "%s", getPostFuncs());
		fprintf(postFile, "%s", getPostVars());
		fprintf(postFile, "%s", getPostLists());
		fclose(postFile);
		exit(0);
	}

	int i;
	for (i=0; i<size; i++) {

		char * className = calloc(10, sizeof(char));
		char * temp = calloc(20, sizeof(char));
		char * name = calloc(20, sizeof(char));
        char * type = malloc(sizeof(char)*15);
        char * value = malloc(sizeof(char)*40);

		if (array[i][0] == '#' || array[i][0] == '/') {
			strcpy(temp, array[i]);
			removeCharFromString(temp, '\n');
			globalList = addValue(globalList, temp);
		}

		if (strcmp(array[i], "class\n") == 0) {
			if (strchr(array[i+1], '{') != NULL) {
				strcpy(className, strtok(array[i+1], "{"));

				strcpy(temp, "class ");
				strcat(temp, className);
				globalList = addValue(globalList, temp);
				if (checkIfClassExists(classList, className) == 0) {
					codeBlocks++;
					classList = addClassToList(classList, className);
					int temp = readClass(array, size, i+1, classList);
					i = temp;
				}
				noMoreVars = 1;
			}
			else if (strcmp(array[i+2], "{\n") == 0) {
				strcpy(className, array[i+1]);
                removeCharFromString(className, '\n');

                strcpy(temp, "class ");
				strcat(temp, className);
				globalList = addValue(globalList, temp);
				if (checkIfClassExists(classList, className) == 0) {
					codeBlocks++;
					classList = addClassToList(classList, className);
					int temp = readClass(array, size, i+2, classList);
					i = temp;
				}
				noMoreVars = 1;
			} /*gotta be a class var*/
			else if (noMoreVars == 0) {
				strcpy(type, array[i]);
            	removeCharFromString(type, '\n');
            	strcpy(name, array[i+1]);
                removeCharFromString(name, '\n');

            	if (strchr(array[i+2], ';') != NULL) {
                    strcpy(value, strtok(array[i+2], ";"));
                    varList = addVarToList(varList, type, name, value);
                    i = i + 2;
                }
                else if (strcmp(array[i+3], ";\n") == 0) { 
                    strcpy(value, array[i+2]);
                    removeCharFromString(value, '\n');
                    varList = addVarToList(varList, type, name, value);
                    i = i + 3;
                }
                else if (strchr(array[i+2], ',') != NULL || strcmp(array[i+3], ",\n") == 0) {
                    varList = storeMultiLineVars(array, size, varList, &i, type, name);
                }
			}
		}
		else if (strchr(array[i], '(') != NULL && codeBlocks == 0) { /*functions*/
			if (strcmp(array[i], "(\n") == 0) {
				strcpy(name, array[i-1]);
				removeCharFromString(name, '\n');

				if (compareTypes(array[i-2])) {
					strcpy(type, array[i-2]);
					removeCharFromString(type, '\n');
				}
				else 
					strcpy(type, "");
			}
			else {
				strcpy(name, strtok(array[i], "(")); 
				strcpy(type, array[i-1]);
				removeCharFromString(type, '\n');
			}
			noMoreVars = 1;
			funcList = addFuncToList(funcList, type, name);
			
			strcpy(temp, "fn ");
			strcat(temp, name);
			globalList = addValue(globalList, temp);

			if (strcmp(array[i+1], ")\n") != 0) { /*if )\n not equal to array i+1,  storing parameters*/
				inFunc = 1;
				int temp = storeFuncParameters(array, funcList, i+1);
				i = temp;
			}

		}
		else if (compareTypes(array[i]) && codeBlocks == 0 && noMoreVars == 0) { /*local class vars*/
			if (strchr(array[i+1], '(') == NULL) { /*if ( not in array at i+1*/
				varList = storeGlobalVariables(array, size, varList, &i);
			}
		}	
	
		if (strcmp(array[i], "{\n") == 0 && inFunc == 1 && codeBlocks == 0) {
			codeBlocks++;
			noMoreVars = 1;
			int temp = storeFuncVariables(array, size, funcList, i+1);
			i = temp;
		}
		else if (strcmp(array[i], "}\n") == 0) {
			codeBlocks--;
			inFunc = 0;
		}
	}
	
	
/*	displayClassList(classList);
	printf("---------------------------------\n");
	displayList(globalList);
	printf("---------------------------------\n");
	displayVarList(varList);
	printf("---------------------------------\n");
	displayFuncList(funcList);*/

	printToOutFile(globalList, classList, funcList, varList, fileName);

	return 0;
}


int printToOutFile(List *globalList, struct Class *classList, struct Func *funcList, struct Var *varList, char *fileName) {

	fileName = getNewFileName(fileName);
	FILE * outFile = fopen(fileName, "w");
	char *item = malloc(sizeof(char)*TOKEN_LIMIT);
	
	int macrosDone = 0;

	displayListReverse(globalList);
	int size = getListSize(globalList);

	int i;
	for (i=size-1; i>0; i--) {
		item = getListNode(globalList);

		if (item[0] == '#') {
			fprintf(outFile, "%s\n", item);
		}
		else if (item[0] == '/') {
			fprintf(outFile, "\n%s\n", item);
			macrosDone = 1;
		}
		else if (macrosDone == 1) {
			int j;
			fprintf(outFile, "\n");
			for (j=getVarListSize(varList)-1; j>0; j--) {
				fprintf(outFile, "%s\n", getVarListNode(varList, 0));
			}
		}

		
		if(strstr(item, "class ")) {
			int j;
			for (j=getClassListSize(classList)-1; j>0; j--) {
				fprintf(outFile, "%s\n", getClassListNode(classList));
			}
		}
		else if (strstr(item, "fn ")) {
			int j;
			fprintf(outFile, "\n");
			for (j=getFuncListSize(funcList)-1; j>0; j--) {
				fprintf(outFile, "%s\n", getFuncListNode(funcList));
			}
		}
	}
	
/*	printf("1%s\n", getListNode(globalList));
	printf("2%s\n", getListNode(globalList));*/

	fclose(outFile);

	free(item);
	free(fileName);

	destroyList(globalList);
	destroyClassList(classList);
	destroyFuncList(funcList);
	destroyVarList(varList);

	return 0;
}

char *getNewFileName(char *oldName) {

	char * newName = malloc(sizeof(char)*20);

	if (strstr(oldName, ".cc") != NULL) {
		strcpy(newName, strtok(oldName, ".cc"));
	}
	else if (strchr(oldName, '.') != NULL) {
		strcpy(newName, strtok(oldName, "."));
	}
	else {
		strcpy(newName, oldName);
	}
	
	strcat(newName, ".c");
	return newName;
}


/*
	Returns:
	0 if only newline is found

	2 if class found
	3 if class name found
	4 if typefound
	5 if var name found
	6 if func found "("
	7 if brace/new code block found "{"
	8 if ending block found "}"
	9 if semicolon found ";"
	10 if something and punc is found "a;" MAYBE
*/
int findKeyWords(char * token, int startQuote) {

	if (startQuote == 0) {
		if (strcmp(token, "class\n") == 0) {
			return 2;
		}
		else if (strcmp(token, "{\n") == 0) {
			return 7;
		}
		else if (strchr(token, ';')) {
			return 9;
		}
		else if (strchr(token, '(')) {
			return 6;
		}
	}

	return 0;
}

int compareTypes(char * type) { 
	if (strcmp(type, "int\n") == 0) return 1;
	else if (strcmp(type, "double\n") == 0) return 1;
	else if (strcmp(type, "float\n") == 0) return 1;
	else if (strcmp(type, "char\n") == 0) return 1;
	else if (strcmp(type, "short\n") == 0) return 1;
	else if (strcmp(type, "void\n") == 0) return 1;
	else if (strcmp(type, "class\n") == 0) return 1;
	else return 0;
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

int freeArray(char ** array, int size) {

	int i;
	for (i=0; i<size; i++) 
		free(array[i]);

	free(array);
	return 0;
}