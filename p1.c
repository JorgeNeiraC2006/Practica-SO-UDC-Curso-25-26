
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#include "p1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

int ex1, ex2, ex3;
int ex_init1 = 1, ex_init2 = 2, ex_init3 = 3;

static dirparams p;

char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}
/*las siguientes funciones devuelven los permisos de un fichero en formato rwx----*/
/*a partir del campo st_mode de la estructura stat */
/*las tres son correctas pero usan distintas estrategias de asignaciÃ³n de memoria*/

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

void cmd_create(char* tokens[],tList *L, tfList *FL,  tMList *ML, tPList *PL, char **envp) {
    int error=0;
    if (tokens[1]!=NULL) {
        if (strcmp(tokens[1], "-f") != 0) {
            error=mkdir(tokens[1], 0777);  // crear un directorio con permisos de lectura, escritura y ejecución para el propietario
        } else
            if (!strcmp(tokens[1], "-f")) {
                if (tokens[2]!=NULL) {
                    error=open(tokens[2],O_CREAT | O_RDWR,0777);
                }
            }
        if (error==-1) {
            perror("No se creo el archivo");
        }else close(error);
    }
}

void cmd_setdirparams(char* tokens[],tList *L, tfList *FL,  tMList *ML, tPList *PL, char **envp) {
    if (tokens[1]==NULL) {
        printf("Parametros no dados\n");
    }else if (!strcmp(tokens[1], "long")) {
        p.size=1;
    }else if (!strcmp(tokens[1], "short")) {
        p.size=0;
    }else if (!strcmp(tokens[1], "link")) {
        p.link=1;
    }else if (!strcmp(tokens[1], "nolink")) {
        p.link=0;
    }else if (!strcmp(tokens[1], "hid")) {
        p.hid=1;
    }else if (!strcmp(tokens[1], "nohid")) {
        p.hid=0;
    }else if (!strcmp(tokens[1], "reca")) {
        p.rec=-1;
    }else if (!strcmp(tokens[1], "recb")) {
        p.rec=1;
    }else if (!strcmp(tokens[1], "norec")) {
        p.rec=0;
    }else printf("Parametro incorrecto\n");
}

void cmd_getdirparams(char* tokens[],tList *L, tfList *FL,  tMList *ML, tPList *PL, char **envp) {
    printf(" Listado: ");
    p.size? printf("largo ") : printf("corto ");
    p.link? printf("con links ") : printf("sin links ");
    if (p.rec==-1) {
        printf("recursivo (despues) ");
    }else if (p.rec==1) {
        printf("recursivo (antes) ");
    }else printf("no recursivo ");

    p.hid? printf("con ficheros ocultos\n") : printf("\n");
}

void sindlong(char* dircharlist[], int linktrue) {
    for (int i = 0; dircharlist[i]!=NULL; i++) {
                struct stat st;
        if (linktrue) {
            if (lstat(dircharlist[i],&st) == -1) { //devuelve -1 si falla al obtener info
                perror(dircharlist[i]);             //mete un error
                continue;                       //siguiente i
            }
        } else
            stat(dircharlist[i], &st);
        if (stat(dircharlist[i],&st) == -1) { //devuelve -1 si falla al obtener info
            perror(dircharlist[i]);             //mete un error
            continue;                       //siguiente i
        }
                if (S_ISLNK(st.st_mode)) {      //si es un link
                    if (linktrue) {             //y link es true
                        char destino[50];
                        ssize_t len = readlink(dircharlist[i], destino,sizeof(destino)-1);
                        if (len != -1) {
                            destino[len] = '\0';
                            printf("%i",st.st_mode);
                            char permisos[12];
                            ConvierteModo(st.st_mode, permisos);

                            char fecha[20]; //para cambiar la fecha de segundos y que no haga un salto de linea
                            struct tm *tm_info = localtime(&st.st_mtime);
                            strftime(fecha, sizeof(fecha), "%b %d %H:%M", tm_info);

                            printf("%s %ld %ld %s %s\n", permisos, st.st_nlink, st.st_size, fecha, dircharlist[i]);
                        }
                    }
                    else {                  //si es nolink -> long de lo que apunta el link
                        char permisos[12];
                        ConvierteModo(st.st_mode, permisos);

                        char fecha[20]; //para cambiar la fecha de segundos y que no haga un salto de linea
                        struct tm *tm_info = localtime(&st.st_mtime);
                        strftime(fecha, sizeof(fecha), "%b %d %H:%M", tm_info);

                        struct passwd *pw = getpwuid(st.st_uid);
                        struct group  *gr = getgrgid(st.st_gid);

                        char *owner = pw ? pw->pw_name : "???";
                        char *group = gr ? gr->gr_name : "???";         ///esto hace que cuando el usuario ni el grupo existen
                        //printee ???? como en el shell de referencia

                        printf("%s:%s %s %ld %ld %s %s\n",owner, group, permisos, st.st_nlink, st.st_size, fecha, dircharlist[i]);
                    }
                }
                else {      //si no es un link
                    char permisos[12];
                    ConvierteModo(st.st_mode, permisos);

                    char fecha[20]; //para cambiar la fecha de segundos y que no haga un salto de linea
                    struct tm *tm_info = localtime(&st.st_mtime);
                    strftime(fecha, sizeof(fecha), "%b %d %H:%M", tm_info);

                    printf("%s %ld %ld %s %s\n", permisos, st.st_nlink, st.st_size, fecha, dircharlist[i]);
                }
            }
}

