/*****************************************************************/
/*    Estrutura nova a implementar | PROG2 | MIEEC | 2020/21     */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"
#include "math.h"

estrutura *st_nova()
{
    estrutura *st = (estrutura*) calloc(1, sizeof(estrutura));
    if(!st)
        return NULL; 

    st->tabelas = (tabela_dispersao**)  calloc(N, sizeof(tabela_dispersao*));
    st->tamanho = N;

    return st;
}

int st_importa_grafo(estrutura *st, grafo *g)
{    
    if(!st || !g)
        return -1;

    for(int i=0; i<g->tamanho; i++)
    {   
        st_insere(st, g->nos[i]);
    } 

    return 0;
}

char *st_pesquisa(estrutura *st, char *origem, char *destino)
{
    if(!st || !origem || !destino)
        return NULL;
    
    vetor vec;
    no_grafo *no;
    aresta_grafo *ag;
    char *codigo = NULL;
    int i, pos, k, preco = 10000;

    pos = origem[0]-65;
    k = st->tabelas[pos]->hfunc(origem, MAX_CAP);
    i = tabela2_existe(st->tabelas[pos], origem);

    if(i==-1)
        return NULL;

    vec =  st->tabelas[pos]->vetores[k];
    no = vec.nos[i];
    for(int j=0; j<no->tamanho; j++)
    {
        ag = no->arestas[j];
        if(strcmp(ag->destino->cidade, destino) == 0)
            if(ag->preco < preco)
            {
                codigo = ag->codigo;
                preco = ag->preco;
            }
    }
    
    return codigo;
}

int st_apaga(estrutura *st)
{
    if(!st)
        return -1;

    for(int i=0; i<N; i++)
    {
        tabela2_apaga(st->tabelas[i]);
    }

    st->tamanho = 0;
    free(st->tabelas);
    free(st); 
    st = NULL;  
    
    return 0;
}

void st_imprime(estrutura *st)
{
    if(!st)
        return;

    tabela_dispersao *td;
    vetor vec;
    no_grafo *no;
    aresta_grafo *ag;
    
    for(int c=0; c<N; c++)
    {   
        printf("LETRA [%c]\n", c+65);
        td = st->tabelas[c];
        if(!td)
        {
            printf("\n\tÍNDICE VAZIO\n");
            continue;
        }

        for(int i=0; i<td->tamanho; i++)
        {
            vec = td->vetores[i];
            if(vec.tamanho==0)
                continue;
            
            printf("\n\tÍNDICE [%d]\n", i);
            for(int j=0; j<vec.tamanho; j++)
            {
                printf("\t\t%s\n", vec.nos[j]->cidade);
            }
        }
    }
}

int st_insere(estrutura *st, no_grafo *no)
{
    if(!st || !no)
        return -1;
    
    int pos = no->cidade[0]-65;

    if(!st->tabelas[pos])
    {
        st->tabelas[pos] = tabela2_nova(hash2_krm);
        if(!st->tabelas[pos])
            return -1;
    }   

    tabela2_adiciona(st->tabelas[pos], no);

    return 0;
}

int st_remove(estrutura *st, no_grafo *no)
{
    if(!st || !no)
        return -1;
        
    int i, k;
    i = no->cidade[0]-65;

    if(tabela2_remove(st->tabelas[i], no)==0)
        return 0;
        
    return -1;
}