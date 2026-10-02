
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#include "p3.h"

#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <sys/resource.h>

extern char **environ;

static char* obtenerUser(uid_t uid) {
    struct passwd *pw = getpwuid(uid);

    return  pw == NULL? NULL : strdup(pw->pw_name);
}

void cmd_uid(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    uid_t uid=getuid(), euid=geteuid();
    char *user=obtenerUser(uid),  *euser=obtenerUser(euid), *flag;
    struct passwd *pw;

    if (tokens[1]==NULL) {
        printf("Sin parametro\n");
    }else if (strcmp(tokens[1],"-get")==0) {
        printf("Credencial real: %u, (%s)\n", uid, user==NULL? "": user);
        printf("Credencial efectiva: %u, (%s)\n", euid, euser==NULL? "": euser);
    }else if (strcmp(tokens[1],"-set")==0) {
        if (tokens[2]==NULL) {
            printf("Sin parametro en el -set\n");
        }else {
            if (strcmp(tokens[2],"-l")==0) {
                if (tokens[3]!=NULL) {
                    pw = getpwnam(tokens[3]);
                    if (pw == NULL) {
                        printf("Usuario no encontrado: %s\n", tokens[3]);
                        return;
                    }
                    uid = pw->pw_uid;
                    if (seteuid(uid)==-1) {
                        perror("error al settear el UID\n");
                        return;
                    }
                    printf(" UID colocado al user: %s\n",tokens[3]);
                }else printf("ID no dado en el uid -set -l\n");
            }else {
                uid=strtol(tokens[2], &flag, 10);
                if(*flag=='\0') {
                    if (seteuid(uid)==-1) {
                        perror("error al settear el UID\n");
                        return;
                    }
                    printf("Poniendo la UID efectiva a: %u\n", uid);
                }else printf("ID mal dado\n");
            }
        }
    }

    if(user!=NULL) free(user);
    if(euser!=NULL) free(euser);
}


void cmd_envvar(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
        if (tokens[1] == NULL) {
        cmd_showenv(tokens, L, FL, ML, PL, envp);
        return;
    }

    if (strcmp(tokens[1], "-show") == 0 && tokens[2] == NULL) {
        char* showenv_tokens[] = {"showenv", NULL};
        cmd_showenv(showenv_tokens, L, FL, ML, PL, envp);
        return;
    }

    if (strcmp(tokens[1], "-show") == 0) {
        char *nombre_var = tokens[2];
        char *valor_getenv = getenv(nombre_var);

        if (valor_getenv == NULL) return;

        size_t len = strlen(nombre_var);

        if (envp != NULL) {
            for (int i = 0; envp[i] != NULL; i++) {
                if (strncmp(envp[i], nombre_var, len) == 0 && envp[i][len] == '=') {
                    printf("Con arg3 main ");
                    char *valor_envp = envp[i] + len + 1;
                    printf("%s=%s(0x%lx) @0x%lx\n",
                           nombre_var, valor_envp,
                           (unsigned long)valor_envp, (unsigned long)envp[i]);
                    break;
                }
            }
        }

        printf("  Con environ ");
        int encontrado_environ = 0;
        if (environ != NULL) {
            for (int i = 0; environ[i] != NULL; i++) {
                if (strncmp(environ[i], nombre_var, len) == 0 && environ[i][len] == '=') {
                    char *valor_environ = environ[i] + len + 1;
                    printf("%s=%s(0x%lx) @0x%lx\n",
                           nombre_var, valor_environ,
                           (unsigned long)valor_environ, (unsigned long)environ[i]);
                    encontrado_environ = 1;
                    break;
                }
            }
        }
        if (!encontrado_environ) printf("\n");

        printf("   Con getenv ");
        printf("%s(0x%lx)\n", valor_getenv, (unsigned long)valor_getenv);
        return;
    }

    if (strcmp(tokens[1], "-change") == 0) {
        if (tokens[2] == NULL || tokens[3] == NULL || tokens[4] == NULL) {
            printf("Uso: -change [-a|-e|-p] var valor\n");
            return;
        }

        char *modo = tokens[2];
        char *nombre_var = tokens[3];
        char *nuevo_valor = tokens[4];
        size_t len = strlen(nombre_var);

        if (strcmp(modo, "-a") == 0 || strcmp(modo, "-e") == 0) {
            char ***array = (strcmp(modo, "-a") == 0) ? &envp : &environ;
            int indice = -1;

            if (*array != NULL) {
                for (int i = 0; (*array)[i] != NULL; i++) {
                    if (strncmp((*array)[i], nombre_var, len) == 0 && (*array)[i][len] == '=') {
                        indice = i;
                        break;
                    }
                }
            }

            if (indice == -1) {
                errno = ENOENT;
                perror("Imposible cambiar variable");
                return;
            }

            char *nueva = malloc(len + 1 + strlen(nuevo_valor) + 1);
            if (nueva == NULL) {
                perror("malloc");
                return;
            }
            snprintf(nueva, len + 1 + strlen(nuevo_valor) + 1, "%s=%s", nombre_var, nuevo_valor);
            (*array)[indice] = nueva;
            return;
        }

        if (strcmp(modo, "-p") == 0) {
            char *nueva = malloc(len + 1 + strlen(nuevo_valor) + 1);
            if (nueva == NULL) {
                perror("malloc");
                return;
            }
            snprintf(nueva, len + 1 + strlen(nuevo_valor) + 1, "%s=%s", nombre_var, nuevo_valor);

            if (putenv(nueva) != 0) {
                perror("putenv");
                free(nueva);
            }
            return;
        }

        printf("Uso: -change [-a|-e|-p] var valor\n");
        return;
    }
}


