
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#include "p2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <ctype.h>

#define TAMANO 1024



void Recursiva (int n){
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

void cmd_recurse(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    int input;
    char* flag;

    if (tokens[1] != NULL) {
        input=strtol(tokens[1],&flag,10); // un atoi mejorado
        if(*flag=='\0') {
            if (input>0) {
                Recursiva(input);
            }else printf(" Tamaño no valido: %d\n", input);
        }else printf(" NUmero invalido");
    }
}

void cmd_malloc(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    int input,i=0;
    char* flag;
    void* memDir;
    tPosML p=Mfirst(*ML);
    tItemML d;
    time_t t=time(NULL);
    struct tm *tm_info = localtime(&t);
    d.time = *tm_info;   // copia por valor
    size_t size;

    if (tokens[1] == NULL) {
        printf("%s->%2s%10s\n", "index", "memDir", "tamaño");
        for(;p!=MLNULL;p=Mnext(p,*ML)) {
            memDir=MgetItem(p,*ML).memDir;
            size=MgetItem(p,*ML).size;
            if (MgetItem(p,*ML).type==0) printf("%d->%12p   %ld\n",i+1, memDir, size);
            i++;
        }
    }else {
        size=strtoul(tokens[1],&flag,10); // un atoi mejorado
        if(*flag=='\0') {
            if (size>0) {
                d.size=size;
                d.memDir=malloc(size*sizeof(char));
                if (d.memDir == NULL) {
                    printf("Malloc fallido\n");
                    return;
                }
                d.name="";
                d.type=0;
                d.fd=-1;
                d.key=-1;
                MinsertItem(d,p,ML);
                printf("Asignados %ld bytes en %p\n", d.size, d.memDir);
            }else printf(" Tamaño no valido: %ld\n", size);
        }else {
            if (tokens[2]==NULL) {
                printf("faltan argumentos\n");
                return;
            }
            if (strcmp(tokens[1],"-free")==0) {

                input=strtol(tokens[2],&flag,10); // un atoi mejorado
                if (*flag != '\0' || input <= 0) {
                   printf("Índice no válido\n");
                   return;
                }
                p=findMem(input,*ML);
                if (p == NULL) {
                    printf("Tamaño no encontrado\n");
                    return;
                }
                d=MgetItem(p,*ML);
                free(d.memDir);
                MdeleteAtPosition(p,ML);
                printf("Bloque %d liberado\n", input);
            }
        }
    }
}

void * CadenatoPointer (char * s)
{
    void *p;
    sscanf(s,"%p",&p);
    if (p==NULL)
        errno=EFAULT;
    return p;
}


void cmd_free(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    void* memDir;
    tPosML p;
    tItemML d;

    if (tokens[1]!=NULL) {
        memDir=CadenatoPointer(tokens[1]);
        p=findAddress(memDir,*ML);
        if(p==NULL) {
            printf("Direccion no encotrada\n");
            return;
        }
        d=MgetItem(p,*ML);
        if (d.type==0) {
            free(d.memDir);
            printf("Bloque %p borrado de memoria\n",p);
            MdeleteAtPosition(p,ML);
        } else if (d.type==1) {
            if (shmdt(d.memDir)==-1) {
                perror("shmdt");
            } else {
                printf("Bloque %p borrado de memoria\n",p);
                MdeleteAtPosition(p,ML);
            }
        } else if (d.type==2) {
            if (munmap(d.memDir,d.size)==-1) {
                perror("munmap");
            } else {
                printf("Bloque %p borrado de memoria\n",p);
                MdeleteAtPosition(p,ML);
            }
        } else {
            printf("Direccion %s no asignada con malloc, shared o map\n",tokens[1]);
        }
    }else printf("No hay direccion\n");
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte){
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

void cmd_memfill(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    void* memDir;
    tPosML p;
    tItemML d;
    char* flag;
    int input;

    if (tokens[1]!=NULL && tokens[2]!=NULL && tokens[3]!=NULL) {
        memDir=CadenatoPointer(tokens[1]);
        if (tokens[3][1]=='\0') {
            input=strtol(tokens[2],&flag,10); // un atoi mejorado
            p=findAddress(memDir,*ML);
            if(p==NULL) {
                printf("Direccion no encotrada\n");
                return;
            }
            if(*flag=='\0') {
                    d=MgetItem(p,*ML);
                if (input>0 && input<=d.size) {
                    printf("Llenando memoria en: %p, %d numero de bytes con el byte: %X %2c\n", d.memDir, input, tokens[3][0], tokens[3][0]);
                    LlenarMemoria(d.memDir,input,tokens[3][0]);
                }else printf(" Tamaño no valido: %d\n", input);
            }
        }else printf("Caracter invalido\n");
    }else printf("faltan argumentos\n");
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam)
{
    void * p;
    int aux,id,flags=0777; /*los 9 bits menos significativos de los flags:permisos*/
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
    {
        flags=flags | IPC_CREAT | IPC_EXCL; /*cuando no es crear pasamos de tamano 0*/
    }
    /*cuando no es crear pasamos de tamano 0*/
    if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s); /* si no es crear, necesitamos el tamano, que es s.shm_segsz*/
    /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}

void cmd_shared (char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    int i=0,id;
    key_t cl;
    tItemML d;
    size_t tam;
    void *p;
    tPosML pos=Mfirst(*ML);
    time_t t=time(NULL);
    struct tm *tm_info = localtime(&t);
    d.time = *tm_info;   // copia por valor



    if (tokens[1]==NULL) {
        printf("%s->%2s%10s%2s\n", "index", "memDir", "llave","tamaño");
        for(;pos!=MLNULL;pos=Mnext(pos,*ML)) {
            if (MgetItem(pos,*ML).type==1) printf("%d->%p %d %ld\n",
                i+1, MgetItem(pos,*ML).memDir, MgetItem(pos,*ML).key, MgetItem(pos,*ML).size);
            i++;
        }
    }else{
        if (tokens[1][0]!='-') {

            cl=(key_t)  strtoul(tokens[1],NULL,10);

            if ((p=ObtenerMemoriaShmget(cl,0))!=NULL)
                printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
            else
                printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
        }else if ( tokens[2]!=NULL){
            if (strcmp(tokens[1], "-create")==0 && tokens[3]!=NULL){
                cl=(key_t)  strtoul(tokens[2],NULL,10); //string to unsigned long
                tam=(size_t) strtoul(tokens[3],NULL,10);

                if (tam==0) {
                    printf ("No se asignan bloques de 0 bytes\n");
                    return;
                }

                if ((p=ObtenerMemoriaShmget(cl,tam))!=NULL) {
                    d.memDir=p;
                    d.size=tam;
                    d.key=cl;
                    d.name="";
                    d.type=1;
                    d.fd=-1;
                    printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
                    MinsertItem(d,pos,ML);
                }
                else
                    printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
            }else if (strcmp(tokens[1], "-free")==0) {
                cl=(key_t)  strtoul(tokens[2],NULL,10);
                p=findKey(cl,*ML);
                if (p==NULL) {
                    printf("key no encontrado\n");
                    return;
                }
                if ((MgetItem(p,*ML).memDir)==NULL){
                    printf ("No hay bloque de esa clave mapeado en el proceso\n");
                    return;
                }
                shmdt(MgetItem(p,*ML).memDir);
                MdeleteAtPosition(p,ML);
            }else if (strcmp(tokens[1], "-delkey")==0) {

                if ((cl=(key_t) strtoul(tokens[2],NULL,10))==IPC_PRIVATE){
                    printf ("      delkey necesita clave_valida\n");
                    return;
                }
                if ((id=shmget(cl,0,0666))==-1){
                    perror ("shmget: imposible obtener memoria compartida");
                    return;
                }
                if (shmctl(id,IPC_RMID,NULL)==-1)
                    perror ("shmctl: imposible eliminar memoria compartida\n");
            }

        }
    }
}

void *MapearFichero(char *fichero, int protection, tMList *ML) {
    int df, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;

    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;

    p = mmap(NULL, s.st_size, protection, MAP_PRIVATE, df, 0);
    if (p == MAP_FAILED) {
        close(df);
        return NULL;
    }

    tItemML item = {0};
    item.memDir = p;
    item.size = s.st_size;
//    item.time = malloc(sizeof(struct tm)); Se pierde el malloc
    time_t now = time(NULL);
    item.time = *localtime(&now);
    item.type = 2;
    item.key = protection;
    item.name = strdup(fichero);
    item.fd = df;

    MinsertItem(item, MLNULL, ML);

    return p;
}

void cmd_mmap(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {
        printf("******Lista de bloques asignados mmap para el proceso %d\n", (int)getpid());

        for (tPosML p = Mfirst(*ML); p != MLNULL; p = Mnext(p, *ML)) {
            tItemML it = MgetItem(p, *ML);
            if (it.type != 2) continue;

            char monthday[12], hourmin[6];
            strftime(monthday, sizeof(monthday), "%b %e", &it.time);
            strftime(hourmin, sizeof(hourmin), "%H:%M", &it.time);

            printf("\t%p\t\t%6lu %s %s %s  (descriptor %d)\n",
                   it.memDir,
                   (unsigned long)it.size,
                   monthday,
                   hourmin,
                   it.name ? it.name : "???",
                   it.fd);
        }
        return;
    }

    // mmap -free fich
    if (strcmp(tokens[1], "-free") == 0) {
        if (tokens[2] == NULL) {
            printf("Uso: mmap -free fichero\n");
            return;
        }
        for (tPosML p = Mfirst(*ML); p != MLNULL; p = Mnext(p, *ML)) {
            tItemML it = MgetItem(p, *ML);
            if (it.type == 2 && it.name && strcmp(it.name, tokens[2]) == 0) {
                munmap(it.memDir, it.size);
                close(it.fd);
                free(it.name);
//                free(it.time); marca error de tipos
                MdeleteAtPosition(p, ML);
                return;
            }
        }
        printf("Fichero %s no está mapeado\n", tokens[2]);
        return;
    }

    // mmap fich perm
    if ((tokens[1] == NULL || tokens[2] == NULL) && tokens[1] != NULL) {
        printf("Uso: mmap fichero permisos(rwx---)\n");
        return;
    }

    char *fichero = tokens[1];
    char *perm = tokens[2];
    int protection = 0;

    if (strchr(perm, 'r')) protection |= PROT_READ;
    if (strchr(perm, 'w')) protection |= PROT_WRITE;
    if (strchr(perm, 'x')) protection |= PROT_EXEC;

    void *p = MapearFichero(fichero, protection, ML);

    if (p == NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n", fichero, p);
}

ssize_t LeerFichero(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;

    if (cont == (size_t)-1)
        cont = s.st_size;

    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }

    close(df);
    return n;
}

void cmd_readfile(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL || tokens[2] == NULL) {
        printf("Uso: readfile fichero direccion [tamano]\n");
        return;
    }

    char *fichero = tokens[1];
    void *dir = NULL;

    sscanf(tokens[2], "%p", &dir);
    if (dir == NULL) {
        printf("Dirección inválida: %s\n", tokens[2]);
        return;
    }

    // chequeamos que la dirección está mapeada
    tPosML pos = findAddress(dir, *ML);
    if (pos == MLNULL) {
        printf("Dirección %p no está mapeada\n", dir);
        return;
    }

    size_t cont = (size_t)-1;
    if (tokens[3] != NULL) {
        char *endptr;
        long tmp = strtol(tokens[3], &endptr, 10);
        if (*endptr != '\0' || tmp < 0) {
            printf("Tamaño inválido: %s\n", tokens[3]);
            return;
        }
        cont = (size_t)tmp;
    }

    ssize_t leidos = LeerFichero(fichero, dir, cont);

    if (leidos == -1) {
        perror("Imposible leer fichero");
    } else {
        printf("Leídos %lld bytes de %s en %p\n", (long long)leidos, fichero, dir);
    }
}

