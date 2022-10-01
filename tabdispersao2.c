#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabdispersao2.h"
#include "grafo.h"

tabela_dispersao *tabela2_nova(hash_func *hfunc)
{
    if(!hfunc)
        return NULL;

    tabela_dispersao *td = (tabela_dispersao*) malloc(sizeof(tabela_dispersao));
    if(!td)
        return NULL;

    td->hfunc = hfunc;
    td->tamanho = MAX_CAP;
    
    td->vetores = (vetor*) calloc(MAX_CAP, sizeof(vetor));
    
    if(!td->vetores)
    {
        free(td);
        return NULL;
    }

    return td;
}

int tabela2_adiciona(tabela_dispersao *td, no_grafo *entrada)
{
    if(!td || !entrada)
        return -1;

    int k, i;

    k = td->hfunc(entrada->cidade, td->tamanho);
    i = tabela2_existe(td, entrada->cidade);

    if(i==-1)
    {
        td->vetores[k].tamanho++;
        int tamanho = td->vetores[k].tamanho;

        td->vetores[k].nos = (no_grafo**) realloc(td->vetores[k].nos, tamanho*sizeof(no_grafo*));
        td->vetores[k].nos[tamanho-1] = entrada;

        return tamanho-1;
    }
    td->vetores[k].nos[i] = entrada;
    
    return i;
}

int tabela2_remove(tabela_dispersao *td, no_grafo *saida)
{
    if(!td || !saida)
        return -1;

    int i, k, tamanho;

    i = tabela2_existe(td, saida->cidade);
    
    if(i==-1)
        return -1;

    k = td->hfunc(saida->cidade, td->tamanho);

    td->vetores[k].tamanho--;
    tamanho = td->vetores[k].tamanho;
    td->vetores[k].nos[i] = td->vetores[k].nos[tamanho]; 
    td->vetores[k].nos[tamanho] = NULL;

    
    td->vetores[k].nos = (no_grafo**) realloc(td->vetores[k].nos, tamanho*sizeof(no_grafo*));
    

    return 0;
}

void tabela2_apaga(tabela_dispersao *td)
{
    if(!td)
        return;

    vetor vec;
    int tamanho;

    for(int i=0; i<MAX_CAP; i++)
    {
        free(td->vetores[i].nos);
    }
    
    td->tamanho = 0;
    free(td->vetores);
    free(td);

    return;
}

int tabela2_existe(tabela_dispersao *td, const char *cidade)
{
    if(!td || !cidade)
        return -1;

    no_grafo *no;
    int k;

    k = td->hfunc(cidade, td->tamanho);

    for(int i=0; i<td->vetores[k].tamanho; i++)
    {
        if(strcmp(td->vetores[k].nos[i]->cidade, cidade) == 0) return i;
    }

    return -1;
}

/*================================================================================*/
unsigned long hash2_krm(const char *chave, int tamanho)
{
    int c, t = strlen(chave);
    unsigned long hash = 7;

    for (c = 0; c < t; c++)
    {
        hash += (int)chave[c];
    }

    return hash % tamanho;
}