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
} *Edge;

typedef struct node{
    Edge e;
    struct node *prev;
    struct node *next;
} *Link;

/*
 * List Operations
 */

typedef struct list_t {
  Link head;
  int size;
  Link back;
} list_t; // list type

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
list_t **adj_list;
int *level;

void init_graph(int vertex){
    adj_list = (list_t **) calloc(vertex+1, sizeof(list_t*));     // A source vai ser 0 e o target vai ser V+1
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
  Edge a = create_edge(v,C);
  Edge b = create_edge(u,C);

  push_back(adj_list[u], a);
  push_back(adj_list[u], b); //reverse
}


int main(){
    int m; /*linhas*/
    int n; /*colunas*/
    int i, begin, end;

    /* input 1: dims da matriz */
    scanf("%d %d", &m, &n);

    /* initialize graph with +2 vertices for the source and target*/
    init_graph(m*n+2);

    for (i=0; i < m; i++) {
   	    scanf("%d %d", &begin, &end);
        addEdge(begin, end, 999999);
    }
}


/*HELLO WORLD*/
