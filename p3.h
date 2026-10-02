
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#ifndef P3_H
#define P3_H
#include <stdio.h>
#include <sys/wait.h>
#include "dynamic_list.h"
#include "file_list.h"
#include "mem_list.h"
#include "proc_list.h"

void cmd_uid(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_envvar(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_showenv(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_fork(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_exec(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_jobs(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_deljobs(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_any(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

#endif //P3_H