ssize_t EscribirFichero(char *fichero, void *p, size_t cont) {
    int fd;
    ssize_t n;

    fd = open(fichero, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd == -1)
        return -1;

    n = write(fd, p, cont);
    if (n == -1) {
        int aux = errno;
        close(fd);
        unlink(fichero);
        errno = aux;
        return -1;
    }

    close(fd);
    return n;
}

void cmd_writefile(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        printf("Uso: writefile fichero direccion tamano\n");
        return;
    }

    void *dir = NULL;
    sscanf(tokens[2], "%p", &dir);
    if (!dir || findAddress(dir, *ML) == MLNULL)
        return;

    size_t cont = (size_t)strtoul(tokens[3], NULL, 10);
    if (cont == 0) return;

    if (EscribirFichero(tokens[1], dir, cont) == -1)
        perror(tokens[1]);
}

void cmd_read(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        printf("Uso: read df addr cont\n");
        return;
    }
    int fd = (int)strtoul(tokens[1], NULL, 10);

    void *dir = NULL;
    sscanf(tokens[2], "%p", &dir);
    if (!dir || findAddress(dir, *ML) == MLNULL)
        return;

    size_t cont = (size_t)strtoul(tokens[3], NULL, 10);
    if (cont == 0) return;

    ssize_t leidos = read(fd, dir, cont);
    if (leidos == -1)
        perror("read");
    else
        printf("Leídos %ld bytes del descriptor %d en la direccion %p\n", leidos, fd, dir);
}

