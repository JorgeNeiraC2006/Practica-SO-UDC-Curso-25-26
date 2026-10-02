
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#ifndef P1_P1_H
#define P1_P1_H
#include <stdio.h>
#include "p0.h"
#include "dynamic_list.h"
#include "file_list.h"
#include "mem_list.h"
#include "proc_list.h"

typedef struct {
    int size; // 0 corto 1 largo
    int link; // 0 no linked 1 linked
    int hid; // 0 no ocultos 1 ocultos
    int rec; // 0 no recursivo 1 antes -1 despues
}dirparams;

void cmd_create(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_setdirparams(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_getdirparams(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_dir(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_erase(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_delrec(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_lseek(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_writestr(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);


#endif //P1_P1_H