void cmd_showenv(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
        if (tokens[1] == NULL) {
        if (envp != NULL) {
            for (int i = 0; envp[i] != NULL; i++) {
                printf("0x%lx->main arg3[%d]=(0x%lx) %s\n",
                       (unsigned long)&envp[i],  // dirección del elemento -> array envp
                       i,
                       (unsigned long)envp[i],   // dirección del contenido
                       envp[i]);                 // contenido
            }
        }
        return;
    }

    if (strcmp(tokens[1], "-environ") == 0) {
        if (environ != NULL) {
            for (int i = 0; environ[i] != NULL; i++) {
                printf("0x%lx->environ[%d]=(0x%lx) %s\n",
                       (unsigned long)&environ[i],  // dirección del elemento ->  environ
                       i,
                       (unsigned long)environ[i],
                       environ[i]);
            }
        }
        return;
    }

    if (strcmp(tokens[1], "-addr") == 0) {
        printf("environ:   ");
        if (environ != NULL) {
            printf("0x%lx", (unsigned long)environ);
        }
        printf(" (almacenado en 0x%lx)\n", (unsigned long)&environ);

        printf("main arg3: ");
        if (envp != NULL) {
            printf("0x%lx", (unsigned long)envp);
        }
        printf(" (almacenado en 0x%lx)\n", (unsigned long)&envp);
        return;
    }

    printf("Uso: showenv [-environ|-addr]\n");
}


void cmd_fork(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    pid_t pid;

    if ((pid = fork()) == 0){
        printf ("ejecutando proceso %d\n", getpid());
    } else if (pid != -1) {
        waitpid (pid, NULL, 0);
    } else {
        perror("fork: error al crear proceso");
    }
}


void cmd_exec(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {
        errno = EFAULT;
        perror("Imposible ejecutar");
        return;
    }

    char* args[256];
    int arg_count = 0;

    for (int i = 1; tokens[i] != NULL && arg_count < 255; i++) {
        args[arg_count++] = tokens[i];
    }

    args[arg_count] = NULL;
    execvp(args[0], args);
    perror("Imposible ejecutar");
}


void printearProceso(tPItemL proceso) {
    char datebuf[64];
    struct tm *tm_info;
    int prio;

    // Mostrar PID
    printf("pid: %i ", proceso.pid);

    // Mostrar fecha y hora de inicio
    tm_info = localtime(&proceso.starttime);
    strftime(datebuf, 64, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s ", datebuf);

    // Mostrar estado
    if (proceso.estado == FINISHED) {
        printf("FINISHED(%d) ", proceso.exitcode);
    }
    else if (proceso.estado == ACTIVE) {
        printf("ACTIVE ");
    }
    else if (proceso.estado == STOPPED) {
        printf("STOPPED ");
    }
    else if (proceso.estado == SIGNALED) {
        printf("SIGNALED(%d) ", proceso.signal);
    }

    // Obtener y mostrar prioridad
    prio = getpriority(PRIO_PROCESS, proceso.pid);

    printf("prio = %i ", prio);

    // Mostrar nombre del proceso
    printf("%s\n", proceso.name);
}

void cmd_jobs(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    int status;
    tPPosL p;
    pid_t r;
    tPItemL proceso;

    // Si la lista está vacía, no hay nada que mostrar
    if (PisEmptyList(*PL)) {
        printf("No hay procesos en segundo plano\n");
        return;
    }

    // Recorrer la lista de procesos
    p = Pfirst(*PL);
    while (p != PLNULL) {
        // Verificar el estado del proceso con waitpid
        r = waitpid(p->data.pid, &status, WNOHANG|WUNTRACED|WCONTINUED);

        // Obtener el proceso actual
        proceso = PgetItem(p, *PL);

        if (r == p->data.pid) {
            // Este proceso específico cambió de estado
            if (WIFEXITED(status)) {
                proceso.estado = FINISHED;
                proceso.exitcode = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status)) {
                proceso.estado = SIGNALED;
                proceso.signal = WTERMSIG(status);
            }
            else if (WIFSTOPPED(status)) {
                proceso.estado = STOPPED;
                proceso.signal = WSTOPSIG(status);
            }
            else if (WIFCONTINUED(status)) {
                proceso.estado = ACTIVE;
            }

            // Actualizar el proceso en la lista
            PupdateItem(proceso, p, PL);
        }
        else if (r == -1 && errno == ECHILD) {
            // El proceso ya no existe (fue reaped por otro wait)
            proceso.estado = FINISHED;
            proceso.exitcode = -1;
            PupdateItem(proceso, p, PL);
        }

        // Obtener el proceso actualizado y mostrarlo
        proceso = PgetItem(p, *PL);
        printearProceso(proceso);

        // Pasar al siguiente proceso
        p = Pnext(p, *PL);
    }
}


