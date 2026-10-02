
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */


#include "proc_list.h"

#include <stdio.h>

bool PcreateNode(tPPosL *p) {
    *p = (tPPosL) malloc(sizeof(struct tPNode));
    return *p != PLNULL;
}

void PcreateEmptyList(tPList *L) {
    *L = PLNULL;
}

bool PisEmptyList(tPList L) {
    return L == PLNULL;
}


int PcountElements(tPList L){
    int count = 0;
        for(tPPosL p = Pfirst(L); p!=PLNULL; p=p->next ) count++;
    return --count;
}

tPPosL Pfirst(tPList L) {
    return L;
}

tPPosL Plast(tPList L) {
    tPPosL p = L;
    while (p->next != NULL) {
        p = p->next;
    }
    return p;
}

tPPosL Pnext(tPPosL p, tPList L) {
    return p->next;
}

tPPosL Pprevious(tPPosL p, tPList L) {
    tPPosL pAux = L;
    if (p == L) {
        pAux = PLNULL;
    } else {
        while (pAux->next != p) {
            pAux = pAux->next;
        }
    }
    return pAux;
}

void PupdateItem(tPItemL d, tPPosL p, tPList *L) {
    p->data = d;
}

bool PinsertItem(tPItemL d, tPPosL p, tPList *L) {
    bool aux = true;
    tPPosL q;
    if (!PcreateNode(&q)) {
        // Si el nodo no se pudo crear devuelve false
        aux = false;
    } else {
        q->data = d;
        q->next = PLNULL;
        if (PisEmptyList(*L)) {
            // Si la lista esta vacia crea el nodo y lo añade manualmente
            *L = q;
        } else {
            if (p == PLNULL) {
                // Lo añade al final
                tPPosL pAux = Plast(*L);
                pAux->next = q;
            } else {
                // En lugar de hallar el anterior de p, lo añadimos despues e intercambiamos informacion entre ellos
                tPItemL auxItem;
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

void PdeleteAtPosition(tPPosL p, tPList *L) {
    tPPosL q = p;
    tPItemL d;
    if (p == *L) {
        *L = p->next;
    } else {
        if (p->next == PLNULL) {
            // Va al ultimo elemento y lo borras
            Pprevious(p, *L)->next = PLNULL;
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


void PdeleteList(tPList *L){
    tPPosL p;
    while (*L != NULL) {
        p=Pfirst(*L);
        *L = p->next;
        free(p);
    }
}


tPItemL PgetItem(tPPosL p, tPList L) {
    return p->data;
}

tPPosL PfindItem(pid_t pid, tPList L) {
    tPPosL p;
    if (PisEmptyList(L)) {
        p = PLNULL;
    } else {
        for (p = L; (p != PLNULL) && (p->data.pid != pid);p=Pnext(p,L)){

        }
            //compara si es el ultimo o lo encontro
    }
    return p;
}
