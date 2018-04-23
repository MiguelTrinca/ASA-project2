#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
  Otimizacao! Quando estamos a ler o input podemos logo descobrir os caminhos e flows de distancia 3.
*/

/*TODO
    Ler o input 4 e 5 o 4 esta a dar problemas
    Implementar a otimizacao
    Fazer funcoes para a Queue. (Ir ver a funcao BFS ver que funcoes e que precisa)
    BFS esta quase so precisa da Queue.
    SendFlow
    MaxFlow
*/


typedef struct edge{
    int id;
    int flow;
    int cap; // Capacity
    int rev; //https://www.geeksforgeeks.org/dinics-algorithm-maximum-flow/
} *Edge;

typedef struct node{
    Edge e;
    struct node *prev;
    struct node *next;
} *Link;


typedef struct list_t {
  Link head;
  int size;
  Link back;
} list_t; // list type

/*
 * Global
 */


list_t **adj_list;
int *level;
int V;

/*
 * List Operations
 */


list_t* init_list(){
  list_t* new_list = (list_t*) malloc(sizeof(list_t));
  new_list->head = NULL;
  new_list->size = 0;
  new_list->back = NULL;
  return new_list;
}

void push_front(list_t *list, Edge e){
    Link new = (Link) malloc(sizeof(struct node));
    new->e = e;

    if (list->head == NULL){
      new->next = NULL;
      list->back = new;
    } else {
      new->next = list->head;
      list->head->prev = new;
    }
    new->prev = NULL;
    list->head = new;
    list->size += 1;
}

void push_back(list_t *list, Edge e){
  Link new = (Link) malloc(sizeof(struct node));
  new->e = e;
  new->next = NULL;
  if (list->back == NULL) {
    new->prev = NULL;
    list->head = new;
  } else {
    new->prev = list->back;
    list->back->next = new;
  }
  list->back = new;
  list->size += 1;
}

void print_list(list_t *list){
  printf("printing list: ");
  Link l;
  for (l=list->head; l!=NULL; l=l->next) printf("%d>", l->e->id);
  printf("\n");
}

void print_list_reverse(list_t *list){
  printf("printing list: ");
  Link l;
  for (l=list->back; l!=NULL; l=l->prev) printf("%d>", l->e->id);
  printf("\n");
}


/*
 * Queue Operations
 */



/*
 *  Graph Operation
 */  

void init_graph(int vertex){
    adj_list = (list_t **) calloc(vertex+1, sizeof(list_t*));     // A source vai ser 0 e o target vai ser V+1
    level = (int *) malloc(sizeof(int)*vertex);
    for (int id=0; id < vertex+1; id++) {
        adj_list[id] = init_list();   // Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas
    }
}

Edge create_edge(int id, int cap){
  Edge e = (struct edge*) malloc(sizeof(struct edge));
  e->id = id;
  e->flow = 0;
  e->cap = cap;
  /*e->rev = length(adj_list[v]);*/
  return e;
}


void addEdge(int u, int v, int C){
  /*Quando fazemos addEdge temos que por o backward edge tambem. Acho que esta implementacao so poe
  um vez*/
  Edge a = create_edge(v,C);
  Edge b = create_edge(u,C);

  push_back(adj_list[u], a);
  push_back(adj_list[u], b); //reverse
}

/*
 * Dinic's Algorithm
 */

/*
int BFS(int s, int t){
  for (int i = 0 ; i < V ; i++)
      level[i] = -1;

  level[s] = 0;  // Level of source vertex
 
   Criar a queue Q
  list< int > q;
  q.push_back(s);
 

  
  while (!empty(Q)){
    int u = front(Q);
    pop_front(Q);
    for (l = adj[u]->head; i != NULL; l=l->next){
      Edge e = l->e;
      if (level[e.id] < 0  && e.flow < e.cap){
                // Level of current vertex is,
                // level of parent + 1
        level[e.id] = level[u] + 1;
 
        push_back(Q, e.id);
        }
      }
    }
    // IF we can not reach to the sink we
    // return false else true
  return level[t] < 0 ? 0 : 1 ;
} */

int main(){
    int m; /*linhas*/
    int n; /*colunas*/
    int i, j, cap;

    /* input 1: dims da matriz */
    scanf("%d %d", &m, &n);

    /* Number of vertexs*/    
    V = m*n + 2;

    /* initialize graph with +2 vertices for the source and target*/
    init_graph(V);

    /* input 2: capacidade dos vertices da source (pretos) */
    for(i=1; i < V-1; i++){
      scanf("%d", &cap);
      addEdge(0, i, cap);
    }
  
    /* input 3: capacidade dos vertices do target (pretos) */
    for(i=1; i < V-1; i++){
      scanf("%d", &cap);
      addEdge(V, i, cap);
      
      /*  Otimizacao: Mandar logo o fluxo total. (pois este caminho {s,i,t} e o menor caminho)
      1 - Comparar a capacidade lida do vertice source -> i e i->target.
      2 - Mandar o flow total da capacidade minima.
      
     if(adj_list[V].head.e.cap <= adj_list[0].head.e.cap){
       //sendFlow(cap);
     }
     else if(adj_list[V].head.e.cap >= adj_list[0].head.e.cap){
       //sendFlow(adj_list[0].head.e.cap);       
     }*/
   
    }

    /* input 4: capacidade entre vertices na horizontal */
    //Esta a dar segfault
    i=1;
    j=1;
    while(i < n){
      scanf("%d", &cap);
      addEdge(j, j+1, cap);
      if(i == n-1 && j != V-2){
        i = 1;
        j *= 2;
      }
      i++;
      j++;
    }
    
    /* input 5: capacidade entre vertices na vertical */
    
}


