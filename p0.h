
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#ifndef P0_H
#define P0_H
#include <stdio.h>
#include "dynamic_list.h"
#include "file_list.h"
#include "mem_list.h"
#include "proc_list.h"

void cmd_authors(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_getpid(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_exit(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_date(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_getcwd(char* tokens[], tList* L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_infosys(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_help(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_historic(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_dup(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_chdir(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_open(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_close(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_dup(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

void cmd_list_open(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

#endif //P0_H
