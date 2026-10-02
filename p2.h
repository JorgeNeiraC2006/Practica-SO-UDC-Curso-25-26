
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#ifndef P2_H
#define P2_H
#include <stdio.h>
#include "dynamic_list.h"
#include "file_list.h"
#include "mem_list.h"
#include "proc_list.h"

void cmd_recurse(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_malloc(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_free(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_mmap(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_memfill(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_readfile(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_writefile(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_read(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_write(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_mem(char* tokens[], tList  *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_shared(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);
void cmd_memdump(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp);

#endif //P2_H