void sindshort(char* dircharlist[], int linktrue) {
    for (int i = 0; dircharlist[i]!=NULL; i++) {
        struct stat st;
        if (linktrue) {
            if (lstat(dircharlist[i],&st) == -1) { //devuelve -1 si falla al obtener info
                perror(dircharlist[i]);             //mete un error
                continue;                       //siguiente i
            }
        } else
            stat(dircharlist[i], &st);
             if (stat(dircharlist[i],&st) == -1) { //devuelve -1 si falla al obtener info
                    perror(dircharlist[i]);             //mete un error
                    continue;                       //siguiente i
             }
        if (S_ISLNK(st.st_mode)) {     //si es un link
            if (linktrue) {                         //si link es un parametro
                char destino[50];
                ssize_t len = readlink(dircharlist[i], destino,sizeof(destino)-1);
                if (len != -1) {
                    destino[len] = '\0';
                    printf("nombre %s -> destino %s\n", dircharlist[i],destino);
                } else {
                    printf("%s, enlace con destino desconocido", dircharlist[i]);
                }
            }
            else {                              //si nolink es el parametro
                printf("Nombre: %s, Tamaño: %ld\n", dircharlist[i],st.st_size);
            }
        }
        printf("Nombre: %s, Tamaño: %ld\n", dircharlist[i],st.st_size);
    }
}

void dir_sind(char* tokens[]) {
    char* dircharlist[10];
        int j = 0;
        for (int i = 1; tokens[i]!=NULL; i++) {
            dircharlist[j] = tokens[i];
            j++;
        }
    dircharlist[j] = NULL;

        if (!p.size) {                                     //si no es long
            sindshort(dircharlist,p.link);
        }
        else {
            //si es long
            sindlong(dircharlist, p.link);
        }
} //reparado

void condlong(char* dircharlist[], int linktrue, int hidtrue, int recursive) {
    for (int i = 0; dircharlist[i]!=NULL; i++) {
        struct stat st;
        lstat(dircharlist[i],&st);
        char permisos[12];
        ConvierteModo(st.st_mode, permisos);

        char fecha[20]; //para cambiar la fecha de segundos y que no haga un salto de linea
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(fecha, sizeof(fecha), "%b %d %H:%M", tm_info);

        if (!strcmp(dircharlist[i],".") && hidtrue) {
        printf("%s %ld %ld %s %s\n", permisos, st.st_nlink, st.st_size, fecha, dircharlist[i]);
            //imprimimos info del directorio en long
        }

        //imprimimos info del directorio en long

        //ahora abrimos el directorio e imprimimos su interior, comprobando si hay hid o rec a/b
        if (st.st_mode & S_IFDIR) {

            DIR* dir = opendir(dircharlist[i]);
            struct dirent *entrada;
            while ((entrada = readdir(dir)) != NULL){
                //readdir itera las entradas del opendir
                //ESTO ES LONG
                //ahora a imprimir info de cada archivo
                //hay que coger la info del la entrada actual
                char ruta[500]; //para guardar el path
                snprintf(ruta, sizeof(ruta), "%s/%s", dircharlist[i], entrada->d_name);
                //guardamos en el el buffer ruta con snprint
                lstat(ruta, &st); //y ahora con la ruta actual hacemos el lstat
                ConvierteModo(st.st_mode, permisos);

                if (!hidtrue && entrada->d_name[0] == '.') continue; //si es nohid pasa de los archivos ocultos .*

                if (recursive == 1) { //si el flag de recursividad before está
                    if (S_ISDIR(st.st_mode)) {
                        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                            char* sublist[2];
                            sublist[0] = ruta;  // ruta completa del subdirectorio
                            sublist[1] = NULL;
                            condlong(sublist, linktrue, hidtrue, recursive); // llamada recursiva
                        }
                    }
                }

                char fechainside[20]; //para cambiar la fecha de segundos y que no haga un salto de linea
                struct tm *tm_infoinside = localtime(&st.st_mtime);
                strftime(fechainside, sizeof(fechainside), "%b %d %H:%M", tm_infoinside);

                struct passwd *pw = getpwuid(st.st_uid);
                struct group  *gr = getgrgid(st.st_gid);

                char *owner = pw ? pw->pw_name : "???";
                char *group = gr ? gr->gr_name : "???";         ///esto hace que cuando el usuario ni el grupo existen
                                                        //printee ???? como en el shell de referencia
                printf("%s:%s %s %ld %ld %s %s\n",owner,group, permisos, st.st_nlink, st.st_size, fechainside, entrada->d_name);

                if (linktrue && S_ISLNK(st.st_mode)) {
                    char destino[PATH_MAX];
                    ssize_t len = readlink(ruta, destino, sizeof(destino)-1);
                    if (len != -1) {
                        destino[len] = '\0';
                        printf("\t -> %s", destino);
                    }// si es un archivo y está Link hacer lo de link
                }

                if (recursive == -1) { //si el flag de recursividad after está
                    if (S_ISDIR(st.st_mode)) {
                        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                            char* sublist[2];
                            sublist[0] = ruta;  // ruta completa del subdirectorio
                            sublist[1] = NULL;
                            condlong(sublist, linktrue, hidtrue, recursive); // llamada recursiva
                        }
                    }
                }
            }
            closedir(dir);
            }

    }
}


