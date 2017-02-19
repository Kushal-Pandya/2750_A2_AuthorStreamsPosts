#include "a1.h"


/* Linked List Library for classes*/

struct Class * createClassList(char * name) {
    struct Class* newList = malloc(sizeof(struct Class));
    if (newList != NULL) {
            newList->name = name;
            newList->variables = createVarList("", "", "");
            newList->functions = createFuncList("", "");
            newList->next = NULL;
    }
    return newList;
}

struct Class * addClassToList(struct Class *classList, char * name) {
    struct Class* newList = createClassList(name);
    if (newList != NULL) {
            newList->next = classList;
    }
    return newList;
}

int checkIfClassExists(struct Class * classList, char *name) {
	struct Class * iter;
	for (iter=classList; iter!=NULL; iter=iter->next) {
		if (strcmp(iter->name, name) == 0) return 1;
	}
	return 0;
}

void displayClassList(struct Class * classList) {
	struct Class * iter;
	for (iter=classList; iter!=NULL; iter=iter->next) {
        printf("Class Name:%s \n", iter->name);

        printf("ClassVariables:\n");
        displayVarList(iter->variables);
        printf("ClassFunctions:\n");
        displayFuncList(iter->functions);   
    }
}

int getClassListSize(struct Class *classList) {
    struct Class *temp = classList;
    int size = 0;

    while (temp != NULL) {
        ++size;
        temp = temp->next;
    }
    return size;
}

void destroyClassList(struct Class * classList) {
    if (classList->next != NULL) {
            destroyClassList(classList->next);
    }
    free(classList);
}

char *getClassListNode(struct Class *classList) {
    struct Class *temp = classList;
    struct Class *t;

    char *toReturn = calloc(1000, sizeof(char));
    char *tempStr = calloc(20, sizeof(char));
    char *tempStr3 = calloc(40, sizeof(char));
    char *tempStr4 = calloc(40, sizeof(char));

    while(temp->next != NULL) {
        t=temp;
        temp=temp->next;
    }

    strcpy(toReturn, "struct ");
    strcat(toReturn, t->name);
    strcat(toReturn, " {\n");

    int i;
    for (i=getVarListSize(t->variables)-1; i>0; i--) { /*Appending variables to function*/

        char *checkIfClass = calloc(10, sizeof(char));
        char *copyOfTemp = calloc(10, sizeof(char));

        tempStr = getVarListNode(t->variables, 0);
        strcat(tempStr, "\n");
        strcpy(copyOfTemp, tempStr);

        checkIfClass = strtok(copyOfTemp, " ");
        if (strcmp(checkIfClass, "struct") == 0) { /*Adding constructor to class variable declarations*/
            char *name = calloc(10, sizeof(char));
            char *value = calloc(15, sizeof(char));
            char *constructorName = calloc(50, sizeof(char));

            name = strtok(NULL, " ");
            value = strtok(NULL, " ");
            if (strchr(value, ';') != NULL) {
                strcpy(value, strtok(value, ";"));
            }

            strcat(toReturn, tempStr);
            strcpy(constructorName, "constructor");
            strcat(constructorName, name);
            strcat(constructorName, "(&");
            strcat(constructorName, value);
            strcat(constructorName, ");\n");

            strcat(toReturn, constructorName);
        }
        else 
            strcat(toReturn, tempStr);
    }
    
    struct Func *funcListHead = t->functions;
    t->functions = reverseFuncList(t->functions);
    t->functions = t->functions->next;
    printf("BEFOER%d\n", getFuncListSize(t->functions));
    for (i=getFuncListSize(t->functions); i>0; i--) {
        
        char *theFunc = calloc(1000, sizeof(char));
        theFunc = getFuncListNodeForClass(t->functions, t->name);

        strcat(toReturn, theFunc);
        t->functions = t->functions->next;
    }
    strcat(toReturn, "};\n");

    t->functions = funcListHead;
    t->functions = reverseFuncList(t->functions);
    printf("PRINT%d\n", getFuncListSize(t->functions));
    for (i=getFuncListSize(t->functions); i>0; i--) {

        strcat(toReturn, t->functions->type); 
        strcat(toReturn, " ");    
        strcat(toReturn, t->name);
        strcat(toReturn, t->functions->name);
        strcat(toReturn, "(");   
        
        if (getVarListSize(t->functions->parameters) == 0) {
            strcat(toReturn, ")");
        }
        else {
            t->functions->parameters = reverseVarList(t->functions->parameters);
            t->functions->parameters = t->functions->parameters->next;
            for (i=getVarListSize(t->functions->parameters); i>0; i--) { 
                char * str = calloc(20, sizeof(char));
                char * str2 = calloc(20, sizeof(char));

                tempStr = getVarListNodeSeq(t->functions->parameters, 1);
                printf("TEMPSTR%s\n", tempStr);

                strcpy(tempStr3, tempStr);
                strcpy(tempStr4, tempStr);
                strcpy(str, strtok(tempStr3, ":"));
                strcat(toReturn, strtok(str, " ")); 
                strcat(toReturn, " "); 

                strcat(toReturn, strtok(NULL, ":")); 
                
                strcpy(str2, strtok(tempStr4, ":"));
                strcat(toReturn, strtok(NULL, ";")); 
                strcat(toReturn, "("); 

                t->functions->parameters = t->functions->parameters->next;
            }
        }
        t->functions = t->functions->next;        
    }

    strcat(toReturn, "{\n}");



   /* while (getVarListSize(funcToWrite) > 1) {

        char * str = calloc(20, sizeof(char));
        char * str2 = calloc(20, sizeof(char));
        tempStr = getVarListNode(funcToWrite, 1);

        printf("TEMPSTR%s\n", tempStr);

        strcpy(tempStr3, tempStr);
        strcpy(tempStr4, tempStr);
        strcpy(str, strtok(tempStr3, ":"));
        strcat(toReturn, strtok(str, " ")); 
        strcat(toReturn, " "); 

        strcat(toReturn, strtok(NULL, ":")); 
        
        strcpy(str2, strtok(tempStr4, ":"));
        strcat(toReturn, strtok(NULL, ";")); 
        strcat(toReturn, "("); 
    }

    strcat(toReturn, ") {\n");*/

    free(t->next);
    t->next = NULL;
    return toReturn;
}