void cmd_write(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        printf("Uso: write df addr cont\n");
        return;
    }

    int fd = (int)strtoul(tokens[1], NULL, 10);

    void *dir = NULL;
    sscanf(tokens[2], "%p", &dir);
    if (!dir || findAddress(dir, *ML) == MLNULL)
        return;

    size_t cont = (size_t)strtoul(tokens[3], NULL, 10);
    if (cont == 0) return;

    ssize_t escritos = write(fd, dir, cont);

    if (escritos == -1)
        perror("write");
    else
        printf("Escritos %ld bytes en el descriptor %d desde la direccion %p\n", escritos, fd, dir);
}

int static st1_init = 1, st2_init = 2, st3_init = 3;
int static st1, st2, st3;

void mem_funcs() {
    printf("Funciones de programa: \n");
    printf("%p\n", (void*)cmd_malloc);
    printf("%p\n", (void*)cmd_free);
    printf("%p\n", (void*)cmd_read);
    printf("Funciones de libreria: \n");
    printf("%p\n", (void*)printf);
    printf("%p\n", (void*)scanf);
    printf("%p\n", (void*)exit);
}

extern int ex1, ex2, ex3;
extern int ex_init1, ex_init2, ex_init3;
void mem_vars() {
    int l1 = 0, l2 = 1, l3 = 2;
    printf("Variables locales: \n");
    printf("%p\n",(void*)&l1);
    printf("%p\n",(void*)&l2);
    printf("%p\n",(void*)&l3);
    printf("Variables globales: \n");
    printf("%p\n",(void*)&ex_init1);
    printf("%p\n",(void*)&ex_init2);
    printf("%p\n",(void*)&ex_init3);
    printf("Variables estaticas: \n");
    printf("%p\n",(void*)&st1_init);
    printf("%p\n",(void*)&st2_init);
    printf("%p\n",(void*)&st3_init);
    printf("Variables estaticas no inicializadas: \n");
    printf("%p\n",(void*)&st1);
    printf("%p\n",(void*)&st2);
    printf("%p\n",(void*)&st3);
    printf("Variables globales no inicializadas: \n");
    printf("%p\n",(void*)&ex1);
    printf("%p\n",(void*)&ex2);
    printf("%p\n",(void*)&ex3);
}

