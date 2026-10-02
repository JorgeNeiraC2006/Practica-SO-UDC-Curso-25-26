
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#ifndef P0_FILE_LIST_H
#define P0_FILE_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FLNULL NULL

typedef struct tfNode *tfPosL;
typedef struct {
    int fd;         // descriptor
    char* filename; // nombre del archivo
    int flags;      // modo (tal como devuelve open())
} tfItemL;


struct tfNode {
    tfItemL data;
    tfPosL next;
};

typedef tfPosL tfList;

void FcreateEmptyList(tfList *L);

/* {Tipo: Generadora.
    Objetivo: Crea una lista vacia y la inicializa.
    Entrada:
        L: Lista donde vamos a insertar.
    Salida: Una lista vacia.
    Poscondicion: La lista queda inicializada. } */

bool FisEmptyList(tfList L);

/* {Tipo: Observadora.
    Objetivo: Indicar si la lista esta vacia.
    Entrada:
        L: Lista que vamos a comprobar.
    Salida: Un booleano. } */

int FcountElements(tfList L);

/* {Tipo: Observadora.
    Objetivo: El nummero de elementos de la lista.
    Entrada:
        L: Lista que vamos a comprobar.
    Salida: Un entero. } */



tfPosL Ffirst(tfList L);

/* {Tipo: Observadora.
    Objetivo: Conocer la primera posicion de la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del primer elemento.
    Salida: La primera posicion util de la lista.
    Precondicion: La lista ha de estar inicializada} */

tfPosL Flast(tfList L);


/* {Tipo: Observadora.
    Objetivo: Conocer la ultima posicion de la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del ultimo elemento.
    Salida: La ultima posicion util de la lista.
    Precondicion: La lista ha de estar inicializada} */


tfPosL Fnext(tfPosL p, tfList L);


/* {Tipo: Observadora.
    Objetivo: Conocer la siguiente posicion de la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del siguiente elemento.
        p: Posicion de la cual queremos conocer la siguiente posicion.
    Salida: La siguiente posicion util de la lista (en caso de ser la posterior a la ultima posicion devuelve LNULL).
    Precondicion: La posicion debe de ser valida.} */


tfPosL Fprevious(tfPosL p, tfList L);

/* {Tipo: Observadora.
    Objetivo: Conocer la anterior posicion de la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a mirar la posicion del anterior elemento.
        p: Posicion de la cual queremos conocer la anterior posicion.
    Salida: La anterior posicion util de la lista (en caso de ser la anterior a la primera posicion devuelve LNULL).
    Precondicion: La posicion debe de ser valida.} */

void FupdateItem(tfItemL d, tfPosL p, tfList *L);

/* {Tipo: Modificadora.
    Objetivo: Modificar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a insertar el objeto.
        p: Posicion donde queremos insertar el objeto.
        d: Objeto a insertar en la lista.
    Salida: Lista modificada con el objeto nuevo incorrporado en la posicion indicada.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: La posicion de los objetos de la lista no se ve modificado.} */

bool FinsertItem(tfItemL d, tfPosL p, tfList *L);

/* {Tipo: Generadora.
    Objetivo: Modificar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a insertar el objeto.
        p: Posicion donde queremos insertar el objeto.
        d: Objeto a insertar en la lista.
    Salida: Un booleano indicando si pudo ser intertado o no el objeto.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.} */

void FdeleteAtPosition(tfPosL p, tfList *L);

/* {Tipo: Destructora.
    Objetivo: Eliminar el contenido la posicion que nos pasen en la lista.
    Entrada:
        L: Lista donde vamos a eliminar la posicion.
        p: Posicion que queremos eliminar.
    Salida: La lista sin la posicion eliminada.
    Precondicion: La posicion debe de ser valida.
    Postcondicion: Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.} */


void FdeleteList( tfList *L);

tfItemL FgetItem(tfPosL p, tfList L);

/* {Tipo: Observadora.
    Objetivo: Obtener el objeto.
    Entrada:
        L: Lista de donde vamos a obtener el objeto.
        p: Posicion de donde queremos obtener el objeto.
    Salida: El objeto almacenado en la posicion indicada.
    Precondicion: La posicion debe de ser valida.} */

tfPosL FfindItem(int fd, tfList L);


/* {Tipo: Observadora.
    Objetivo: Encontrar la PRIMERA posicion donde se halla el objeto.
    Entrada:
        L: Lista donde vamos a buscar el objeto.
        c: Id del objeto a obtener la posicion en la lista.
    Salida: La posicion del PRIMER objeto con ese Id(En caso de no encontrarlo devuelve LNULL).} */


#endif