char *getPostStruct() {

    char *str = malloc(sizeof(char)*500);

    str = "/*"
    "CIS 2750\n"
    "A2\n"
    "Author: Kushal Pandya\n"
    "Due Date: February 17 2017\n"
    "POST PROGRAM\n"
    "*/\n"
    "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "#include <string.h>\n"
    "#include <unistd.h>\n"
    "#include <time.h>\n"
    "#include \"stream.h\"\n\n"
    "struct PostEntry {\n\t"
    "void (*PreadInput)();\n\t"
    "void (*PformatEntry)();\n\t"
    "void (*PgetTimeDate)();\n\t"
    "void (*PsubmitPost)();\n"
    "};\n";

    return str;
}

int storeClassVariables(char ** array, int arraySize, struct Class * classList, int arrayIndex) {

    int j;
    int i = arrayIndex;

    char * type = malloc(sizeof(char)*15);

    strcpy(type, array[i]);
    removeCharFromString(type, '\n');


    if (strchr(array[i+1], ';') != NULL) {
        char * name = calloc(20, sizeof(char));
        strcpy(name, strtok(array[i+1], ";")); 

        classList->variables = addVarToList(classList->variables, type, name, "");
        i = i + 1;
        return i;
    }
    else if (strchr(array[i+1], ',') != NULL || strcmp(array[i+2], ",\n") == 0) {

        for (j=i+1; j<arraySize; j++) {

            char * name = calloc(20, sizeof(char));
            if (strchr(array[j], ',') != NULL) {
                strcpy(name, strtok(array[j], ","));
                classList->variables = addVarToList(classList->variables, type, name, ""); 
            }
            else if (strcmp(array[j+1], ",\n") == 0) {
                strcpy(name, array[j]);
                removeCharFromString(name, '\n'); 
                classList->variables = addVarToList(classList->variables, type, name, "");
                j = j + 1;
            }
            else if (strchr(array[j], ';') != NULL) {
                strcpy(name, strtok(array[j], ";")); 
                classList->variables = addVarToList(classList->variables, type, name, "");
                return j;
            }
            else if (strcmp(array[j+1], ";\n") == 0) {
                strcpy(name, array[j]);
                removeCharFromString(name, '\n');
                classList->variables = addVarToList(classList->variables, type, name, "");
                j = j + 1;
                return j;
            }
        }
    }
    else if (strcmp(array[i+2], ";\n") == 0) {
        char * name = calloc(20, sizeof(char));
        strcpy(name, array[i+1]);
        removeCharFromString(name, '\n');

        classList->variables = addVarToList(classList->variables, type, name, "");
        i = i + 2;
        return i;
    }
    else if (compareTypes(array[i])) {
        if (strchr(array[i+2], ';') != NULL) {
            char * name = calloc(20, sizeof(char));
            char * value = calloc(20, sizeof(char));

            strcpy(name, array[i+1]);
            removeCharFromString(name, '\n');
            strcpy(value, strtok(array[i+2], ";")); 
            classList->variables = addVarToList(classList->variables, type, name, value);
            i = i + 2;
            return i;
        }
    }

    return i;
}

