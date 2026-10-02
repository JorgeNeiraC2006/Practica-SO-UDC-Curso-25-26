
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#ifndef P0_PROC_LIST_H
#define P0_PROC_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PLNULL NULL

typedef struct tPNode *tPPosL;

typedef enum {
    ACTIVE,
    STOPPED,
    FINISHED,
    SIGNALED
} tEstado;

typedef struct {
    pid_t pid;         // descriptor
    uid_t uid;
    char* name;      // nombre del archivo
    tEstado estado;      // modo (tal como devuelve open())

    int exitcode;
    int signal;
    time_t starttime;
} tPItemL;


struct tPNode {
    tPItemL data;
    tPPosL next;
};

typedef tPPosL tPList;

void PcreateEmptyList(tPList *L);

/* {Tipo: Generadora.
    Objetivo: Crea una lista vacia y la inicializa.
    Entrada:
        L: Lista donde vamos a insertar.
    Salida: Una lista vacia.
    Poscondicion: La lista queda inicializada. } */

bool PisEmptyList(tPList L);

/* {Tipo: Observadora.
    Objetivo: Indicar si la lista esta vacia.
    Entrada:
        L: Lista que vamos a comprobar.
    Salida: Un booleano. } */

int PcountElements(tPList L);

/* {Tipo: Observadora.
    Objetivo: El nummero de elementos de la lista.
    Entrada:
        L: Lista que vamos a comprobar.
    Salida: Un entero. } */



tPPosL Pfirst(tPList L);

/* {Tipo: Observadora.
    Objetivo: Conocer la primera posicion de la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del primer elemento.
    Salida: La primera posicion util de la lista.
    Precondicion: La lista ha de estar inicializada} */

tPPosL Plast(tPList L);


/* {Tipo: Observadora.
    Objetivo: Conocer la ultima posicion de la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del ultimo elemento.
    Salida: La ultima posicion util de la lista.
    Precondicion: La lista ha de estar inicializada} */


tPPosL Pnext(tPPosL p, tPList L);


/* {Tipo: Observadora.
    Objetivo: Conocer la siguiente posicion de la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del siguiente elemento.
        p: Posicion de la cual queremos conocer la siguiente posicion.
    Salida: La siguiente posicion util de la lista (en caso de ser la posterior a la ultima posicion devuelve LNULL).
    Precondicion: La posicion debe de ser valida.} */


tPPosL Pprevious(tPPosL p, tPList L);

/* {Tipo: Observadora.
    Objetivo: Conocer la anterior posicion de la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del anterior elemento.
        p: Posicion de la cual queremos conocer la anterior posicion.
    Salida: La anterior posicion util de la lista (en caso de ser la anterior a la primera posicion devuelve LNULL).
    Precondicion: La posicion debe de ser valida.} */

void PupdateItem(tPItemL d, tPPosL p, tPList *L);

/* {Tipo: Modificadora.
    Objetivo: Modificar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a insertar el objeto.
        p: Posicion donde queremos insertar el objeto.
        d: Objeto a insertar en la lista.
    Salida: Lista modificada con el objeto nuevo incorrporado en la posicion indicada.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: La posicion de los objetos de la lista no se ve modificado.} */

bool PinsertItem(tPItemL d, tPPosL p, tPList *L);

/* {Tipo: Generadora.
    Objetivo: Modificar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a insertar el objeto.
        p: Posicion donde queremos insertar el objeto.
        d: Objeto a insertar en la lista.
    Salida: Un booleano indicando si pudo ser intertado o no el objeto.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.} */

void PdeleteAtPosition(tPPosL p, tPList *L);

/* {Tipo: Destructora.
    Objetivo: Eliminar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a eliminar la posicion.
        p: Posicion que queremos eliminar.
    Salida: La lista sin la posicion eliminada.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.} */


void PdeleteList( tPList *L);

tPItemL PgetItem(tPPosL p, tPList L);

/* {Tipo: Observadora.
    Objetivo: Obtener el objeto.
    Entrada:
        L: Lista de donde vamos a obtener el objeto.
        p: Posicion de donde queremos obtener el objeto.
    Salida: El objeto almacenado en la posicion indicada.
    Precondicion: La posicion debe de ser valida.} */

tPPosL PfindItem(int fd, tPList L);


/* {Tipo: Observadora.
    Objetivo: Encontrar la PRIMERA posicion donde se halla el objeto.
    Entrada:
        L: Lista donde vamos a buscar el objeto.
        c: Id del objeto a obtener la posicion en la lista.
    Salida: La posicion del PRIMER objeto con ese Id(En caso de no encontrarlo devuelve LNULL).} */


#endif
