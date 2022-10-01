/*****************************************************************/
/*          Grafo direcionado | PROG2 | MIEEC | 2020/21          */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "grafo.h"
#include "heap.h"

grafo *grafo_novo()
{
    grafo *g = (grafo *)malloc(sizeof(grafo));
    g->tamanho = 0;
    g->nos = NULL;

    return g;
}

no_grafo *encontra_no(grafo *g, char *cidade)
{
    if (!g || !cidade)
        return NULL;

    // pesquisa por cidade no vetor de nós
    for (int i = 0; i < g->tamanho; i++)
    {
        if (strcmp(g->nos[i]->cidade, cidade) == 0)
            return g->nos[i];
    }
    return NULL;
}

no_grafo *no_insere(grafo *g, char *cidade)
{
    if (!g || !cidade)
        return NULL;

    // verifica se o nó já existe
    no_grafo *no = encontra_no(g, cidade);
    if (no)
        return NULL;

    // cria o novo nó para o user
    no = (no_grafo *)malloc(sizeof(no_grafo));
    if (!no)
        return NULL;

    // aloca espaço para o campo cidade
    no->cidade = (char *)malloc((strlen(cidade) + 1) * sizeof(char));
    if (!no->cidade)
    {
        free(no);
        return NULL;
    }
    // inicializa todos os campos
    strcpy(no->cidade, cidade);
    no->tamanho = 0;
    no->arestas = NULL;
    no->p_acumulado = 0;
    no->anterior = NULL;
    no->dataatualizada = NULL;

    // insere o nó criado no final do vetor de nós
    g->nos = (no_grafo **)realloc(g->nos, (g->tamanho + 1) * sizeof(no_grafo *));
    if (!g->nos)
    {
        free(no->cidade);
        free(no);
        return NULL;
    }
    // incrementa o tamanho do numero de nós e insere no vetor de apontadores de nós
    g->tamanho++;
    g->nos[g->tamanho - 1] = no;

    return no;
}

// função auxiliar que retorna 1 se existir a aresta para destino ou 0,
// se a aresta não existir, -1 se der erro
int existe_aresta(no_grafo *origem, no_grafo *destino, char *codigo)
{

    if (!origem || !destino)
        return -1;

    //pesquisa em todas as arestas se existe
    for (int i = 0; i < origem->tamanho; i++)
    {

        if ((origem->arestas[i]->destino == destino) && ((strcmp(origem->arestas[i]->codigo, codigo) == 0)))
            return 1;
    }

    return 0;
}

int cria_aresta(no_grafo *origem, no_grafo *destino, char *codigo, char *companhia, data partida, data chegada, float preco, int lugares)
{
    if (!origem || !destino || !codigo || !companhia)
        return -1;

    if (preco < 0 || lugares < 0)
        return -1;

    // verifica se a ligação já existe
    if (existe_aresta(origem, destino, codigo) > 0)
        return -1;

    // cria a nova ligação
    aresta_grafo *ag = (aresta_grafo *)malloc(sizeof(aresta_grafo));
    if (!ag)
        return -1;

    ag->destino = destino;
    ag->preco = preco;
    ag->lugares = lugares;
    // aloca espaço para o código
    ag->codigo = (char *)malloc((strlen(codigo) + 1) * sizeof(char));
    if (!ag->codigo)
    {
        free(ag);
        return -1;
    }
    strcpy(ag->codigo, codigo);
    // aloca espaço para o companhia
    ag->companhia = (char *)malloc((strlen(companhia) + 1) * sizeof(char));
    if (!ag->companhia)
    {
        free(ag->codigo);
        free(ag);
        return -1;
    }
    strcpy(ag->companhia, companhia);

    // inicializa todos os campos
    ag->partida.tm_year = partida.tm_year;
    ag->partida.tm_mon = partida.tm_mon;
    ag->partida.tm_mday = partida.tm_mday;
    ag->partida.tm_hour = partida.tm_hour;
    ag->partida.tm_min = partida.tm_min;
    ag->partida.tm_sec = 0;
    ag->partida.tm_isdst = 0;

    // inicializa todos os campos
    ag->chegada.tm_year = chegada.tm_year;
    ag->chegada.tm_mon = chegada.tm_mon;
    ag->chegada.tm_mday = chegada.tm_mday;
    ag->chegada.tm_hour = chegada.tm_hour;
    ag->chegada.tm_min = chegada.tm_min;
    ag->chegada.tm_sec = 0;
    ag->chegada.tm_isdst = 0;

    // insere a nova ligação no vetor de ligações
    origem->arestas = (aresta_grafo **)realloc(origem->arestas, (origem->tamanho + 1) * sizeof(aresta_grafo *));
    if (!origem->arestas)
    {
        free(ag->codigo);
        free(ag->companhia);
        free(ag);
        return -1;
    }
    origem->tamanho++;
    origem->arestas[origem->tamanho - 1] = ag;

    return 0;
}

