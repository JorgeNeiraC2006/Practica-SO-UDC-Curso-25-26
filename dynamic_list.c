
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#include "dynamic_list.h"

#include <stdio.h>

bool createNode(tPosL *p) {
    *p = (tPosL) malloc(sizeof(struct tNode));
    return *p != LNULL;
}

void createEmptyList(tList *L) {
    *L = LNULL;
}

bool isEmptyList(tList L) {
    return L == LNULL;
}


int countElements(tList L){
    int count = 0;
        for(tPosL p=first(L); p!=LNULL; p=p->next ) count++;
    return --count;
}

tPosL first(tList L) {
    return L;
}

tPosL last(tList L) {
    tPosL p = L;
    while (p->next != NULL) {
        p = p->next;
    }
    return p;
}

tPosL next(tPosL p, tList L) {
    return p->next;
}

tPosL previous(tPosL p, tList L) {
    tPosL pAux = L;
    if (p == L) {
        pAux = LNULL;
    } else {
        while (pAux->next != p) {
            pAux = pAux->next;
        }
    }
    return pAux;
}

void updateItem(tItemL d, tPosL p, tList *L) {
    p->data = d;
}

bool insertItem(tItemL d, tPosL p, tList *L) {
    bool aux = true;
    tPosL q;
    if (!createNode(&q)) {
        // Si el nodo no se pudo crear devuelve false
        aux = false;
    } else {
        q->data = strdup(d);
        q->next = LNULL;
        if (isEmptyList(*L)) {
            // Si la lista esta vacia crea el nodo y lo añade manualmente
            *L = q;
        } else {
            if (p == LNULL) {
                // Lo añade al final
                tPosL pAux = last(*L);
                pAux->next = q;
            } else {
                // En lugar de hallar el anterior de p, lo añadimos despues e intercambiamos informacion entre ellos
                tItemL auxItem;
                q->next = p->next;
                auxItem = p->data;
                p->data = q->data;
                q->data = auxItem;
                p->next = q;
            }
        }
    }
    return aux;
}

void deleteAtPosition(tPosL p, tList *L) {
    tPosL q = p;
    tItemL d;
    if (p == *L) {
        *L = p->next;
    } else {
        if (p->next == LNULL) {
            // Va al ultimo elemento y lo borras
            previous(p, *L)->next = LNULL;
        } else {
            // En lugar de hallar el anterior de p, lo añadimos despues e intercambiamos informacion entre ellos para poder borrarlos
            p = p->next;
            d = p->data;
            p->data = q->data;
            q->data = d;
            q->next = p->next;
        }
    }

    free(p->data);
    free(p);
}

void deleteList(tList *L) {
    tPosL p;
    while (*L != NULL) {
        p = *L;
        *L = p->next;
        free(p->data);
        free(p);
    }
}


tItemL getItem(tPosL p, tList L) {
    return p->data;
}

tPosL findItem(char* c, tList L) {
    tPosL p;
    if (isEmptyList(L)) {
        p = LNULL;
    } else {
        for (p = L; (p != LNULL) && strcmp(p->data, c) != 0; p = p->next) {//compara si es el ultimo o lo encontro
        }
    }
    return p;
}