void condshort(char* dircharlist[], int linktrue, int hidtrue, int recursive) {
    for (int i = 0; dircharlist[i]!=NULL; i++) {
        struct stat st;
        lstat(dircharlist[i],&st);

        if (!strcmp(dircharlist[i],".") && hidtrue) {
            printf("%ld %s\n",st.st_size,  dircharlist[i]);
            //imprimimos info del directorio en short
        }
        //ahora abrimos el directorio e imprimimos su interior, comprobando si hay hid o rec a/b
        if (st.st_mode & S_IFDIR) {

            DIR* dir = opendir(dircharlist[i]);
            struct dirent *entrada;
            while ((entrada = readdir(dir)) != NULL){
                //readdir itera las entradas del opendir
                //ESTO ES SHORT

                //ahora a imprimir info de cada archivo
                //hay que coger la info del la entrada actual
                char ruta[500]; //para guardar el path
                snprintf(ruta, sizeof(ruta), "%s/%s", dircharlist[i], entrada->d_name);
                //guardamos en el el buffer ruta con snprint
                lstat(ruta, &st); //y ahora con la ruta actual hacemos el lstat

                if (!hidtrue && entrada->d_name[0] == '.') continue; //si es nohid pasa de los archivos ocultos .*

                if (recursive == 1) { //si el flag de recursividad before está
                    if (S_ISDIR(st.st_mode)) {
                        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                            char* sublist[2];
                            sublist[0] = ruta;  // ruta completa del subdirectorio
                            sublist[1] = NULL;
                            condshort(sublist, linktrue, hidtrue, recursive); // llamada recursiva
                        }
                    }
                }

                printf("%ld %s\n",st.st_size,  entrada->d_name);

                if (linktrue && S_ISLNK(st.st_mode)) {
                    char destino[PATH_MAX];
                    ssize_t len = readlink(ruta, destino, sizeof(destino)-1);
                    if (len != -1) {
                        destino[len] = '\0';
                        printf("\t -> destino %s", destino);
                    }// si es un archivo y está Link hacer lo de link
                }
                if (recursive == -1) { //si el flag de recursividad after está
                    if (S_ISDIR(st.st_mode)) {
                        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                            char* sublist[2];
                            sublist[0] = ruta;  // ruta completa del subdirectorio
                            sublist[1] = NULL;
                            condshort(sublist, linktrue, hidtrue, recursive); // llamada recursiva
                        }
                    }
                }
            }
            closedir(dir);
            }

    }
}

void dir_cond(char* tokens[]) {
    char* dircharlist[10];
    int j = 0;
    for (int i=2; tokens[i]!=NULL; i++) {
        dircharlist[j]=tokens[i];       //guardamos el nombre del directorio / archivo
        j++;
    }
    dircharlist[j]=NULL;
    if (p.size) {
        condlong(dircharlist,p.link,p.hid,p.rec);
    } else {
        condshort(dircharlist,p.link,p.hid,p.rec);
    }

}

void cmd_dir(char* tokens[],tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1]==NULL) {
        //ls -l
        char* ls[] = { "dir","-d",".", NULL };
        dir_cond(ls);
    } else if (strcmp(tokens[1], "-d") != 0) {
        dir_sind(tokens);
    } else { //con flag -d
        dir_cond(tokens);
    }
} //

