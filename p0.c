
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#include "p0.h"
#include "dynamic_list.h"
#include "file_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <fcntl.h>

#include "p1.h"


int AuxTrocearCadena(char * cadena, char * trozos[])
{ int i=1;

    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void cmd_authors(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {
        printf("Autores: Jorge Neira Cociña, jorge.neirac\n Daniel Bouzas Piñeiro,  daniel.bouzasp \n"
               "Santiago Cabral Silva, santiago.csilva\n");
    }
    else if (!strcmp(tokens[1], "-l")) {
        printf("Logins de los autores: jorge.neirac\ndaniel.bouzasp\nsantiago.csilva\n");
    } else
        if (!strcmp(tokens[1], "-n")) {
            printf("Nombres de los autores: Jorge Neira Cociña\nDaniel Bouzas Piñeiro\n"
                   "Santiago Cabral Silva\n");
        } else printf("flag no reconocido\n");
}

void cmd_getpid(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    pid_t pid;
    if (tokens[1] == NULL) { //getPid del proceso actual
        pid = getpid();
        printf("Pid del shell: %d\n",pid);
    } else
        if (!strcmp(tokens[1], "-p")) {
            pid = getppid();
            printf("Pid del padre del shell: %d\n",pid);
        } else printf("flag no reconocido\n");
}

void cmd_exit(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    liberarMemoria(ML);
	deleteList(L);
    FdeleteList(FL);
    exit(0);
}

void cmd_getcwd(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp){ //el unistd para la funcion
    char cwd[200];
    // primer parametro: el string donde se almacena segundo parametro: Tamaño del string

    if (getcwd(cwd, sizeof(cwd))!= NULL) {
        printf("%s\n", cwd);
    } else {
        perror("error obteniendo el path del directorio actual");
    }
}

void cmd_dup(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {
        perror("Error: no se ha proporcionado un descriptor de archivo");
    } else {
        int fd = atoi(tokens[1]);
        int new_fd = dup(fd);
        if (new_fd == -1) {
            perror("Error: no se pudo duplicar el descriptor de archivo");
        } else {
            printf("Se ha duplicado el descriptor de archivo %d a %d\n", fd, new_fd);
        }
    }

}

void cmd_chdir(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp){
	if (tokens[1] == NULL) {
		cmd_getcwd(tokens,L,FL,ML,PL,envp); // llamo a getcwd con todos los tokens
    } else{
		if(chdir(tokens[1])!=0){
// chdir devuelve 0 si lo logra cambiar devuelve -1 si no lo logra cambiar
			perror("Path erroneo\n");
		}
	}
}

void cmd_infosys(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    struct utsname sysinfo; //struct para almacenar info del sistema
    if (uname(&sysinfo) == 0) {
        printf("%s (%s), OS: %s-%s-%s\n",
               sysinfo.nodename, // hostname
               sysinfo.machine, // arquitectura
               sysinfo.sysname, // nombre del so
               sysinfo.release, // version del kernel
               sysinfo.version // fecha de compilacion
               );
    }
}

void cmd_date(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    time_t t = time(NULL); //guardamos en t los segundos desde la epoca
    char buffer_fecha[32];
    char buffer_hora[32];
    struct tm *lt = localtime(&t); //cambiamos los segundos a fecha y hora
    strftime(buffer_fecha, sizeof(buffer_fecha), "%d-%m-%Y", lt); //la formateamos y guardamos
    strftime(buffer_hora, sizeof(buffer_hora), "%H:%M:%S", lt);   //desglosada en fecha y hora

    if (tokens[1] == NULL && strcmp(tokens[0], "hour") != 0) {
        printf("Fecha y hora: %s , %s\n", buffer_fecha, buffer_hora); //para luego imprimirla separada
    } else
    if (!strcmp(tokens[0], "hour") || !strcmp(tokens[1], "-t")) {
        printf("Hora: %s\n",buffer_hora);
    } else if (!strcmp(tokens[1], "-d")) {
        printf("Fecha: %s\n",buffer_fecha);
    } else printf("flag no reconocido\n");
}


void historicN(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp){

     if (!strcmp(tokens[0], "authors")) {
        cmd_authors(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "getpid")) {
         cmd_getpid(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "chdir")) {
         cmd_chdir(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "getcwd")) {
         cmd_getcwd(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "date")) {
         cmd_date(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "hour")) {
         cmd_date(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "historic")) {
         cmd_historic(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "open")) {
         cmd_open(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "close")) {
         cmd_close(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "dup")) {
         cmd_dup(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "listopen")) {
         cmd_open(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "infosys")) {
         cmd_infosys(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "help")) {
         cmd_help(tokens,L,FL,ML,PL,envp);
     } else if (!strcmp(tokens[0], "create")) {
         cmd_create(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "setdirparams")) {
         cmd_setdirparams(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "getdirparams")) {
         cmd_getdirparams(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "dir")) {
         cmd_dir(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "erase")) {
         cmd_erase(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "delrec")) {
         cmd_erase(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "lseek")) {
         cmd_lseek(tokens,L,FL,ML,PL,envp);
     }else if (!strcmp(tokens[0], "writestr")) {
         cmd_writestr(tokens,L,FL,ML,PL,envp);
   }
   else printf("Comando no reconocido\n");

}

void cmd_historic(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) { //Empieza en 1  el historic N y empieza en -1 el historic -N
    char d [1024];
    char* trozos [256];
    tPosL p=first(*L);
    char* flag;
    int input=0,i=0, nElements;
    if (tokens[1] == NULL) {
        for(;p!=LNULL;p=next(p,*L)) {
            strcpy(d,getItem(p,*L));
            printf("%d->%s",i+1,d);
            i++;
        }
    }else{
        input=strtol(tokens[1],&flag,10); // un atoi mejorado
        nElements=countElements(*L);
        if(*flag=='\0'){
            if(input<0){
                input=-input;
                if (input > nElements) input = nElements;
                // Avanzar hasta el primer elemento a mostrar
                p = first(*L);
                for (i = 1; i < nElements - input + 1 ; i++) {
                    p = next(p, *L);
                }
                // Imprimir los últimos 'input' elementos
                while( p != LNULL){
                    i++;
                    printf("%d->%s", i+1, getItem(p, *L)); //muestra el numero de elementos que es
                    p = next(p, *L);
                }
            }else{

                if ( input==0 || input > nElements) return;

                for (int j = 1; j < input; j++) {
                    p = next(p, *L);
                }
                printf("%s:\n",getItem(p, *L));
				AuxTrocearCadena(getItem(p, *L), trozos);
                historicN(trozos,L,FL,ML,PL,envp);
                deleteAtPosition(last(*L), L);
           }
        }else{

        if (!strcmp(tokens[1], "-count")) {
            printf("Hay %d comandos registrados\n",nElements);
        } else
            if (!strcmp(tokens[1], "-clear")) {
                deleteList(L);
                printf("Historic limpio\n");
            } else printf("flag no reconocido\n");}
    }
}

char* flag_traductor(int flag) { //con esta función sabemos la cadena para cada modo
                                //esta funcion hace saltos con valores no inicializados sera mejor rehacerla
    int access = flag & O_ACCMODE;
    if (access == O_CREAT) return "cr";
    if (access == O_RDONLY) return "ro";
    if (access == O_WRONLY) return "wo";
    if (access == O_RDWR) return "rw";
    if (access ==  O_EXCL) return "ex";
    if (access == O_APPEND) return "ap";
    if (access == O_TRUNC) return "tr";
    return "?";
}


void cmd_open(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    int i,fd, mode=0;
    if (tokens[1] == NULL || strcmp(tokens[0],"listopen") == 0) {           //sin flags lista los directorios abiertos

        for (tfPosL p=Ffirst(*FL);p!=FLNULL;p=Fnext(p,*FL)) {//si no hay ninguno: in out err
            tfItemL d = FgetItem(p,*FL);
            d.flags=fcntl(d.fd,F_GETFL);
            printf("Descriptor: %i, nombre: %s, modo: ", d.fd, d.filename);
            int flags = FgetItem(p,*FL).flags;
            if (flags & O_CREAT) printf("O_CREAT ");
            if  (flags & O_EXCL) printf("O_EXCL ");

            if (flags & O_WRONLY) printf("O_WRONLY ");
            else
            if (flags & O_RDWR) printf("O_RDWR ");
            else printf("O_RDONLY ");

            if (flags & O_APPEND) printf("O_APPEND ");
            if (flags & O_TRUNC) printf("O_TRUNC ");


            printf("\n");
        }
         return;
    }
    for (i=2; tokens[i]!=NULL; i++)
        if (!strcmp(tokens[i],"cr")) mode|=O_CREAT;
        else if (!strcmp(tokens[i],"ex")) mode|=O_EXCL;
        else if (!strcmp(tokens[i],"ro")) mode|=O_RDONLY;
        else if (!strcmp(tokens[i],"wo")) mode|=O_WRONLY;
        else if (!strcmp(tokens[i],"rw")) mode|=O_RDWR;
        else if (!strcmp(tokens[i],"ap")) mode|=O_APPEND;
        else if (!strcmp(tokens[i],"tr")) mode|=O_TRUNC;
        else break;


    if ((fd=open(tokens[1],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        tfItemL d;
        d.flags = mode;
        d.filename = strdup(tokens[1]); //hace el malloc y copia el contenido
        d.fd = fd;
        FinsertItem(d,LNULL,FL);
        printf("Añadido fichero abierto a la entrada %d \n",fd);
    }
}

void cmd_close(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {//si no se le pasa el fd imprimimos la lista de ficheros abiertos
        for (tfPosL p=Ffirst(*FL) ; p!=FLNULL ; p=Fnext(p,*FL)) {
            tfItemL d = FgetItem(p,*FL);
            d.flags=fcntl(d.fd,F_GETFL);
            printf("Descriptor: %i, nombre: %s, modo: %s \n", d.fd, d.filename, flag_traductor(d.flags));
                                                                                    //^^^^^^^^^
        }
        return;
    }
        int fd = atoi(tokens[1]);
        FdeleteAtPosition(FfindItem(fd,*FL),FL);
        close(fd);
}

void cmd_help(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    //Esta Funcion Me Quita Años De Vida
    if (tokens[1] == NULL) {
        printf("Los comandos disponibles son: \nP0: authors getpid chdir getcwd date"
               "hour historic open close dup listopen infosys help quit exit bye\n"
               "P1: create setdirparams getdirparams dir erase delrec lseek writestr\n");

    } else {
        if (!strcmp(tokens[1], "authors")) {
            printf("authors [-n|-l] Imprime los nombres y logins de los autores\n");
        } else if (!strcmp(tokens[1], "getpid")) {
            printf("getpid [-p] Muestra el pid del shell o de su proceso padre\n");
        } else if (!strcmp(tokens[1], "chdir")) {
            printf("chdir [dir] Cambia o muestra el directorio actual del shell\n");
        } else if (!strcmp(tokens[1], "getcwd")) {
            printf("getcwd Muestra el directorio actual del shell\n");
        } else if (!strcmp(tokens[1], "date")) {
            printf("date [-t|-d] Muestra la fecha y hora actual\n");
        } else if (!strcmp(tokens[1], "hour")) {
            printf("hour Muestra la hora actual\n");
        } else if (!strcmp(tokens[1], "historic")) {
            printf("historic [-N|N|-clear|-count] Muestra el histórico de comandos\n");
        } else if (!strcmp(tokens[1], "open")) {
            printf("open f mode1 mode2 ... Abre el fichero f y lo añade a la lista de ficheros abiertos en "
                   "el shell, mode1, mode2... es el modo de apertura\n");
        } else if (!strcmp(tokens[1], "close")) {
            printf("close df Cierra el descriptor df y elimina el fichero de la lista de ficheros abiertos\n");
        } else if (!strcmp(tokens[1], "dup")) {
            printf("dup df Duplica el descriptor df y lo añade con una nueva entrada a la lista de ficheros "
                   "abiertos\n");
        } else if (!strcmp(tokens[1], "listopen")) {
            printf("listopen [n] Lista los (n) ficheros abiertos del shell\n");
        } else if (!strcmp(tokens[1], "infosys")) {
            printf("infosys Muestra la información de la máquina que corre el shell\n");
        } else if (!strcmp(tokens[1], "help")) {
            printf("help [cmd] Muestra todos los comandos (o ayuda sobre cmd específico)\n");
        } else if (!strcmp(tokens[1], "quit")) {
            printf("quit Termina la ejecución del shell\n");
        } else if (!strcmp(tokens[1], "exit")) {
            printf("exit Termina la ejecución del shell\n");
        } else if (!strcmp(tokens[1], "bye")) {
            printf("bye Termina la ejecución del shell\n");
        }
        else if (!strcmp(tokens[1], "create")) {
            printf("create [-f] [name] Crea un directorio o fichero (-f)\n");
        }
        else if (!strcmp(tokens[1], "setdirparams")) {
            printf("setdirparams [long|short][hid|nohid][lnk|nolnk][recb|reca|norec] \n"
                   "Establece parametros de los listados con dir:\n"
                   "long|short => listado largo o corto\n"
                   "hid|nohid => lista o no ocultos\n"
                   "link|unlink => lista o no el destino de los enlaces simbolicos\n"
                   "recb|reca|norec => si el listado es recursivo, antes(before) o despues(after)\n");
        }
        else if (!strcmp(tokens[1], "getdirparams")) {
            printf("Muestra los parametros de los listados con dir\n");
        }
        else if (!strcmp(tokens[1], "dir")) {
            printf("dir [-d] Lista los ficheros y directorios que se le pasa por argumento\n"
                   "de acuerdo a lo establecido en setdirparams (long, hid, link,,,)\n"
                   "-d => lista los contenidos de los directorios\n");
        }
        else if (!strcmp(tokens[1], "erase")) {
            printf("erase [n1, n2, ...] Borra los ficheros o directorios vacíos\n");
        }
        else if (!strcmp(tokens[1], "delrec")) {
            printf("delrec [n1, n2, ...] Borra los ficheros o directorios no vacíos recursivamente\n");
        }                                                   //hacerlo en tmp, borrar home es una cosa
        else if (!strcmp(tokens[1], "lseek")) {
            printf("lseek df off ref Posiciona el offset de df en off. ref es la referencia:\n"
                   "SEEK_SET princiipo del fichero\nSEEK_CUR posicion actual\nSEEK_END final del fichero\n");
        }
        else if (!strcmp(tokens[1], "writestr")) {
            printf("writestr df str  Escribe el string str en el fichero descrito por df\n");
        }
    }
}
