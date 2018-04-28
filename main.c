#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

# define min(a,b) (((a)<(b))?(a):(b))


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
    int cap; // Capacity
    int rev; //https://www.geeksforgeeks.org/dinics-algorithm-maximum-flow/
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
  Link l = list->head;
  int i;
  for (i=0;i<pos && l!=NULL;i++) l=l->next;
  if (l==NULL) return -1;
  return l;
}

void print_list(list_t *list){
  Link l;
  for (l=list->head; l!=NULL; l=l->next)
    printf("%d |", l->e->id);
  printf("\n");
}

void print_list_reverse(list_t *list){
  Link l;
  for (l=list->back; l!=NULL; l=l->prev) printf("%d |", l->e->id);
  printf("\n");
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
} queue_t; // list type

//Queue Q;
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
  if (Q.head == NULL) return -1;
  int retVal = Q.head->i;
  Queue tmp = Q.head;
  Q.head = Q.head->next;
  free(tmp);
  return retVal; /* returns -1 in case of error */
}

void print_Q(){
  Queue q;
  for (q = Q.head; q != NULL; q=q->next)
      printf("%d |", q->i);
  printf("\n");
}

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
  Edge a = create_edge(v,C);
  push_back(adj_list[u], a);
}

/*
 * Dinic's Algorithm
 */

/* Level Graph construction */
int BFS(int s, int t){
  for (int i = 0 ; i < V ; i++)
      level[i] = -1;

  level[s] = 0;  // Level of source vertex

  init_Queue();

  Q_append(s);

  Link l;
  while (!Q_empty()){
    int u = Q_pop();
    for (l = adj_list[u]->head; l != NULL; l=l->next){
      Edge e = l->e;
      if (level[e->id] < 0  && e->flow < e->cap){
                // Level of current vertex is,
                // level of parent + 1
        level[e->id] = level[u] + 1;

        Q_append(e->id);
        }
      }
    }
    // IF we can not reach to the sink we
    // return false else true
  return level[t] < 0 ? 0 : 1 ;
}



int sendFlow(int u, int flow, int t, int *start){
  // Sink reached
  if (u == t)
      return flow;

  // Traverse all adjacent edges one-by-one.
  Link l = element_at(adj_list[u], start[u]);
  /*int i;
  for (i=0;i<start[u];i++) l=l->next;*/


  /*Edge e = adj_list[u][start[u]];*/
  for (  ; start[u] < adj_list[u]->size; start[u]++){
    Edge e = l->e;
    // Pick next edge from adjacency list of u
    // Edge e = adj_list[u][start[u]];

    if (level[e->id] == level[u]+1 && e->flow < e->cap){
      // find minimum flow from u to t
      int curr_flow = min(flow, e->cap - e->flow);
      int temp_flow = sendFlow(e->id, curr_flow, t, start);
      // flow is greater than zero
      if (temp_flow > 0){
        // add flow  to current edge
        e->flow += temp_flow;
        // subtract flow from reverse edge
        // of current edge
        Link temp = element_at(adj_list[e->id],e->rev);
        temp->e->flow -= temp_flow;
        return temp_flow;
      }
    }
    l=l->next;
  }
    return 0;
}

int DinicMaxflow(int s, int t){
  // Corner case
  if (s == t)
      return -1;

  int total = 0;  // Initialize result

  // Augment the flow while there is path
  // from source to sink
  while (BFS(s, t) == 1){
    // store how many edges are visited
    // from V { 0 to V }

    //int *start = new int[V+1];
    int start[V+1];

   // while flow is not zero in graph from S to D
   int flow;
    while (flow = sendFlow(s, INT_MAX, t, start))

       // Add path flow to overall flow
        total += flow;
  }

  // return maximum flow
  return total;
}


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
      addEdge(i, V+1, cap);

      /*  Otimizacao: Mandar logo o fluxo total. (pois este caminho {s,i,t} e o menor caminho)
      1 - Comparar a capacidade lida do vertice source -> i e i->target.
      2 - Mandar o flow total da capacidade minima.*/

     if(adj_list[V]->head->e->cap <= adj_list[0]->head->e->cap){
       //sendFlow(cap);
     }
     else {
       //sendFlow(adj_list[0].head.e.cap);
     }

    }

    /* input 4: capacidade entre vertices na horizontal */
    //Esta a dar segfault
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
        addEdge(i*m+j,(i+1)*m+j , cap);
        addEdge((i+1)*m+j, i*m+j, cap);
      }
    }

    /* for debugging */
    /*for(i = 0; i<V-1; i++){
    printf("%d: ", i);
    print_list(adj_list[i]);
  }*/

    list_t *l = init_list();
    
    init_Queue();
    Q_append(1);
    Q_append(2);
    Q_append(3);
    Q_append(4);
    print_Q();
    printf("removed %d from list\n", Q_pop());
    printf("removed %d from list\n", Q_pop());
    printf("removed %d from list\n", Q_pop());
    printf("removed %d from list\n", Q_pop());
    print_Q();
}
