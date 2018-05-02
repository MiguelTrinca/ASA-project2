#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

# define min(a,b) (((a)<(b))?(a):(b))

/* terminal colors */
#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

#define DEBUGGING 0
/*
  Otimizacao! Quando estamos a ler o input podemos logo descobrir os caminhos e flows de distancia 3.
*/

/*TODO
    Ler o input 4 e 5 o 4 esta a dar problemas
    Implementar a otimizacao
    SendFlow temos que pensar de maneira diferente. Nao da para fazer como ele esta
    a fazer.
    MaxFlow
*/


typedef struct edge{
    int id;
    int flow;
    int cap; /* Capacity*/
    /*int rev;*/ /*https://www.geeksforgeeks.org/dinics-algorithm-maximum-flow/*/
    struct edge *rev; /* fints directly to the reverse edged */
} *Edge;

typedef struct list_node{
    Edge e;
    struct list_node *next;
} *Link;


typedef struct list_t {
  Edge *head;
  int size;
  Link back;
} list_t; /* list type*/


/*
 * Global
 */


list_t **adj_list;
int *level;
int V;
int lines_n, columns_n;

struct edge *edges;
int edges_sz = 0; /* keeps track of edges's size */
/*
 * List Operations
 */


list_t* init_list(int max_size){
  list_t* new_list = (list_t*) malloc(sizeof(list_t));
  new_list->head = (Edge*) malloc(max_size*sizeof(Edge));
  new_list->size = 0;
  new_list->back = NULL;
  return new_list;
}

void push_back(list_t *list, Edge e){
  list->head[list->size] = e;
  list->size += 1;
}

/* lists returns the element at a specified position */
Edge element_at(list_t *list, int pos){
  return list->head[pos];
}


void print_list(list_t *list){
  int i;
  for (i=0; i<list->size; i++)
      printf("[%d|%d|%d]\t", list->head[i]->id, list->head[i]->flow, list->head[i]->cap);
  printf("\n");
}

void free_list(list_t *list){
  free(list->head);
  free(list);
}


/*
 * Queue Operations
 */


/*Queue de inteiros*/
typedef struct queue_node{
  int i;
  struct queue_node *next;
  struct queue_node *prev;
} *Queue;

/* contains information about the queue */
typedef struct queue_t {
  int *head;
  int front;
  int back;
} queue_t; /* list type*/

/*Queue Q;*/
queue_t Q;


void init_Queue(){
  Q.front = 0;
  Q.back = 0;
  Q.head = (int*) malloc(sizeof(int)*V);
}

void reset_Q(){
  Q.front = 0;
  Q.back = 0;
}

void Q_append(int i){ /* adds element to the back of the list */
  Q.head[Q.back] = i;
  Q.back += 1;
}

int Q_empty(){
  return (Q.front == Q.back);
}

int Q_front(){
  return Q.head[Q.front];
}

int Q_pop() { /* pops from the front */
  int retVal = Q.head[Q.front];
  Q.front += 1;
  return retVal;
}

void print_Q(){
  int i;
  for (i=0; i<(Q.back - Q.front); i++){
    if(DEBUGGING) printf("%d |", Q.head[Q.front+i]);
  }
}

/*
 *  Graph Operation
 */

void init_graph(int vertex){
    int id=0;
    adj_list = (list_t **) malloc((vertex+1)*sizeof(list_t*));     /* A source vai ser 0 e o target vai ser V+1*/
    level = (int *) malloc(sizeof(int)*vertex);

    adj_list[id] = init_list(vertex);   /* initilizing source */
    for (id=1; id < vertex+1; id++) {
        adj_list[id] = init_list(10);   /* Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas*/
        /* each node in the photograh plane has at most 9 edges including reverse ones */
    }
}

void destroy_graph(int vertex){
  int id;
  free(level);
  for (id=0; id < vertex+1; id++) {
      free_list(adj_list[id]);   /* Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas*/
  }
  free(adj_list);
}

Edge create_edge(int id, int cap){
  Edge e = &edges[edges_sz++];
  e->id = id;
  e->flow = 0;
  e->cap = cap;
  e->rev = NULL; /* will be initialized just after */
  if (DEBUGGING) printf("%d %d %d\n", e->id, e->flow, e->cap);
  return e;
}


Edge addEdge(int u, int v, int C){
  Edge a = create_edge(v,C);
  Edge b = create_edge(u,0);
  a->rev = b;
  b->rev = a;
  push_back(adj_list[u], a);
  push_back(adj_list[v], b);
  return a;
}

/*
 * Dinic's Algorithm
 */

/* Level Graph construction */
int BFS(int s, int t){
  int i;
  int u;
  if(DEBUGGING) printf("BFSing\n");
  for (i = 0 ; i < V ; i++) {
      level[i] = -1;
  }

  level[s] = 0;  /* Level of source vertex*/

  reset_Q();

  Q_append(s);

  while (!Q_empty()){
    u = Q_pop();
    for (i=0;i<adj_list[u]->size;i++) {
      Edge e = adj_list[u]->head[i];
      /*printf("e->id = %d\n", e->id);*/
      if (level[e->id] < 0  && e->flow < e->cap){
        level[e->id] = level[u] + 1;
        Q_append(e->id);
      }
    }
  }
  return level[t] < 0 ? 0 : 1 ;
}



