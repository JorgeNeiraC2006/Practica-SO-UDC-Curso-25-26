
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#ifndef MEM_LIST_H
#define MEM_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>


#define MLNULL NULL

typedef struct tMNode *tPosML;

typedef struct tItemML {
    void* memDir;
    size_t size;
    struct tm time;
    int type; //malloc memory=0, shared memory=1, mapped file=2
    key_t key;
    int fd;
    char* name;
}tItemML;


struct tMNode {
    tItemML data;
    tPosML next;
};

typedef tPosML tMList;

void McreateEmptyList(tMList *L);

/* {Tipo: Generadora.
    Objetivo: Crea una lista vacia y la inicializa.
    Entrada:
        L: Lista donde vamos a insertar.
    Salida: Una lista vacia.
    Poscondicion: La lista queda inicializada. } */

bool MisEmptyList(tMList L);

/* {Tipo: Observadora.
    Objetivo: Indicar si la lista esta vacia.
    Entrada:
        L: Lista que vamos a comprobar.
    Salida: Un booleano. } */

int McountElements(tMList L);

/* {Tipo: Observadora.
    Objetivo: El nummero de elementos de la lista.
    Entrada:
        L: Lista que vamos a comprobar.
    Salida: Un entero. } */



tPosML Mfirst(tMList L);

/* {Tipo: Observadora.
    Objetivo: Conocer la primera posicion de la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del primer elemento.
    Salida: La primera posicion util de la lista.
    Precondicion: La lista ha de estar inicializada} */

tPosML Mlast(tMList L);


/* {Tipo: Observadora.
    Objetivo: Conocer la ultima posicion de la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del ultimo elemento.
    Salida: La ultima posicion util de la lista.
    Precondicion: La lista ha de estar inicializada} */


tPosML Mnext(tPosML p, tMList L);


/* {Tipo: Observadora.
    Objetivo: Conocer la siguiente posicion de la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del siguiente elemento.
        p: Posicion de la cual queremos conocer la siguiente posicion.
    Salida: La siguiente posicion util de la lista (en caso de ser la posterior a la ultima posicion devuelve LNULL).
    Precondicion: La posicion debe de ser valida.} */


tPosML Mprevious(tPosML p, tMList L);

/* {Tipo: Observadora.
    Objetivo: Conocer la anterior posicion de la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del anterior elemento.
        p: Posicion de la cual queremos conocer la anterior posicion.
    Salida: La anterior posicion util de la lista (en caso de ser la anterior a la primera posicion devuelve LNULL).
    Precondicion: La posicion debe de ser valida.} */

void MupdateItem(tItemML d, tPosML p, tMList *L);

/* {Tipo: Modificadora.
    Objetivo: Modificar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a insertar el objeto.
        p: Posicion donde queremos insertar el objeto.
        d: Objeto a insertar en la lista.
    Salida: Lista modificada con el objeto nuevo incorrporado en la posicion indicada.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: La posicion de los objetos de la lista no se ve modificado.} */

bool MinsertItem(tItemML d, tPosML p, tMList *L);

/* {Tipo: Generadora.
    Objetivo: Modificar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a insertar el objeto.
        p: Posicion donde queremos insertar el objeto.
        d: Objeto a insertar en la lista.
    Salida: Un booleano indicando si pudo ser intertado o no el objeto.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.} */

void MdeleteAtPosition(tPosML p, tMList *L);

/* {Tipo: Destructora.
    Objetivo: Eliminar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a eliminar la posicion.
        p: Posicion que queremos eliminar.
    Salida: La lista sin la posicion eliminada.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.} */


void MdeleteList( tMList *L);

tItemML MgetItem(tPosML p, tMList L);

/* {Tipo: Observadora.
    Objetivo: Obtener el objeto.
    Entrada:
        L: Lista de donde vamos a obtener el objeto.
        p: Posicion de donde queremos obtener el objeto.
    Salida: El objeto almacenado en la posicion indicada.
    Precondicion: La posicion debe de ser valida.} */

tPosML MfindItem(tItemML d, tMList L);

tPosML findMem(size_t size, tMList L);

tPosML findAddress(void* adress, tMList L);

tPosML findKey(key_t key, tMList L);

/* {Tipo: Observadora.
    Objetivo: Encontrar la PRIMERA posicion donde se halla el objeto.
    Entrada:
        L: Lista donde vamos a buscar el objeto.
        c: Id del objeto a obtener la posicion en la lista.
    Salida: La posicion del PRIMER objeto con ese Id(En caso de no encontrarlo devuelve LNULL).} */

void liberarMemoria(tMList *ML);

#endif