void cmd_deljobs(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
        tPPosL p= Pfirst(*PL), first= Pfirst(*PL), aux;
    tPItemL d;

    if (tokens[1] == NULL) {
        printf("Faltan parametros\n");
        return;
    }

    if (strcmp(tokens[1], "-term") == 0) {
        printf("Borrando procesos terminados\n");
        while (p != LNULL) {
            first=Pfirst(*PL);
            aux = Pnext(p, *PL);
            d = PgetItem(p, *PL);
            if (d.estado==FINISHED) {

                PdeleteAtPosition(p, PL); // Elimina la consola

                if (p==first) {
                    p = Pfirst(*PL);
                }
            } else p = aux;
        }
    }
    else if (strcmp(tokens[1], "-sig") == 0) {
        printf("Borrando procesos señalados\n");

        while (p != LNULL) {
            first=Pfirst(*PL);
            aux = Pnext(p, *PL);
            d = PgetItem(p, *PL);
            if (d.estado==SIGNALED) {

                PdeleteAtPosition(p, PL); // Elimina la consola

                if (p==first) {
                    p = Pfirst(*PL);
                }
            } else p = aux;
        }
    }
    else {
        printf("Flag no reconocido\n");
    }
}


void cmd_any(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[0] == NULL) return;

    char* args[256];
    int priority = -1; // -1 prioridad no definida
    int background = 0; // 0 foreground 1 background
    int arg_count = 0;
    char* programa = NULL;

    for (int i = 0; tokens[i] != NULL && arg_count < 255; i++) {
        if (tokens[i][0] == '@') { // prioridad
            char* endptr;
            long pri = strtol(tokens[i] + 1, &endptr, 10);
            if (*endptr != '\0' || pri < 0 || pri > 99) {
                printf("prioridad inválida '%s'\n", tokens[i]);
                return;
            }
            priority = (int)pri;
        }
        else if (strcmp(tokens[i], "&") == 0) { // Check background
            background = 1;
        }
        else {
            if (programa == NULL) {
                programa = tokens[i];
            }
            args[arg_count++] = tokens[i];
        }
    }

    if (arg_count == 0) return;

    args[arg_count] = NULL;

    pid_t pid = fork();

    if (pid == 0) {
        if (background) {
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
        }
        if (priority != -1) {
            setpriority(PRIO_PROCESS, 0, priority);
        }
        execvp(args[0], args);
        perror("No ejecutado");
        exit(EXIT_FAILURE);
    } else if (pid != -1) {
        if (background) {
            tPItemL proceso;
            proceso.pid = pid;
            proceso.uid = getuid();

            size_t name_len = strlen(programa) + 1;
            proceso.name = malloc(name_len);
            if (proceso.name != NULL) {
                strncpy(proceso.name, programa, name_len);
                proceso.name[name_len - 1] = '\0';
            } else {
                proceso.name = NULL;
            }

            proceso.estado = ACTIVE;
            proceso.exitcode = -1;
            proceso.signal = -1;
            proceso.starttime = time(NULL);

            if (PisEmptyList(*PL)) { // insertamos como primer elemento (lista vacia)
                if (!PinsertItem(proceso, PLNULL, PL)) {
                    if (proceso.name != NULL) {
                        free(proceso.name);
                    }
                }
            } else { // insertamos al final (lista con algún elemento)
                tPPosL ultima_pos = Plast(*PL);
                if (ultima_pos == PLNULL) {
                    while (Pnext(ultima_pos, *PL) != PLNULL) {
                        ultima_pos = Pnext(ultima_pos, *PL);
                    }
                }

                if (!PinsertItem(proceso, ultima_pos, PL)) {
                    if (proceso.name != NULL) {
                        free(proceso.name);
                    }
                }
            }

        }   else {
            int status;
            waitpid(pid, &status, 0);
        }
    } else {
        perror("fork");
    }
}
