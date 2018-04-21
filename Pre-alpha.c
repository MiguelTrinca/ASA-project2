#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
  Fiz cenas so para te ensinar a usar git
*/
// Ver aulas 10 e 11 onde tem codigo de ford-fulkerson e edmonds-karp
// Ver aula 7 onde esta a BFS

//Nota: O Edmonds-Karp e um ford-fulkerson, no entanto temos que fazer BFS para descobrir os caminhos de aumento,
//  isto e os caminhos mais curtos


//Para implementar tanto EK ou Dinitz temos que fazer uma BFS, portanto Fazer isso.

/*TODO
    Defenir a minha lista de adjacencias
    Ler o input
    Fazer BFS
    Fazer Dinics
*/


typedef struct edge{
    int id;
    int flow;
    int cap; // Capacity
    int rev; //https://www.geeksforgeeks.org/dinics-algorithm-maximum-flow/
} Edge;

typedef struct node{
    Edge e;
    struct node *next;
} Link;

Link **adj_list;
int *level;

void init_list(Link *l){
    l = NULL;
}

Link* push_end(Link *l, Edge e){
    Link *new;
    new = (struct node *) malloc(sizeof(struct node));
    new->e = e;
    new->next = l;
    l = new;
    return l;  // tenho que devolver o l porque nao e global;
}

void init_graph(int vertex){
    adj_list = (struct node **)malloc(sizeof(struct node *)*(vertex+2));     // A source vai ser 0 e o target vai ser V+1
    for (int id=0; id < vertex+2; id++) {
        init_list(adj_list[id]);   // Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas
    }
}


void addEdge(int u, int v, int C){
    Edge a = (struct edge)malloc(sizeof(struct edge *));
    Edge b = (struct edge)malloc(sizeof(struct edge *));

    a.id = v;
    a.flow = 0;
    a.cap = C;
//    a.rev = length(adj_list[v]);

    b.id = u;
    b.flow = 0;
    b.cap = C;
//    b.rev = length(adj_list[v]);

    adj_list[u] = push_end(adj_list[u], a);
    adj_list[v] = push_end(adj_list[u], b); //reverse
}


int main(){
    int V;
    int m;          //linhas
    int n;          //colunas
    scanf("%d", &m);
    scanf("%d", &n);

    V = m*n;        //Numero de Vertices

   /* for (int i=0; i < edges; i++) {
   	    scanf("%d %d", &begin, &end);
        add_edge(begin, end);         //Comeca a construir o grafo segundo o input
       }*/

    init_graph(V);

}
