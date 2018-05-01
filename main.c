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
    int rev; /*https://www.geeksforgeeks.org/dinics-algorithm-maximum-flow/*/
} *Edge;

typedef struct list_node{
    Edge e;
    struct list_node *prev;
    struct list_node *next;
} *Link;


typedef struct list_t {
  Link head;
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
    Link new = (Link) malloc(sizeof(struct list_node));
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
  Link new = (Link) malloc(sizeof(struct list_node));
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

/* lists returns the element at a specified position */
Link element_at(list_t *list, int pos){
  int i;
  Link l = list->head;
  if (l == NULL) return NULL;
  for (i=0;i<pos && l!=NULL;i++) l=l->next;
  return l;
}


void print_list(list_t *list){
  Link l;
  for (l=list->head; l!=NULL; l=l->next)
      printf("[%d|%d|%d]\t", l->e->id, l->e->flow, l->e->cap);
  printf("\n");
}

void print_list_reverse(list_t *list){
  Link l;
  if(DEBUGGING) for (l=list->back; l!=NULL; l=l->prev) printf("%d |", l->e->id);
    if(DEBUGGING) printf("\n");
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
  Queue head;
  Queue back;
} queue_t; /* list type*/

/*Queue Q;*/
queue_t Q;


void init_Queue(){
  Q.head = NULL;
  Q.back = NULL;
}

void Q_append(int i){ /* adds element to the back of the list */
    Queue new = (Queue) malloc(sizeof(struct queue_node));
    new->i = i;
    new->next = NULL;

    if (Q.head == NULL) {
      Q.head = new;
      new->prev = NULL;
    } else {
      new->prev = Q.back;
      Q.back->next = new;
    }
    Q.back = new;
    return;
}

int Q_empty(){
  return (Q.head == NULL);
}

int Q_front(){
  return Q.head->i;
}

int Q_pop() { /* pops from the front */
  Queue tmp = Q.head;
  int retVal = Q.head->i;
  if (Q.head == NULL) return -1;
  Q.head = Q.head->next;
  free(tmp);
  return retVal; /* returns -1 in case of error */
}

void print_Q(){
  Queue q;
  for (q = Q.head; q != NULL; q=q->next)
      if(DEBUGGING) printf("%d |", q->i);
  if(DEBUGGING) printf("\n");
}

/*
 *  Graph Operation
 */

void init_graph(int vertex){
    int id;
    adj_list = (list_t **) calloc(vertex+1, sizeof(list_t*));     /* A source vai ser 0 e o target vai ser V+1*/
    level = (int *) malloc(sizeof(int)*vertex);
    for (id=0; id < vertex+1; id++) {
        adj_list[id] = init_list();   /* Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas*/
    }
}

void print_flow_graph(){
  int i,j; /* i-linhas; m-colunas */
  printf("%sFlow from source\n", CYAN);

  for (i=0; i<lines_n; i++) {
    for (j=0; j<columns_n; j++) {
      if (element_at(adj_list[0],i*columns_n+j) != NULL)
        printf("%d\t",element_at(adj_list[0],i*columns_n+j)->e->flow);
      else printf("0\t");
    }
    printf("\n");
  }

  printf("%s\n", NORMAL);
}

Edge create_edge(int id, int cap){
  Edge e = (struct edge*) malloc(sizeof(struct edge));
  e->id = id;
  e->flow = 0;
  e->cap = cap;
  e->rev = adj_list[id]->size;
  return e;
}


void addEdge(int u, int v, int C){
  Edge a = create_edge(v,C);
  Edge b = create_edge(u,0);
  push_back(adj_list[u], a);
  push_back(adj_list[v], b);
}

void addEdgeSource(int u, int v, int C){
  Edge a = create_edge(v,C);
  push_back(adj_list[u], a);
}

/*
 * Dinic's Algorithm
 */

/* Level Graph construction */
int BFS(int s, int t){
  int i;
  Link l;
  int u;
  if(DEBUGGING) printf("BFSing\n");
  for (i = 0 ; i < V ; i++) {
      level[i] = -1;
  }

  level[s] = 0;  /* Level of source vertex*/

  init_Queue();

  Q_append(s);

  while (!Q_empty()){
    u = Q_pop();
    for (l = adj_list[u]->head; l != NULL; l=l->next){
      Edge e = l->e;
      if (level[e->id] < 0  && e->flow < e->cap){
        level[e->id] = level[u] + 1;
        Q_append(e->id);
      }
    }
   }
  return level[t] < 0 ? 0 : 1 ;
}



int sendFlow(int u, int flow, int t, int *start){
  Link temp;
  if (DEBUGGING) printf("send_flow\n");
  /* Target reached*/
  if (u == t)
    return flow;

  for (  ; start[u] < adj_list[u]->size; start[u]++){
    Link l = element_at(adj_list[u], start[u]);
    Edge e = l->e;

    if (level[e->id] == level[u]+1 && e->flow < e->cap){
      /* find minimum flow from u to t*/
      int curr_flow = min(flow, e->cap - e->flow);
      int temp_flow = sendFlow(e->id, curr_flow, t, start);
      /* flow is greater than zero*/

      if (temp_flow > 0){
        /* add flow  to current edge*/
        e->flow += temp_flow;
        temp = element_at(adj_list[e->id],e->rev);
        temp->e->flow -= temp_flow;
        return temp_flow;
      }
    }
    l= l->next;
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
  int *start;
  /* input 1: dims da matriz */
  scanf("%d %d", &n, &m);
  lines_n = n;
  columns_n = m;
  /* Number of vertexs*/
  V = m*n + 2;
  /* initialize graph with +2 vertices for the source and target*/
  init_graph(V);
  /*list_t *list = init_list();
  push_back(list,create_edge(1,1));
  printf("stuff\n");
  push_back(list,create_edge(2,2));
  print_list(list);*/
  /* input 2: capacidade dos vertices da source (pretos) */
  for(i=1; i < V-1; i++){
    scanf("%d", &cap);
    addEdge(0, i, cap);
  }
  /* input 3: capacidade dos vertices do target (pretos) */
  for(i=1; i < V-1; i++){
    scanf("%d", &cap);
    addEdge(i, V-1, cap);
    start = (int*) calloc(V+1, sizeof(int));
    /*  Otimizacao: Mandar logo o fluxo total. (pois este caminho {s,i,t} e o menor caminho)
    1 - Comparar a capacidade lida do vertice source -> i e i->target.
    2 - Mandar o flow total da capacidade minima.*/
   /*if(cap <= adj_list[0]->head->e->cap) flow += sendFlow(0, cap, V+1, start);
   else flow += sendFlow(0, adj_list[0]->head->e->cap, V+1, start);*/
   free(start);
  }
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
  /*if(DEBUGGING)
    for(i = 0; i<V; i++){
      printf("%d\t", i);
      print_list(adj_list[i]);
    }*/
  max_flow = DinicMaxflow(0,V-1);
  print_output(flow+max_flow, n, m);
  return 0;
}
