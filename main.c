///////////// HW2 - OS /////////////
///// Gil ben hamo - 315744557 /////
///// Itzhak rahamim - 312202351 ///
////////////////////////////////////

#include "funcs.c"

int main (int argc, char **argv) 
{ 

    int childPid; char * cmdLine;
    parseInfo *info; 
    printf("\033[0;36m################### Running custom shell ###################\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m\n");
    while(1)
    {
        printCurrentPath();
        cmdLine= readline(" >> "); 
        add_history(cmdLine);

        info = parse(cmdLine);  // Parse information from user

        if(!isPipe(info) && ifChangeDir(info))   // Change folder if its not pipe command
            continue;

        childPid = fork();   

        if (childPid == 0)
            {
                /* child code */
                executeCommand(info); //calls  execvp  
            }
        else 
            { 
                /* parent code */
                waitpid(childPid);         
            }	 
    } 
}
