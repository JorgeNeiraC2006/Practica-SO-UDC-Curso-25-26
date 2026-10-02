
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#include "file_list.h"

#include <stdio.h>

bool FcreateNode(tfPosL *p) {
    *p = (tfPosL) malloc(sizeof(struct tfNode));
    return *p != FLNULL;
}

void FcreateEmptyList(tfList *L) {
    *L = FLNULL;
}

bool FisEmptyList(tfList L) {
    return L == FLNULL;
}


int FcountElements(tfList L){
    int count = 0;
        for(tfPosL p = Ffirst(L); p!=FLNULL; p=p->next ) count++;
    return --count;
}

tfPosL Ffirst(tfList L) {
    return L;
}

tfPosL Flast(tfList L) {
    tfPosL p = L;
    while (p->next != NULL) {
        p = p->next;
    }
    return p;
}

tfPosL Fnext(tfPosL p, tfList L) {
    return p->next;
}

tfPosL Fprevious(tfPosL p, tfList L) {
    tfPosL pAux = L;
    if (p == L) {
        pAux = FLNULL;
    } else {
        while (pAux->next != p) {
            pAux = pAux->next;
        }
    }
    return pAux;
}

void FupdateItem(tfItemL d, tfPosL p, tfList *L) {
    p->data = d;
}

bool FinsertItem(tfItemL d, tfPosL p, tfList *L) {
    bool aux = true;
    tfPosL q;
    if (!FcreateNode(&q)) {
        // Si el nodo no se pudo crear devuelve false
        aux = false;
    } else {
        q->data = d;
        q->next = FLNULL;
        if (FisEmptyList(*L)) {
            // Si la lista esta vacia crea el nodo y lo añade manualmente
            *L = q;
        } else {
            if (p == FLNULL) {
                // Lo añade al final
                tfPosL pAux = Flast(*L);
                pAux->next = q;
            } else {
                // En lugar de hallar el anterior de p, lo añadimos despues e intercambiamos informacion entre ellos
                tfItemL auxItem;
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

void FdeleteAtPosition(tfPosL p, tfList *L) {
    tfPosL q = p;
    tfItemL d;
    if (p == *L) {
        *L = p->next;
    } else {
        if (p->next == FLNULL) {
            // Va al ultimo elemento y lo borras
            Fprevious(p, *L)->next = FLNULL;
        } else {
            // En lugar de hallar el anterior de p, lo añadimos despues e intercambiamos informacion entre ellos para poder borrarlos
            p = p->next;
            d = p->data;
            p->data = q->data;
            q->data = d;
            q->next = p->next;
        }
    }
    free(p);
}


void FdeleteList(tfList *L){
    tfPosL p;
    while (*L != NULL) {
        p=Ffirst(*L);
        *L = p->next;
        free(p);
    }
}


tfItemL FgetItem(tfPosL p, tfList L) {
    return p->data;
}

tfPosL FfindItem(int fd, tfList L) {
    tfPosL p;
    if (FisEmptyList(L)) {
        p = FLNULL;
    } else {
        for (p = L; (p != FLNULL) && (p->data.fd != fd);p=Fnext(p,L)){

        }
            //compara si es el ultimo o lo encontro
    }
    return p;
}