no_grafo *no_remove(grafo *g, char *cidade)
{
    if(!g || !cidade)
        return NULL;
    
    
    no_grafo *no = encontra_no(g, cidade);
    if(!no)
        return NULL;

    for(int i=0; i<g->tamanho; i++)
    {
        no_grafo *aux = g->nos[i];

        for(int j=0; j<aux->tamanho; j++)
        {
            aresta_grafo *ag = aux->arestas[j];

            if(strcmp(cidade, ag->destino->cidade)==0)
            {
                aresta_apaga(ag);   

                for(int k=j; k<aux->tamanho-1; k++)
                {
                    aux->arestas[k]=aux->arestas[k+1];
                }
                aux->tamanho--;
                j--;

                aux->arestas = (aresta_grafo**) realloc(aux->arestas, (aux->tamanho) * sizeof(aresta_grafo *));
                if(!aux->arestas && aux->tamanho!=0)
                    return NULL;
            }
        }
    }
    
    for(int i=0; i<g->tamanho-1; i++)
    {
        if(strcmp(g->nos[i]->cidade, cidade)==0)
        {
            for(int j=i; j<g->tamanho-1; j++)
            {
                g->nos[j]=g->nos[j+1];
            }
            g->tamanho--;
            i--;
                        
            g->nos = (no_grafo**) realloc(g->nos, (g->tamanho) * sizeof(no_grafo*));
            if(!g->nos && g->tamanho!=0)
                return NULL;

            break;
        }
    }    

    return no;
}

int aresta_apaga(aresta_grafo *aresta)
{
    if(!aresta)
        return -1;

    free(aresta->codigo);
    aresta->codigo = NULL;

    free(aresta->companhia);
    aresta->companhia = NULL;

    free(aresta);
    aresta = NULL;

    return 0;
}

int no_apaga(no_grafo *no)
{
    if(!no)
        return -1;

    free(no->cidade);
    no->cidade = NULL;

    free(no->dataatualizada);
    no->dataatualizada = NULL;

    int tam = no->tamanho;
    for(int i=0; i<tam; i++)
    {
        aresta_apaga(no->arestas[i]);
        no->tamanho--;
    }

    free(no->arestas);
    no->arestas = NULL;
    free(no);
    no = NULL;
    
    return 0;
}

void grafo_apaga(grafo *g)
{
    if(!g)
        return;
    
    int tam = g->tamanho;
    for(int i=0; i<tam; i++)
    {
        no_apaga(g->nos[i]);
        g->tamanho--;
    }

    free(g->nos);
    g->nos = NULL;
    free(g);
    g = NULL;

    return;
}

no_grafo *encontra_voo(grafo *g, char *codigo, int *aresta_pos)
{
    if(!g || !codigo || !aresta_pos)
        return NULL;

    for(int i=0; i<g->tamanho; i++)
    {
        no_grafo *curr = g->nos[i];

        for(int j=0; j<curr->tamanho; j++)
        {
            if(strcmp(codigo, curr->arestas[j]->codigo)==0)
            {
                *aresta_pos=j;
                return curr;
            }
        }
    }
    
    return NULL;
}

no_grafo **pesquisa_avancada(grafo *g, char *destino, data chegada, double preco_max, int *n)
{
    if(!g || !destino || !n)
        return NULL;

    if(preco_max < 0)
        return NULL;

    no_grafo **vec = NULL;
    int tamanho=0;

    for(int i=0; i<g->tamanho; i++)
    {
        no_grafo *curr = g->nos[i];
        
        for(int j=0; j<curr->tamanho;j++)
        {
            aresta_grafo *ag = curr->arestas[j];
            if(strcmp(destino, ag->destino->cidade) == 0 && ag->preco <= preco_max && ag->chegada.tm_year == chegada.tm_year && ag->chegada.tm_mon == chegada.tm_mon && ag->chegada.tm_mday == chegada.tm_mday)
            {
                tamanho++;
                vec = (no_grafo**) realloc(vec, tamanho*sizeof(no_grafo*));
                if(!vec)
                    return NULL;

                vec[tamanho-1]=curr;
                break;
            }
        }
    }

    *n=tamanho;

    return vec;
}