char *getFuncListNodeForClass(struct Func *funcList, char *className) {
    struct Func *t = funcList;
    
    char *toReturn = malloc(sizeof(char)*1000);
    char *tempStr = calloc(20, sizeof(char));
    char *tempStr2 = calloc(20, sizeof(char));

    strcpy(toReturn, t->type); /*Appending type and name to function*/
    strcat(toReturn, " (*");    
    strcat(toReturn, className);
    strcat(toReturn, t->name); 

    t->parameters = reverseVarList(t->parameters);
    t->parameters = t->parameters->next;
    int i;

    if (getVarListSize(t->parameters) != 0) {
        for (i=getVarListSize(t->parameters); i>0; i--) { 
            tempStr = getVarListNodeSeq(t->parameters, 0);            
            char param = tempStr[0];

            sprintf(tempStr2, "%s%c", toReturn, param);
            strcpy(toReturn, tempStr2);

            t->parameters = t->parameters->next;
        }
    }

    strcat(toReturn, ")();\n"); 


/*    printf("->%s\n\n", toReturn);
*/


/*    strcat(toReturn, newVars->value);
    strcat(toReturn, ",");
*/


/*
    for (i=getVarListSize(funcParams->parameters)-1; i>0; i--) { Appending parameters to function
        printf("TESTSTS%d\n", funcParams->parameters);
        tempStr = getVarListNode(funcParams->parameters);
        if (strchr(tempStr, ';') != NULL) {
            strcat(toReturn, strtok(tempStr, ";"));
        }
        else {
            strcat(toReturn, tempStr);   
        }

        if (getVarListSize(funcParams->parameters) > 1) 
            strcat(toReturn, ",");
    }
*/


/*

    strcat(toReturn, ")");
    strcat(toReturn, " {\n");

    for (i=getVarListSize(t->variables)-1; i>0; i--) { Appending variables to function

        char *checkIfClass = calloc(10, sizeof(char));
        char *copyOfTemp = calloc(10, sizeof(char));
        
        tempStr = getVarListNode(t->variables);
        strcat(tempStr, "\n");
        strcpy(copyOfTemp, tempStr);

        checkIfClass = strtok(copyOfTemp, " ");
        if (strcmp(checkIfClass, "struct") == 0) { Adding constructor to class variable declarations
            char *name = calloc(10, sizeof(char));
            char *value = calloc(15, sizeof(char));
            char *constructorName = calloc(50, sizeof(char));

            name = strtok(NULL, " ");
            value = strtok(NULL, " ");
            if (strchr(value, ';') != NULL) {
                strcpy(value, strtok(value, ";"));
            }

            strcat(toReturn, tempStr);
            strcpy(constructorName, "constructor");
            strcat(constructorName, name);
            strcat(constructorName, "(&");
            strcat(constructorName, value);
            strcat(constructorName, ");\n");

            strcat(toReturn, constructorName);
        }
        else 
            strcat(toReturn, tempStr);
    }

    for (i=getListSize(t->contents)-1; i>0; i--) { Appending contents to function
        tempStr = getListNode(t->contents);

        if (strchr(tempStr, ';')) {
            strcat(tempStr, "\n");
        }
        strcat(toReturn, tempStr);
    }
    strcat(toReturn, "}\n");*/
    
    return toReturn;
}
