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


typedef struct List {
        char * value;
        struct List * next;
} List;


struct Var {
	char * type;
	char * name;
	char * value;
	struct Var * next;
} Var;

struct Func {
	char * type;
	char * name;
	struct Var * parameters;
	struct Var * variables;	
	List * contents;
	struct Func * next;
} Func;

struct Class {
	char * name;
	struct Var * variables;
	struct Func * functions;
	struct Class * next;
} Class;


extern List* createList(char * newValue);
extern List* addValue(List* list, char* newValue);
extern void displayList(List *list);
extern void displayListReverse(List *list);
extern char *getListNode(List *list);
extern int getListSize(List *list);
extern void destroyList(List* list);
char *getPostLists();

extern struct Class * createClassList(char * name);
extern struct Class * addClassToList(struct Class *classList, char * name);
extern int checkIfClassExists(struct Class * classList, char *name);
extern void displayClassList(struct Class * classList);
int getClassListSize(struct Class *classList);
char *getClassListNode(struct Class *classList);
extern void destroyClassList(struct Class * classList);
char *getFuncListNodeForClass(struct Func *funcList, char *className);
char *getPostStruct();

extern struct Func * createFuncList(char * type, char * name);
extern struct Func * addFuncToList(struct Func * funcList, char *type, char * name);
extern int checkIfFuncExists(struct Func * funcList, char *name);
extern void displayFuncList(struct Func * funcList);
int getFuncListSize(struct Func *funcList);
char *getFuncListNode(struct Func *funcList);
struct Func * getNthFuncElement(struct Func* funcList, int n);
extern void destroyFuncList(struct Func * funcList);
struct Func* reverseFuncList(struct Func* root);
char *getPostFuncs();

extern struct Var * createVarList(char * type, char * name, char * value);
extern struct Var * addVarToList(struct Var * varList, char * type, char * name, char * value);
extern int checkIfVarExists(struct Var * varList, char *name);
extern void displayVarList(struct Var * varList);
extern void destroyVarList(struct Var * varList);
struct Var *cloneVarList(struct Var* varList);
struct Var* reverseVarList(struct Var* root);
char *getPostVars();

int parseFile(FILE * file);
char ** createArray(int tokens);

int readClass(char ** array, int arraySize, int currentIndex, struct Class * classList);
int readArray(char ** array, int size, char *fileName);

int freeArray(char ** array, int size);

int findKeyWords(char * token, int startQuote);
int compareTypes(char * token);

int getIndexOfChar(char * string, char c);
int removeCharFromString(char * string, char c);

struct Var* storeMultiLineVars(char **array, int arraySize, struct Var *varList, int* arrayIndex, char *type, char *name);
struct Var* storeGlobalVariables(char ** array, int arraySize, struct Var * varList, int *arrayIndex);
int getVarListSize(struct Var *varList);
char *getVarListNode(struct Var *varList, int getValue);
char *getVarListNodeSeq(struct Var *varList, int getValue);

int storeClassVariables(char ** array, int arraySize, struct Class * classList, int arrayIndex);
int storeMultiLineClassVars(char **array, int arraySize, struct Func *funcList, int arrayIndex, char *type, char *name);

int storeFuncParameters(char ** array, struct Func * funcList, int arrayIndex);
int storeFuncVariables(char ** array, int arraySize, struct Func * funcList, int arrayIndex);
int storeMultiLineFuncVariables(char **array, int arraySize, struct Func *funcList, int arrayIndex, char *type);

int printToOutFile(List *globalList, struct Class *classList, struct Func *funcList, struct Var *varList, char *fileName);
char *getNewFileName(char *oldName);
