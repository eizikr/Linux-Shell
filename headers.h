///////////// HW2 - OS /////////////
///// Gil ben hamo - 315744557 /////
///// Itzhak rahamim - 312202351 ///
////////////////////////////////////

#include<stdio.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

#define MAX_BUFF_LEN 2048
#define READ 0
#define WRITE 1


typedef struct parseInfo
{
    char** tokens;
    int size;
    
}parseInfo;

parseInfo* parse(char* line); 

void executeCommand(parseInfo* tokens);

void printCurrentPath();

int countTokens();

int isSupported(char* line);

void checkCat(parseInfo* tokens);

int ifChangeDir(parseInfo* tokens);

void checkPipe(parseInfo* info);

int isPipe(parseInfo* info);

void freeInfo(parseInfo* info);
