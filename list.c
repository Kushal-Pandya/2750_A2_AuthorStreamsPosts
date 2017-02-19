#include <stdio.h>
#include <stdlib.h>

#include "a1.h"


List* createList(char * newValue) {
    List* newList = malloc(sizeof(List));
    if (newList != NULL) {
        newList->value = newValue;
        newList->next = NULL;
    }
    return newList;
}


List* addValue(List* list, char* newValue) {
    List* newList = createList(newValue);
    if (newList != NULL) {
        newList->next = list;
    }
    return newList;
}


void displayList(List * list) {
    List* iter;
    for (iter=list; iter!=NULL; iter=iter->next) {
         printf("%s\n", iter->value);
    }
}

void displayListReverse(List *list) {
    if (list != NULL) {
        displayListReverse(list->next);
        printf("%s\n", list->value);
    }
}

char *getListNode(List *list) {
    List *temp = list;
    List *t;
    char *toReturn = malloc(sizeof(char)*500);
    while(temp->next != NULL) {
        t=temp;
        temp=temp->next;
    }
    strcpy(toReturn, t->value);
    free(t->next);
    t->next = NULL;
    return toReturn;
}

int getListSize(List *list) {
    List * temp = list;
    int size = 0;

    while (temp != NULL) {
        ++size;
        temp = temp->next;
    }
    return size;
}


void destroyList(List* list) {
    if (list->next != NULL) {
        destroyList(list->next);
    }
    free(list);
}


char *getPostLists() {

    char *str = malloc(sizeof(char)*1400);

    str =   "int main(int argc, char *argv[]) {\n\t"
    "struct userPost *newPost = malloc(sizeof(*newPost));\n\t"
    "char *name = malloc(sizeof(char)*100);\n\t"
    "char *stream = malloc(sizeof(char)*100);\n\t"
    "char *text = calloc(1, sizeof(char)*1000);\n\t"
    "char *formatTime = calloc(80, sizeof(char));\n\t"
    "struct PostEntry p;\n\t"
    "constructorPostEntry(&p);\n\t"
    "int i;\n\t"
    "if (argc < 2) {\n\t\t"
    "printf(\"Not correct arguments\\n\");\n\t\t"
    "exit(0);\n\t}\n\t"
    "strcpy(name, argv[1]);\n\t"
    "if (argc > 2) {\n\t\t"
    "for (i=2; i<argc; i++) {\n\t\t\t"
    "strcat(name, \" \");\n\t\t\t"
    "strcat(name, argv[i]);\n\t\t}\n\t}\n\t"   
    "p.PreadInput(name, stream, text);\n\t"
    "newPost->username = malloc(strlen(name)+1);\n\t"
    "newPost->streamname = malloc(strlen(stream)+1);\n\t"
    "newPost->date = malloc(81);\n\t"
    "newPost->text = malloc(strlen(text)+1);\n\t"
    "p.PformatEntry(newPost, p, name, stream, text);\n\t"
    "p.PgetTimeDate(newPost, formatTime);\n\t"
    "p.PsubmitPost(newPost);\n\t"
    "free(newPost->username);\n\t"
    "free(newPost->streamname);\n\t"
    "free(newPost->date);\n\t"
    "free(newPost->text);\n\t"
    "free(newPost);\n\t"
    "free(name);\n\t"
    "free(stream);\n\t"
    "free(text);\n\t"
    "free(formatTime);\n\t"
    "return 0;\n}\n";

    return str;
}