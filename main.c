
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "commands.h"
#define MAX_INPUT 1024
#define MAX_TOKENS 256

// Personalización del prompt
#define MAIN_COLOR "\033[1;34m"
#define COLOR_RESET "\033[0m"

int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;

    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}


int main(int argc, char *argv[], char *envp[]) {

    //bucle de shell
    bool finished = false;
    char input[MAX_INPUT],inputCpy[MAX_INPUT];
    char* tokens[MAX_TOKENS];
    bool found;
    int numTokens = 0;
    tList L;
    createEmptyList(&L);
    tfList FL;
    FcreateEmptyList(&FL);
    tMList ML;
    McreateEmptyList(&ML);
    tPList PL;
    PcreateEmptyList(&PL);
    tfItemL stdin_item = {0,"stdin",2};
    tfItemL stdout_item = {1,"stdout",2};
    tfItemL stderr_item = {2,"stderr",1};
    FinsertItem(stdin_item,LNULL, &FL);
    FinsertItem(stdout_item,LNULL, &FL);
    FinsertItem(stderr_item,LNULL, &FL);

    // Obtenemos el hostname para el prompt
    char hostname[256];
    gethostname(hostname, 256);

    while (!finished) {
        char pwd[512];
        getcwd(pwd, 512); // Actualizamos el cwd

        // Formateo del prompt
        printf(MAIN_COLOR "%s@%s:%s $" COLOR_RESET " ",
               getenv("USER"), hostname, pwd);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            exit(EXIT_SUCCESS); // Fix EOF...
        }
        strcpy(inputCpy, input);
        numTokens = TrocearCadena(input, tokens);
        found = false;

        if (numTokens==0) {
            continue;
        }

        for (int i = 0; i < num_commands; i++) {
            cmd_data command = commands[i];
            if (strcmp(tokens[0], command.name) == 0) {
                insertItem(inputCpy,LNULL,&L);
                command.handler(tokens,&L,&FL,&ML,&PL,envp);
                found = true;
                break;
            };
        }
        if (!found) {
            insertItem(inputCpy, LNULL, &L);
            cmd_any(tokens, &L, &FL, &ML, &PL,envp);
        }
    }

    return 0;
}