no_grafo **trajeto_mais_rapido(grafo *g, char *origem, char *destino, data partida, int *n)
{
    if(!g || !origem || !destino || !n)
        return NULL;

    no_grafo **vec=NULL;

    time_t t_partida = mktime(&partida);

    no_grafo *no_origem = encontra_no(g, origem);
    if(!no_origem)
        return NULL;

    no_grafo *no_destino = encontra_no(g, destino);
    if(!no_destino)
        return NULL;

    heap *hp = heap_nova(g->tamanho);
    if(!hp)
        return NULL;

    for(int i=0; i<g->tamanho; i++)         //iniciar distancias a infinito
    {
        g->nos[i]->p_acumulado = INFINITY;
        g->nos[i]->anterior = NULL;
    }
    no_origem->p_acumulado=0;


    for(int i=0; i<g->tamanho; i++)
    {
        heap_insere(hp, g->nos[i], g->nos[i]->p_acumulado);     //inserir nós na heap
    }


    while(hp->tamanho!=0)
    {
        no_grafo *curr = heap_remove(hp);

        for(int i=0; i<curr->tamanho; i++)
        {
            aresta_grafo *ag = curr->arestas[i];
            
            if(difftime(mktime(&ag->partida), t_partida)<0)
                continue;
            
            time_t dist = difftime(mktime(&ag->chegada), mktime(&ag->partida));
            double peso = curr->p_acumulado + dist;
        
            if(ag->destino->p_acumulado > peso)
            {
                ag->destino->p_acumulado = peso;
                ag->destino->anterior = curr;

                heap_insere(hp, ag->destino, ag->destino->p_acumulado);

                // heap_apaga(hp);
                // hp = heap_nova(g->tamanho);
                // if(!hp)
                //     return NULL;

                // for(int j=0; j<g->tamanho; j++)
                // {
                //     heap_insere(hp, g->nos[j], g->nos[j]->p_acumulado);     //inserir nós na heap
                // }
            }
        }
    }

    heap_apaga(hp);

    if(!no_destino->anterior)
        return NULL;

    int tam=1;
    no_grafo *curr = no_destino;
    while(curr->anterior)           //descobrir o tamanho do percurso
    {
        tam++;
        curr = curr->anterior;
    }

    vec = (no_grafo**) calloc(tam, sizeof(no_grafo*));
    if(!vec)
        return NULL;

    for(int i=tam; i>0; i--)        //preencher o vetor
    {
        vec[i-1]=no_destino;
        no_destino = no_destino->anterior;
    }

    *n=tam;
    return vec;
}

no_grafo **menos_transbordos(grafo *g, char *origem, char *destino, data partida, int *n)
{
    if(!g || !origem || !destino || !n)
        return NULL;

    no_grafo **vec=NULL;

    time_t t_partida = mktime(&partida);

    no_grafo *no_origem = encontra_no(g, origem);
    if(!no_origem)
        return NULL;

    no_grafo *no_destino = encontra_no(g, destino);
    if(!no_destino)
        return NULL;

    heap *hp = heap_nova(g->tamanho);
    if(!hp)
        return NULL;

    for(int i=0; i<g->tamanho; i++)         //iniciar distancias a infinito
    {
        g->nos[i]->p_acumulado = INFINITY;
        g->nos[i]->anterior = NULL;
    }
    no_origem->p_acumulado=0;


    for(int i=0; i<g->tamanho; i++)
    {
        heap_insere(hp, g->nos[i], g->nos[i]->p_acumulado);     //inserir nós na heap
    }


    while(hp->tamanho!=0)
    {
        no_grafo *curr = heap_remove(hp);

        for(int i=0; i<curr->tamanho; i++)
        {
            aresta_grafo *ag = curr->arestas[i];
            
            if(difftime(mktime(&ag->partida), t_partida)<0)
                continue;
            
            double peso = curr->p_acumulado + 1;
        
            if(ag->destino->p_acumulado > peso)
            {
                ag->destino->p_acumulado = peso;
                ag->destino->anterior = curr;

                heap_insere(hp, ag->destino, ag->destino->p_acumulado);

                // heap_apaga(hp);
                // hp = heap_nova(g->tamanho);
                // if(!hp)
                //     return NULL;

                // for(int j=0; j<g->tamanho; j++)
                // {
                //     heap_insere(hp, g->nos[j], g->nos[j]->p_acumulado);     //inserir nós na heap
                // }
            }
        }
    }

    heap_apaga(hp);
    
    if(!no_destino->anterior)
        return NULL;

    int tam=1;
    no_grafo *curr = no_destino;
    while(curr->anterior)           //descobrir o tamanho do percurso
    {
        tam++;
        curr = curr->anterior;
    }

    vec = (no_grafo**) calloc(tam, sizeof(no_grafo*));
    if(!vec)
        return NULL;

    for(int i=tam; i>0; i--)        //preencher o vetor
    {
        vec[i-1]=no_destino;
        no_destino = no_destino->anterior;
    }

    *n=tam;
    return vec;
}

