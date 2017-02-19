#include "a1.h"


/* Linked List Library for Variables*/

struct Var * createVarList(char * type, char * name, char * value) {
	struct Var * newList = malloc(sizeof(struct Var));
	if (newList != NULL) {
			newList->type = type;
            newList->name = name;
            newList->value = value;
            newList->next = NULL;
    }
    return newList;
}

struct Var * addVarToList(struct Var * varList, char * type, char * name, char * value) {
	struct Var* newList = createVarList(type, name, value);
    if (newList != NULL) {
            newList->next = varList;
    }
    return newList;
}

int checkIfVarExists(struct Var * varList, char *name) {
	struct Var * iter;
	for (iter=varList; iter!=NULL; iter=iter->next) {
		if (strcmp(iter->name, name) == 0) return 1;
	}
	return 0;
}

void displayVarList(struct Var * varList) {
	struct Var * iter;
	for (iter=varList; iter!=NULL; iter=iter->next) 
		printf("VarType:%s VarName:%s VarValue:%s\n", iter->type, iter->name, iter->value);
}

int getVarListSize(struct Var *varList) {
    struct Var *temp = varList;
    int size = 0;

    while (temp != NULL) {
        ++size;
        temp = temp->next;
    }
    return size;
}

struct Var* reverseVarList(struct Var* root) {
  struct Var* new_root = 0;
  while (root) {
    struct Var* next = root->next;
    root->next = new_root;
    new_root = root;
    root = next;
  }
  return new_root;
}



char *getVarListNode(struct Var *varList, int getValue) {
    struct Var *temp = varList;
    struct Var *t;
    char *toReturn = malloc(sizeof(char)*200);
    while(temp->next != NULL) {
        t=temp;
        temp=temp->next;
    }

    if (strcmp(t->type, "class") == 0) {
        strcpy(toReturn, "struct");
        strcat(toReturn, " ");
        strcat(toReturn, t->name);
        strcat(toReturn, " ");
        strcat(toReturn, t->value);
        strcat(toReturn, ";");
    }
    else if (getValue == 1) {
        strcpy(toReturn, t->type);
        strcat(toReturn, " ");
        strcat(toReturn, t->name);
        strcat(toReturn, ":");
        strcat(toReturn, t->value);
        strcat(toReturn, ",");
    }
    else {
        strcpy(toReturn, t->type);
        strcat(toReturn, " ");
        strcat(toReturn, t->name);
        strcat(toReturn, ";");
    }
    free(t->next);
    t->next = NULL;
    return toReturn;
}

char *getVarListNodeSeq(struct Var *varList, int getValue) {
    char *toReturn = malloc(sizeof(char)*200);

    if (strcmp(varList->type, "class") == 0) {
        strcpy(toReturn, "struct");
        strcat(toReturn, " ");
        strcat(toReturn, varList->name);
        strcat(toReturn, " ");
        strcat(toReturn, varList->value);
        strcat(toReturn, ";");
    }
    else if (getValue == 1) {
        strcpy(toReturn, varList->type);
        strcat(toReturn, " ");
        strcat(toReturn, varList->name);
        strcat(toReturn, ":");
        strcat(toReturn, varList->value);
        strcat(toReturn, ",");
    }
    else {
        strcpy(toReturn, varList->type);
        strcat(toReturn, " ");
        strcat(toReturn, varList->name);
        strcat(toReturn, ";");
    }
    return toReturn;
}

struct Var *cloneVarList(struct Var* varList) {
    if (varList == NULL) return NULL;

    struct Var* result = createVarList("", "", "");
    result->name = varList->name;
    result->type = varList->type;
    result->value = varList->value;
    result->next = cloneVarList(varList->next);
    return result;
}


void destroyVarList(struct Var * varList) {
    if (varList->next != NULL) {
            destroyVarList(varList->next);
    }
    free(varList);
}


