///////////// HW2 - OS /////////////
///// Gil ben hamo - 315744557 /////
///// Itzhak rahamim - 312202351 ///
////////////////////////////////////

#include "headers.h"

int countTokens(char* line) // Count how many Tokens (Seperated strings) in <line>.
{
    int count=0;
    if(line == NULL)
        return 0;
    for(int i=0;line[i]!='\0';i++)
        if(line[i] == ' ')
            count++;
    return ++count;
}

parseInfo* parse(char* line) // Parse information from input
{
    int index = 0, num_of_tokens = countTokens(line);   // Get number of tokens

    parseInfo* p_info = (parseInfo*)malloc(sizeof(parseInfo));
    p_info->tokens = (char**)malloc(sizeof(char*) * (num_of_tokens + 1)); // Allocate memory for array of strings (Tokens)
    
    // Check if there is no tokens
    if(strlen(line)==0) 
    {
        p_info->tokens[0] = strdup("");
        return p_info;
    }

    char* token = strtok(line, " ");

    // "exit" command
    if(strcmp(token,"exit")==0) 
    {
        printf("\033[0;31mExiting custome shell..\033[0m\n");
        if(line)
            free(line);
        exit(1);
    }
    
    // Add tokens to the "tokens" array
    while (token != NULL) {
        p_info->tokens[index++] = strdup(token);
        token = strtok(NULL, " ");
    }

    p_info->size = index; // Setup size
    p_info->tokens[index] = NULL; // Clean last token

    if(line)
        free(line); // Free memory

    return p_info;
}

void executeCommand(parseInfo* p_info)
{
    // Check if the command is legal
    if(!isSupported(p_info->tokens[0]))
    {
        //Release allocated memo
        freeInfo(p_info);
        exit(1);
    } 
    
    // Handale pipe
    checkPipe(p_info);

    // Handale "cat >" command
    if(strcmp(p_info->tokens[0],"cat")==0 && 
        strcmp(p_info->tokens[1],">")==0
        )
            checkCat(p_info);

    
    // If the user enter a blank command, \n
    if(strcmp(p_info->tokens[0],"")==0) 
    {
        //Release allocated memo
        freeInfo(p_info);
        exit(1);
    }

    execvp(p_info->tokens[0],p_info->tokens);
    
    //Release allocated memo
    freeInfo(p_info);
}

int ifChangeDir(parseInfo* info) // Take care of changing current folder
{
    if(strcmp(info->tokens[0],"cd")==0) 
    {
        // Go back to root
        if(info->tokens[1] == NULL || strcmp(info->tokens[1],"~")==0)
            chdir(getenv("HOME"));
        
        // Go to the folder  
        else
            chdir(info->tokens[1]);
        
        freeInfo(info);
        return 1;
    }
    return 0;
}

void printCurrentPath() // Print the path of the current file
{
    char curr_path[MAX_BUFF_LEN];
    getcwd(curr_path, MAX_BUFF_LEN);
    printf("\033[0;35m%s \033[0m: \033[0;36m%s\033[0m:",getenv("USER"),curr_path);
}

int isSupported(char* line) // Check if the command is supported by this shell
{
    if(
            !(
                    strcmp(line, "pwd") == 0 || strcmp(line, "cd") == 0 ||strcmp(line, "nano") == 0 
                ||  strcmp(line, "cat") == 0 || strcmp(line, "wc") == 0 || strcmp(line, "cp") == 0 
                ||  strcmp(line, "clear") == 0 || strcmp(line, "grep") == 0 || strcmp(line, "wget") == 0 
                ||  strcmp(line, "ls") == 0 || strcmp(line, "tree") == 0 
                || strcmp(line, "") == 0 
            )
        )
        {
        printf("%s : \033[0;31mThis command don't supported!\033[0m\n",line);
        return 0;
        }
    return 1;
}

void checkCat(parseInfo* info) // Handale the "cat >" command
{
    // Setup
    FILE *file;
    char temp[MAX_BUFF_LEN];
    // Check if there is a file name
    if(!info->tokens[2]) 
    {
        printf("\033[0;31mThis command requires a file name\033[0m\n");
        freeInfo(info);
        exit(1);
    }
    
    file = fopen(info->tokens[2],"w");

    // Check if the file is open
    if(!file)
    {
        printf("\033[0;31mCould not open the file\033[0m\n");
        freeInfo(info);
        exit(1);
    }    

    // Insert string to the file
    while(fgets(temp, MAX_BUFF_LEN, stdin)) fprintf(file, "%s" ,temp);
    fclose(file);
    
    freeInfo(info);
    exit(1);
}

void checkPipe(parseInfo* info)  // Handale pipe
{
    // Setup
    parseInfo *p1, *p2;
    int fds_arr[2], childPid;

    // Look for '|' character
    for(int i=0 ; i< info->size; i++)
        if(!strcmp(info->tokens[i],"|"))
        {
            // Setup for 2 separated tokens arrays
            p1 = (parseInfo*)malloc(sizeof(parseInfo));
            p2 = (parseInfo*)malloc(sizeof(parseInfo));
            p1->size = i;
            p2->size = info->size - i -1;
            p1->tokens = (char**)malloc(sizeof(char*)* (p1->size + 1));  
            p2->tokens = (char**)malloc(sizeof(char*)* (p2->size + 1));

            // Copy values
            for (int j = 0, idx1 = 0, idx2 = 0 ; j < info->size ; j++) 
            {
                if(j < i) p1->tokens[idx1++] = strdup(info->tokens[j]);
                if(j > i) p2->tokens[idx2++] = strdup(info->tokens[j]);
            }
            
            p1->tokens[p1->size] = NULL;
            p2->tokens[p2->size] = NULL;

            // Create pipe
            pipe(fds_arr);
            childPid = fork();
            if(childPid == 0)
            {
                close(fds_arr[READ]);
                dup2(fds_arr[WRITE], WRITE);
                executeCommand(p1);
            }
            else
            {
                waitpid(childPid);
                close(fds_arr[WRITE]);
                dup2(fds_arr[READ], READ);
                if(!ifChangeDir(p2))
                    executeCommand(p2);
            }

            freeInfo(info);

            exit(1);
        }    
}

void freeInfo(parseInfo* info) // Free the allocated memory
{
    if(info){
        for(int i=0 ; i<info->size ; i++)
            if(info->tokens[i])
                free(info->tokens[i]);
        if(info->tokens)
            free(info->tokens);
        free(info);
    }
}

int isPipe(parseInfo* info)
{
    for(int i=0 ; i< info->size; i++)
        if(strcmp(info->tokens[i],"|"))
            return 1;
    return 0;
}