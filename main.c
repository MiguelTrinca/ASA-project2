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

/*Queue de inteiros*/
typedef struct queue{
  int i;
  struct queue *next;
} *Queue;

Queue Q;

void init_Queue(){
  Q = NULL;
}

void Queue_append(int i){
    Queue new = (Queue) malloc(sizeof(struct queue));
 
    Queue last = Q; 
    
    new->i = i;
 
    new->next = NULL;
 
    if (Q == NULL)
    {
       Q = new;
       return;
    }  
      
    while (last->next != NULL)
        last = last->next;
  
    last->next = new;
    return;    
}

int Queue_is_empty(){
  return Q == NULL;
}

int Queue_front(){
  return Q->i;
}

void Queue_pop_front() {
    Queue tmp = Q;            
    if (tmp == NULL) 
      return;            
    Q = tmp->next;                   
    free (tmp);                    
}
void print_Queue(){
    Queue q;
    for (q = Q; q != NULL; q=q->next)
        printf("%d\n", q->i);
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

int BFS(int s, int t){
  for (int i = 0 ; i < V ; i++)
      level[i] = -1;

  level[s] = 0;  // Level of source vertex
 
  init_Queue();
   
  Queue_append(s);
  
  Link l;
  while (!Queue_is_empty()){
    int u = Queue_front();
    Queue_pop_front();
    for (l = adj_list[u]->head; l != NULL; l=l->next){
      Edge e = l->e;
      if (level[e->id] < 0  && e->flow < e->cap){
                // Level of current vertex is,
                // level of parent + 1
        level[e->id] = level[u] + 1;
 
        Queue_append(e->id);
        }
      }
    }
    // IF we can not reach to the sink we
    // return false else true
  return level[t] < 0 ? 0 : 1 ;
}



int sendFlow(int u, int flow, int t, int start[]){
  // Sink reached
  if (u == t)
      return flow;}
/*
  // Traverse all adjacent edges one -by - one.
  for (  ; start[u] < adj[u].size(); start[u]++){
    // Pick next edge from adjacency list of u
    Edge e = adj[u][start[u]]; 
                                 
    if (level[e.v] == level[u]+1 && e.flow < e.C){
      // find minimum flow from u to t
      int curr_flow = min(flow, e.C - e.flow);
     int temp_flow = sendFlow(e.v, curr_flow, t, start);
     // flow is greater than zero
     if (temp_flow > 0){
       // add flow  to current edge
        e.flow += temp_flow;
       // subtract flow from reverse edge
       // of current edge
       adj[e.v][e.rev].flow -= temp_flow;
       return temp_flow;
      }
    }
  }
    return 0;
} */

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
      addEdge(V, i, cap);
      
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