char *getPostVars() {

    char *str = malloc(sizeof(char)*900);

    str = "void PreadInput(char *name, char *stream, char *text) {\n\t"
    "char *textBuffer = malloc(sizeof(char)*100);\n\t"
    "char ifEOF[2];\n\t"
    "ifEOF[1] = '\\0'\n\t;"
    "printf(\"Stream: \");\n\t"
    "fgets(stream, 100, stdin);\n\t"
    "printf(\"Enter Text: \");\n\t"
    "while ((ifEOF[0] = getchar()) != EOF) {\n\t\t"
    "printf(\"- \");\n\t\t"
    "fgets(textBuffer, 100, stdin);\n\t\t"
    "strcat(text, ifEOF);\n\t\t"
    "strcat(text, textBuffer);\n\t}\n\t"
    "free(textBuffer);\n}\n"
    "void PsubmitPost(struct userPost *st) {\n\t"
    "updateStream(st);\n}\n"
    "void constructorPostEntry(struct PostEntry *self) {\n\t"
    "self->PreadInput = &PreadInput;\n\t"
    "self->PformatEntry = &PformatEntry;\n\t"
    "self->PgetTimeDate = &PgetTimeDate;\n\t"
    "self->PsubmitPost = &PsubmitPost;\n}\n";
    
    return str;
}

struct Var * storeMultiLineVars(char **array, int arraySize, struct Var *varList, int *arrayIndex, char *type, char *name) {
    int j;
    int i = *arrayIndex;

    for (j=i+2; j<arraySize; j++) {

        char * value = calloc(20, sizeof(char));
        if (strchr(array[j], ',') != NULL) {
            strcpy(value, strtok(array[j], ","));
            varList = addVarToList(varList, type, name, value); 
        }
        else if (strcmp(array[j+1], ",\n") == 0) {
            strcpy(value, array[j]);
            removeCharFromString(value, '\n'); 
            varList = addVarToList(varList, type, name, value);
            j = j + 1;
        }
        else if (strchr(array[j], ';') != NULL) {
            strcpy(value, strtok(array[j], ";")); 
            varList = addVarToList(varList, type, name, value);
            *arrayIndex = j;
            return varList;
        }
        else if (strcmp(array[j+1], ";\n") == 0) {
            strcpy(value, array[j]);
            removeCharFromString(value, '\n');
            varList = addVarToList(varList, type, name, value);
            j = j + 1;
            *arrayIndex = j;
            return varList;
        }
    }
    return varList; 
}

struct Var* storeGlobalVariables(char ** array, int arraySize, struct Var *varList, int* arrayIndex) {

    int j;
    int i = *arrayIndex;

    char * type = malloc(sizeof(char)*15);

    strcpy(type, array[i]);
    removeCharFromString(type, '\n');

    if (strchr(array[i+1], ';') != NULL) {
        char * name = calloc(20, sizeof(char));
        strcpy(name, strtok(array[i+1], ";")); 

        varList = addVarToList(varList, type, name, "");
        i = i + 1;
        *arrayIndex = i;
        return varList;
    }
    else if (strchr(array[i+1], ',') != NULL || strcmp(array[i+2], ",\n") == 0) {

        for (j=i+1; j<arraySize; j++) {

            char * name = calloc(20, sizeof(char));
            if (strchr(array[j], ',') != NULL) {
                strcpy(name, strtok(array[j], ","));
                varList = addVarToList(varList, type, name, ""); 
            }
            else if (strcmp(array[j+1], ",\n") == 0) {
                strcpy(name, array[j]);
                removeCharFromString(name, '\n'); 
                varList = addVarToList(varList, type, name, "");
                j = j + 1;
            }
            else if (strchr(array[j], ';') != NULL) {
                strcpy(name, strtok(array[j], ";")); 
                varList = addVarToList(varList, type, name, "");
                *arrayIndex = j;
                return varList;
            }
            else if (strcmp(array[j+1], ";\n") == 0) {
                strcpy(name, array[j]);
                removeCharFromString(name, '\n');
                varList = addVarToList(varList, type, name, "");
                j = j + 1;
                *arrayIndex = j;
                return varList;
            }
        }
    }
    else if (strcmp(array[i+2], ";\n") == 0) {
        char * name = calloc(20, sizeof(char));
        strcpy(name, array[i+1]);
        removeCharFromString(name, '\n');

        varList = addVarToList(varList, type, name, "");
        i = i + 2;
        *arrayIndex = i;
        return varList;
    }

    free(type);
    return varList;
}