aresta_grafo **atualiza_lugares(char *ficheiro, grafo *g, int *n)
{
    if(!ficheiro || !g || !n)
       return NULL;

    aresta_grafo **vec = NULL;
    FILE *f;
    char str[200] = {0}, codigo[100] = {0}, *tk;
    int lugares, tam=0;

    
    f=fopen(ficheiro, "r");

    while(fgets(str, 200 * sizeof(char), f) != NULL)
    {
        tk = strtok(str, ",");
        if(!tk)
            break;
        
        strcpy(codigo, tk);
        
        tk = strtok(NULL, ",");
        lugares = atoi(tk);
        

        for(int i=0; i<g->tamanho; i++)
        {
            no_grafo *no = g->nos[i];
            
            for(int j=0; j<no->tamanho; j++)
            {
                aresta_grafo *ag = no->arestas[j];
                
                if(strcmp(codigo, ag->codigo)==0)
                {
                    ag->lugares = lugares;
                    if(lugares==0)
                    {
                        tam++;

                        vec = (aresta_grafo**) realloc(vec, tam * sizeof(aresta_grafo*));
                        if(!vec && tam!=0)
                            return NULL;

                        vec[tam-1] = ag;
                
                        for(int k=j; k<no->tamanho-1; k++)
                        {
                            no->arestas[k]=no->arestas[k+1];
                        }
                        no->tamanho--;
                        j--;

                        no->arestas = (aresta_grafo**) realloc(no->arestas, no->tamanho * sizeof(aresta_grafo*));
                        if(!no->arestas && no->tamanho!=0)
                            return NULL;
                    }                        
                }          
            }            
        }
    }
    
    fclose(f);

    *n=tam;
    return vec;
}

grafo *grafo_importa(const char *nome_ficheiro)
{
    if (nome_ficheiro == NULL)
        return NULL;

    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL)
        return NULL;

    grafo *g = grafo_novo();
    if (g == NULL)
    {
        fclose(f);
        return NULL;
    }
    char str[500] = {0}, codigo[15] = {0}, origem[50] = {0}, destino[50] = {0}, companhia[4] = {0};
    int lugares = 0, num[26] = {0}, i;
    double preco = 0;
    struct tm partida, chegada;

    char *token;
    no_grafo *no_origem, *no_destino;
    int ret;
    while (fgets(str, 500 * sizeof(char), f) != NULL)
    {
        str[strlen(str) - 1] = '\0';

        token = strtok(str, ",");
        if (!token)
            break;
        strcpy(codigo, token);

        token = strtok(NULL, ",");
        strcpy(origem, token);

        token = strtok(NULL, ",");
        strcpy(destino, token);

        token = strtok(NULL, ",");
        partida.tm_year = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_mon = atoi(token) - 1;

        token = strtok(NULL, ",");
        partida.tm_mday = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_hour = atoi(token);

        token = strtok(NULL, ",");
        partida.tm_min = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_year = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_mon = atoi(token) - 1;

        token = strtok(NULL, ",");
        chegada.tm_mday = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_hour = atoi(token);

        token = strtok(NULL, ",");
        chegada.tm_min = atoi(token);

        token = strtok(NULL, ",");
        preco = atof(token);

        token = strtok(NULL, ",");
        lugares = atoi(token);

        token = strtok(NULL, ",");
        strcpy(companhia, token);

        // procura no grafo um nó com o nome da origem
        no_origem = encontra_no(g, origem);
        // se o nó ainda não existe, cria-o e insere-o no grafo
        if (!no_origem)
        {

            no_origem = no_insere(g, origem);

            if (!no_origem)
            {
                fclose(f);
                return NULL;
            }
        }
        // procura no grafo um nó com o nome do destino
        no_destino = encontra_no(g, destino);

        // se o nó ainda não existe, cria-o e insere-o no grafo
        if (!no_destino)
        {
            no_destino = no_insere(g, destino);

            if (!no_destino)
            {
                fclose(f);
                return NULL;
            }
        }

        if (existe_aresta(no_origem, no_destino, codigo) == 0)
        {

            ret = cria_aresta(no_origem, no_destino, codigo, companhia, partida, chegada, preco, lugares);

            if (ret == -1)
            {
                fclose(f);
                return NULL;
            }
        }
    }

    fclose(f);

    return g;
}