int sendFlow(int u, int flow, int t, int *start){
  /*Link temp;*/
  if (DEBUGGING) printf("send_flow\n");
  /* Target reached*/
  if (u == t)
    return flow;


  for (  ; start[u] < adj_list[u]->size; start[u]++){
    Edge e = element_at(adj_list[u], start[u]);
    if (level[e->id] == level[u]+1 && e->flow < e->cap){
      /* find minimum flow from u to t*/
      int curr_flow = min(flow, e->cap - e->flow);
      int temp_flow = sendFlow(e->id, curr_flow, t, start);
      /* flow is greater than zero*/

      if (temp_flow > 0){
        /* add flow  to current edge*/
        e->flow += temp_flow;
        if (u!=0 && t!=V-1) e->rev->flow -= temp_flow;
        return temp_flow;
      }
    }
  }
  return 0;
}

int DinicMaxflow(int s, int t){
  int total = 0;
  int flow;

  if (s == t)
      return -1;

  /* Augment the flow while there is path
   from source to target*/
  while (BFS(s, t) == 1){
    int *start = (int*) calloc(V+1, sizeof(int));

    /* while flow is not zero in graph from S to D*/
    while ((flow = sendFlow(s, INT_MAX, t, start))){
      /* Add path flow to overall flow*/
      total += flow;
    }
    free(start);
  }

  return total;
}

void print_output(int max_flow, int n, int m){

  int i,j;
  printf("%d\n\n", max_flow);
  j=0;
  for(i = 1; i<V-1; i++){
    if(level[i]<0){
      j++;
      printf("P ");
    }
    else{
      j++;
      printf("C ");
    }
    if(j%m == 0){
      printf("\n");
    }
  }
}


int main(){
  int m; /*linhas*/
  int n; /*colunas*/
  int i, j, cap;
  int max_flow;
  int flow = 0;
  /*int *start;*/
  int *cap_source;
  Edge *source_fast;
  Edge ed;

  /* input 1: dims da matriz */
  scanf("%d %d", &n, &m);
  lines_n = n;
  columns_n = m;
  /* Number of vertexs*/
  V = m*n + 2;
  /* initialize graph with +2 vertices for the source and target*/
  init_graph(V);
  init_Queue();

  /*list_t *list = init_list();
  push_back(list,create_edge(1,1));
  printf("stuff\n");
  push_back(list,create_edge(2,2));
  print_list(list);*/

  /* each edge has the right to 9 edges at most*/
  /* target has no edges */
  edges = (Edge) malloc((11*(m*n))*sizeof(struct edge));
  edges_sz = 0;


  cap_source = (int*) malloc((V+1)*sizeof(int));
  source_fast = (Edge*) malloc((V+1)*sizeof(Edge));

  /* input 2: capacidade dos vertices da source (pretos) */
  for(i=1; i < V-1; i++){
    scanf("%d", &cap);
    if (cap>0) {
      ed = create_edge(i,cap); /* same as addEdge but with no reverse */
      push_back(adj_list[0], ed);
      source_fast[i] = ed;
      cap_source[i]=cap;

      /* they need a reverse edge for the BFS to work */
      ed = create_edge(0,0); /* same as addEdge but with no reverse */
      push_back(adj_list[i], ed);

    } else {
      source_fast[i] = NULL;
      cap_source[i]=0;
    }
  }
  /* input 3: capacidade dos vertices do target (pretos) */
  for(i=1; i < V-1; i++){
    scanf("%d", &cap);
    if (cap > 0) {
      ed = create_edge(V-1,cap); /* to the target needs no reverse */
      push_back(adj_list[i], ed);
      if (cap_source[i]){
        if (cap <= cap_source[i]) { /* se capacidade meio>->target >= source>->meio*/
          source_fast[i]->flow = cap; /* updates flow source>meio */
          ed->flow = cap;
          flow += cap;
        } else {
          source_fast[i]->flow = cap_source[i]; /* updates flow source>meio */
          ed->flow = cap_source[i];
          flow += cap_source[i];
        }
      }
    }
    /*start = (int*) calloc(V+1, sizeof(int));*/
    /*  Otimizacao: Mandar logo o fluxo total. (pois este caminho {s,i,t} e o menor caminho)
    1 - Comparar a capacidade lida do vertice source -> i e i->target.
    2 - Mandar o flow total da capacidade minima.*/
   /*if(cap <= adj_list[0]->head->e->cap) {flow += sendFlow(0, cap, V+1, start);
   else flow += sendFlow(0, adj_list[0]->head->e->cap, V+1, start);*/
   /*free(start);*/

  }

  free(cap_source);
  free(source_fast);

  /* input 4: capacidade entre vertices na horizontal */
  for (i=0;i<n;i++) { /* itera nas linhas */
    for (j=1;j<m;j++){ /* itera nas colunas */
      scanf("%d", &cap);
      if (cap > 0) {
        addEdge(i*m + j, i*m+j+1, cap);
        addEdge(i*m+j+1, i*m + j, cap);
      }
    }
  }
  /* input 5: capacidade entre vertices na vertical */
  for (i=0;i<n-1;i++) { /* itera nas linhas */
    for (j=1;j<=m;j++){  /* itera nas colunas */
      scanf("%d", &cap);
      if (cap > 0) {
        addEdge(i*m+j,(i+1)*m+j , cap);
        addEdge((i+1)*m+j, i*m+j, cap);
      }
    }
  }
  /* for debugging */
  /*for(i = 0; i<V; i++){
    printf("%d\t", i);
    print_list(adj_list[i]);
  }*/
  max_flow = DinicMaxflow(0,V-1);
  print_output(flow+max_flow, n, m);
  destroy_graph(V);
  free(edges);

  return 0;
}
