/*****************************************************************/
/*         Tabela de Dispersao | PROG2 | MIEEC | 2020/21         */
/*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabdispersao.h"
#include "grafo.h"

tabela_dispersao *tabela_nova(int capacidade, hash_func *hfunc, sond_func *sfunc)
{
    if(!hfunc || !sfunc)
        return NULL;

    tabela_dispersao *td = (tabela_dispersao*) malloc(sizeof(tabela_dispersao));
    if(!td)
        return NULL;

    td->nos = (no_grafo**) calloc(capacidade, sizeof(no_grafo*));
    if(!td->nos)
    {
        free(td);
        return NULL;
    }

    td->estado_celulas = (int*) calloc(capacidade, sizeof(int));
    if(!td->estado_celulas)
    {
        free(td);
        free(td->nos);
        return NULL;
    }


    td->capacidade = capacidade;
    td->tamanho = 0;
    td->hfunc = hfunc;
    td->sfunc = sfunc;
    

    return td;
}

int tabela_adiciona(tabela_dispersao *td, no_grafo *entrada)
{
    if(!td || !entrada)
        return -1;

    if(td->capacidade <= td->tamanho)
        return -1;

    int k, index, estado, tentativas;
    
    index = tabela_existe(td, entrada->cidade);

    if(index == -1)
    {
        k = td->hfunc(entrada->cidade, td->capacidade);

        tentativas = 0;

        index = k;
        estado = td->estado_celulas[index];

        while(estado!=0)
        {
            tentativas++;
            index = td->sfunc(k, tentativas, td->capacidade);

            estado = td->estado_celulas[index];
        }

        td->nos[index] = entrada;
        td->estado_celulas[index] = 1;
        td->tamanho++;
        return index;
    }
    
    td->nos[index] = entrada;
    return index;
}

int tabela_remove(tabela_dispersao *td, no_grafo *saida)
{
    if(!td || !saida)
        return -1;

    int index;

    index = tabela_existe(td, saida->cidade);

    if(index == -1)
        return -1;

    td->nos[index] = NULL;
    td->estado_celulas[index] = -1;
    td->tamanho--;

    return 0;
}

void tabela_apaga(tabela_dispersao *td)
{
    if(!td)
        return;

    free(td->nos);
    td->nos = NULL;
    free(td->estado_celulas);
    td->estado_celulas = NULL;
    free(td);
    td = NULL;

    return;
}

int tabela_existe(tabela_dispersao *td, const char *cidade)
{
    if(!td || !cidade)
        return -1;
    
    no_grafo *aux;
    int k, index, estado, tentativas;

    k = td->hfunc(cidade, td->capacidade);
    index = k;
    estado = td->estado_celulas[index];

    aux = td->nos[index];
    tentativas=0;

    while(1)
    {
        if(tentativas == td->capacidade)
            return -1;

        if(estado == 0)
            return -1;
        
        if(aux != NULL)
            if(strcmp(aux->cidade, cidade)==0)
                return index;

        tentativas++;
        index = td->sfunc(k, tentativas, td->capacidade);

        aux = td->nos[index];
        estado = td->estado_celulas[index];
    }  
    
    return -1;
}

tabela_dispersao *tabela_carrega(grafo *g, int tamanho)
{
    if(!g || tamanho<1)
        return NULL;

    tabela_dispersao *td = tabela_nova(tamanho, hash_krm, sond_rh);
    if(!td)
        return NULL;


    for(int i=0; i<g->tamanho; i++)
    {
        tabela_adiciona(td, g->nos[i]);
    }

    return td;
}

/*================================================================================*/
unsigned long sond_rh(int pos, int tentativas, int tamanho)
{

    return (pos + tentativas * tentativas) % tamanho;
}

unsigned long hash_krm(const char *chave, int tamanho)
{
    int c, t = strlen(chave);
    unsigned long hash = 7;

    for (c = 0; c < t; c++)
    {
        hash += (int)chave[c];
    }

    return hash % tamanho;
}