void mem_blocks(tMList *ML) {
    void* memDir;
    int i = 0;
    tPosML p=Mfirst(*ML);
    size_t size;
    char type[10];
    key_t key;

    printf("%s->%2s%10s\n", "index", "memDir", "tamaño");
    for(;p!=LNULL;p=Mnext(p,*ML)) {
        memDir=MgetItem(p,*ML).memDir;
        size=MgetItem(p,*ML).size;
        if (MgetItem(p,*ML).type == 0) {
            strcpy(type, "malloc");
            if (MgetItem(p,*ML).type==0) printf("%d->%12p   %ld %s\n",i+1, memDir, size , type);
        } if (MgetItem(p,*ML).type == 1) {
            strcpy(type, "shared");
            key = MgetItem(p,*ML).key;
            if (MgetItem(p,*ML).type==0) printf("%d->%12p   %ld %s (key %i)\n",i+1, memDir, size , type, key);
        }
        i++;
    }

}

void Do_pmap (void) /*sin argumentos*/{
    pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

       argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
       if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
           perror("cannot execute procmap (OpenBSD)");

       argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
       if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
           perror("cannot execute vmmap (Mac-OS)");
       exit(1);
  }
  waitpid (pid,NULL,0);
}

void cmd_mem(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (strcmp(tokens[1], "-funcs") == 0) {
        mem_funcs();
    }
    else if (strcmp(tokens[1], "-blocks") == 0) {
        mem_blocks(ML);
    }else if (strcmp(tokens[1], "-vars") == 0) {
        mem_vars();
    }else if (strcmp(tokens[1], "-pmap") == 0) {
        Do_pmap();
    }else if (strcmp(tokens[1], "-all") == 0) {
        mem_funcs();
        mem_vars();
        mem_blocks(ML);
    } else {
        perror("Flag no reconocido");
    }
}


void cmd_memdump(char *tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    int cont=0, input, i;
    void* memDir;
    char* flag;
    unsigned char c;
    bool finish=false;

    if (tokens[1] == NULL ) {
        perror("Direccion no dada\n");
    }else if (tokens[2] == NULL ) {
        perror("Numero de bytes a mostrar no dados\n");
    } else {
        memDir=CadenatoPointer(tokens[1]);
        input=strtol(tokens[2],&flag,10); // un atoi mejorado

        if (*flag!='\0') {
            printf("Numero no vaido\n");
            return;
        }

        while (!finish){
            printf("%p->",memDir);
            for (i=0; i<20 && cont<input;i++) {
                c=*((unsigned char*)memDir+i);
                if (c==9) printf("%3c%c",'\\','t');
                else if (c==10) printf("%3c%c",'\\','n');
                else if (c==13) printf("%3c%c",'\\','r');
                else if (c==20) printf("%4c",' ');
                else if (isprint(c)) printf("%4c", c);
                else printf("%4c",' ');
                cont++;
            }
            cont-=i;
            printf("\n");

            printf("%p->",memDir);
            for (i=0; i<20 && cont<input;i++) {
                c=*((unsigned char*)memDir+i);
                printf("%4X", c);
                cont++;
            }
            printf("\n");
            memDir+=20;
            finish=cont==input;
        }
    }
}
