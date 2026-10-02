
/* 
 * Nombre            
 * Jorge Neira Cociña 
 *
 */

#include "mem_list.h"

#include <stdio.h>

bool McreateNode(tPosML *p) {
    *p = (tPosML) malloc(sizeof(struct tMNode));
    return *p != MLNULL;
}

void McreateEmptyList(tMList *L) {
    *L = MLNULL;
}

bool MisEmptyList(tMList L) {
    return L == MLNULL;
}


int McountElements(tMList L){
    int count = 0;
        for(tPosML p = Mfirst(L); p!=MLNULL; p=p->next ) count++;
    return --count;
}

tPosML Mfirst(tMList L) {
    return L;
}

tPosML Mlast(tMList L) {
    tPosML p = L;
    while (p->next != NULL) {
        p = p->next;
    }
    return p;
}

tPosML Mnext(tPosML p, tMList L) {
    return p->next;
}

tPosML Mprevious(tPosML p, tMList L) {
    tPosML pAux = L;
    if (p == L) {
        pAux = MLNULL;
    } else {
        while (pAux->next != p) {
            pAux = pAux->next;
        }
    }
    return pAux;
}

void MupdateItem(tItemML d, tPosML p, tMList *L) {
    p->data = d;
}

bool MinsertItem(tItemML d, tPosML p, tMList *L) {
    bool aux = true;
    tPosML q;
    if (!McreateNode(&q)) {
        // Si el nodo no se pudo crear devuelve false
        aux = false;
    } else {
        q->data = d;
        q->next = MLNULL;
        if (MisEmptyList(*L)) {
            // Si la lista esta vacia crea el nodo y lo añade manualmente
            *L = q;
        } else {
            if (p == MLNULL) {
                // Lo añade al final
                tPosML pAux = Mlast(*L);
                pAux->next = q;
            } else {
                // En lugar de hallar el anterior de p, lo añadimos despues e intercambiamos informacion entre ellos
                tItemML auxItem;
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

void MdeleteAtPosition(tPosML p, tMList *L) {
    tPosML q = p;
    tItemML d;
    if (p == *L) {
        *L = p->next;
    } else {
        if (p->next == MLNULL) {
            // Va al ultimo elemento y lo borras
            Mprevious(p, *L)->next = MLNULL;
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

void MdeleteList(tMList *L){
    tPosML p;
    while (*L != NULL) {
        p=Mfirst(*L);
        free(p->data.memDir);
        *L = p->next;
        free(p);
    }
}


tItemML MgetItem(tPosML p, tMList L) {
    return p->data;
}


bool sameObject(tItemML a, tItemML b) {
    bool aux;
    aux=a.key==b.key;
    aux=aux&&a.memDir==b.memDir;
    aux=aux&&a.type==b.type;
    aux=aux&&a.size==b.size;
    aux=aux&&strcmp(a.memDir,b.memDir)==0;


    return aux;
}

tPosML MfindItem(tItemML d, tMList L) {
    tPosML p;
    tItemML aux;
    if (MisEmptyList(L)) {
        p = MLNULL;
    } else {
        p = Mfirst(L);
        aux=MgetItem(p,L);
        for (p = L; (p != MLNULL) && sameObject(aux,d); p = p->next) {
        MgetItem(p,L);
        }
    }
    return p;
}

tPosML findMem(size_t size, tMList L) {
    for (tPosML p = L; p != MLNULL; p = p->next) {
        if (p->data.size == size && p->data.type == 0)
            return p;
    }
    return MLNULL;
}

tPosML findAddress(void *addr, tMList L) {
    for (tPosML p = L; p != MLNULL; p = p->next) {
        if (p->data.memDir == addr)
            return p;
    }
    return MLNULL;
}

tPosML findKey(key_t key, tMList L) {
    for (tPosML p = L; p != MLNULL; p = p->next) {
        if (p->data.type == 1 && p->data.key == key)
            return p;
    }
    return MLNULL;
}

void liberarMemoria(tMList *ML) {
    tPosML p = Mfirst(*ML);
    while (p != MLNULL) {
        tItemML it = MgetItem(p, *ML);

        if (it.type == 0) {
            free(it.memDir);
        }
        else if (it.type == 1) {
            shmdt(it.memDir);
        }
        else if (it.type == 2) {
            munmap(it.memDir, it.size);
            if (it.fd >= 0) close(it.fd);
        }

        if (it.name) free(it.name);

        tPosML next = Mnext(p, *ML);
        MdeleteAtPosition(p, ML);
        p = next;
    }
}

