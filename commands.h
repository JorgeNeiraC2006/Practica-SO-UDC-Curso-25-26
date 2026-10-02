
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "p0.h"
#include "p1.h"
#include "p2.h"
#include "p3.h"

typedef void (*cmd_handler)(char*[],tList*, tfList*, tMList*, tPList*, char **envp);

typedef struct {
    char* name;
    cmd_handler handler;
} cmd_data;

cmd_data commands[] = {
    {"authors", cmd_authors},
    {"getpid", cmd_getpid},
    {"exit", cmd_exit},
    {"quit", cmd_exit},
    {"bye", cmd_exit},
    {"date", cmd_date},
    {"hour", cmd_date},
    {"getcwd", cmd_getcwd},
	{"chdir", cmd_chdir},
	{"historic", cmd_historic},
    {"dup", cmd_dup},
    {"infosys", cmd_infosys},
    {"help", cmd_help},
    {"open", cmd_open},
    {"close", cmd_close},
    {"listopen", cmd_open},
    {"create", cmd_create},
    {"setdirparams", cmd_setdirparams},
    {"getdirparams", cmd_getdirparams},
    {"dir", cmd_dir},
    {"erase", cmd_erase},
    {"delrec", cmd_delrec},
    {"lseek", cmd_lseek},
    {"writestr", cmd_writestr},
    {"malloc",cmd_malloc},
    {"recurse",cmd_recurse},
    {"free",cmd_free},
    {"mmap",cmd_mmap},
    {"memfill",cmd_memfill},
    {"readfile",cmd_readfile},
    {"writefile",cmd_writefile},
    {"read",cmd_read},
    {"write",cmd_write},
    {"mem", cmd_mem},
    {"shared", cmd_shared},
    {"memdump", cmd_memdump},
    {"uid", cmd_uid},
    {"envvar", cmd_envvar},
    {"showenv", cmd_showenv},
    {"fork", cmd_fork},
    {"exec", cmd_exec},
    {"jobs", cmd_jobs},
    {"deljobs", cmd_deljobs}
};

int num_commands = sizeof(commands) / sizeof(cmd_data);

#endif //COMMANDS_H