void cmd_erase(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {
        printf("erase: missing file or directory name\n");
        return;
    }

    for (int i = 1; tokens[i] != NULL; i++) {
        struct stat st;

        if (stat(tokens[i], &st) == -1) {
            perror(tokens[i]);
            continue;
        }

        char tipo = LetraTF(st.st_mode);
        if (tipo == '-' || tipo == 'l') { // Regular files o symbolic links
            if (unlink(tokens[i]) == -1) {
                perror(tokens[i]);
            } else {
                printf("erase: file %s deleted\n", tokens[i]);
            }
        } else if (tipo == 'd') { // Directory
            if (rmdir(tokens[i]) == -1) {
                perror(tokens[i]);
            } else {
                printf("erase: directory %s deleted\n", tokens[i]);
            }
        } else {
            printf("erase: %s is neither a regular file nor a directory\n", tokens[i]);
        }
    }
}

void cmd_delrec(char* tokens[], tList *L, tfList *FL, tMList *ML, tPList *PL, char **envp) {
    if (tokens[1] == NULL) {
        printf("delrec: missing file or directory name\n");
        return;
    }

    for (int i = 1; tokens[i] != NULL; i++) {
        char *path = tokens[i];
        struct stat st;

        if (lstat(path, &st) == -1) {
            perror(path);
            continue;
        }

        // Regular file o symbolic links --> usamos el cmd_erase
        if (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
            char *file_tokens[3] = { "erase", path, NULL };
            cmd_erase(file_tokens, L, FL, ML, PL, envp);
            continue;
        }

        if (!S_ISDIR(st.st_mode)) { // ni archivo ni directorio, no vaya a ser que alguien lo use en cierto path...
            printf("delrec: cannot delete %s: not file or directory\n", path);
            continue;
        }

        if (chdir(path) == -1) {
            perror(path);
            continue;
        }

        DIR *dir = opendir(".");
        if (dir) {
            struct dirent *ent;
            while ((ent = readdir(dir)) != NULL) {
                if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                    continue;

                char *rec_tokens[3] = { "delrec", ent->d_name, NULL }; // usamos el nombre relativo
                cmd_delrec(rec_tokens, L, FL, ML, PL, envp);
            }
            closedir(dir);
        }

        chdir("..");

        // Borramos el directorio (debería estar vacío)
        if (rmdir(path) == -1) {
            perror(path);
        } else {
            // info por pantalla para comprobar...
            char permisos[12];
            ConvierteModo(st.st_mode, permisos);
            printf("Deleted directory: %s [%s]\n", path, permisos);
        }
    }
}

void cmd_lseek(char* tokens[],tList *L, tfList *FL,  tMList *ML, tPList *PL, char **envp) {
    int fd,offset, ref;
    tfPosL p;
    char *restos;
    if (tokens[1]==NULL) {
        printf("Parametros no dados\n");
    }else {
        fd=strtol(tokens[1],&restos,10);
        if (*restos!='\0') {
            printf("Descriptor de fichero contiene caracteres\n");
            return;
        }
        p=FfindItem(fd,*FL);
        if (p==FLNULL) {
            printf("Descriptor de fichero no abierto");
            return;
        }
        if (*restos!='\0') {
            printf("Descriptor de fichero contiene caracteres\n");
            return;
        }
        offset=strtol(tokens[2],&restos,10);
        if (*restos!='\0') {
            printf("Offset contiene caracteres\n");
            return;
        }
        if (strcmp(tokens[3], "SEEK_SET") == 0) {
            ref = SEEK_SET;   // equivale a 0
        } else if (strcmp(tokens[3], "SEEK_CUR") == 0) {
            ref = SEEK_CUR;   // equivale a 1
        } else if (strcmp(tokens[3], "SEEK_END") == 0) {
            ref = SEEK_END;   // equivale a 2
        } else {
            printf("Referencia no válida. Usa SEEK_SET, SEEK_CUR o SEEK_END.\n");
            return;
        }
        lseek(fd,offset, ref);

    }
}

void cmd_writestr(char* tokens[],tList *L, tfList *FL,  tMList *ML, tPList *PL, char **envp) {
    int i, fd, n=strlen(tokens[2]);
    tfPosL p;
    tfItemL d;
    char *restos;
    if (tokens[1]==NULL) {
        printf("Parametros no dados\n");
    }else {
        fd=strtol(tokens[1],&restos,10);
        if (*restos!='\0') {
            printf("Descriptor de fichero contiene caracteres\n");
            return;
        }
        p=FfindItem(fd,*FL);
        d=FgetItem(p,*FL);
        if (p==FLNULL) {
            printf("Descriptor de fichero no abierto\n");
            return;
        }
        if ((d.flags & O_RDWR) == O_RDWR || (d.flags & O_RDONLY) == O_RDONLY) {
            for (i=2; tokens[i+1]!=LNULL;i++) {
                write(fd,tokens[i],n);
                write(fd," ",n);
            }
            write(fd,tokens[i],n);

        }else printf("Fichero abierto en modo read only\n");
    }
}

