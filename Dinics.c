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
    struct node *next;
} *Link;



/*
 * Global
 */


Link **adj_list;
int *level;
int V;

/*
 * List Operations
 */

void List_append(int p, Edge e){
    Link new = (Link) malloc(sizeof(struct link*));

    Link last = adj_list[p];

    new->e = e;

    new->next = NULL;

    if (adj_list[p] == NULL)
    {
       adj_list[p] = new;
       return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new;
    return;
}

Edge find_key(int p, int k){
  int cont;
  cont = 0;
  Link l;
  Edge e;
  for(l = adj_list[p]; l!=NULL; l=l->next){
    if(cont == k){
      e = l->e;
      break;
    }
    cont++;
  }
  /*while(cont != k){
    l = l->next;
    printf("Acho que nao posso fazer isto\n");
    cont++;
  } */
  return e;
}


void change_key(int p, int k, int value){
  int cont;
  cont = 0;
  Link l;
  l = adj_list[p];
  while(cont != k){
    l = l->next;
    cont++;
  } 
  l->e->flow -= value;
}

int List_size(int p){
  int cont;
  cont = 0;
  Link l;
  for (l = adj_list[p]; l!=NULL; l=l->next){
    cont++;
  }
  return cont;
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
    adj_list = (Link **) calloc(vertex+1, sizeof(Link*));     // A source vai ser 0 e o target vai ser V+1
    level = (int *) malloc(sizeof(int)*vertex);
    for (int id=0; id < vertex+1; id++) {
        adj_list[id] = NULL;   // Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas
    }
}

Edge create_edge(int id, int cap){
  Edge e = (struct edge*) malloc(sizeof(struct edge));
  e->id = id;
  e->flow = 0;
  e->cap = cap;
  e->rev = List_size(id);
  return e;
}


void addEdge(int u, int v, int C){
  /*Quando fazemos addEdge temos que por o backward edge tambem. Acho que esta implementacao so poe
  um vez*/
  Edge a = create_edge(v,C);
  Edge b = create_edge(u,C);

  List_append(u, a);
  List_append(v, b); //reverse
}

/*
 * Dinic's Algorithm
 */

int BFS(int s, int t){
  printf("Estou a fazer a BFS\n");
  printf("V = %d\n", V);
    
  for (int i = 0 ; i < V ; i++)
      level[i] = -1;

  level[s] = 0;  // Level of source vertex

  init_Queue();

  Queue_append(s);

  Link l;
  while (!Queue_is_empty()){
    int u = Queue_front();
    Queue_pop_front();
    for (l = adj_list[u]; l != NULL; l=l->next){
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

    for(int i = 0; i<V; i++){
        printf("Level[%d] = %d\t", i, level[i]);
    }

    printf("\n");

  return level[t] < 0 ? 0 : 1 ;
}



int sendFlow(int u, int flow, int t, int start[]){
  if (u==t)
    return flow;

    for( ; start[u] < List_size(u); start[u]++){

    Edge e = find_key(u, start[u]);
    
    printf("Edge e = %d\n", e->id);
  
    printf("Level[%d] = %d\n", e->id, level[e->id]);
    
    
    if(level[e->id] == level[u]+1 && e->flow < e->cap){
    
      int curr_flow = min(flow, e->cap - e->flow);

      printf("Current flow = %d\n", curr_flow);

      int temp_flow = sendFlow(e->id, curr_flow, t, start);
    
       // flow is greater than zero
      if (temp_flow > 0){
       // add flow  to current edge
        e->flow += temp_flow;
        change_key(e->id, e->rev, temp_flow);
        return temp_flow;
      }
    }
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
    int i;
    int start[V+1];
    for (i = 0; i < V+1; i++){
      start[i] = 0;
    }

    // while flow is not zero in graph from S to D
    int flow;
    printf("Vou mandar Fluxo \n");
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
      addEdge(i, V-1, cap);

      /*  Otimizacao: Mandar logo o fluxo total. (pois este caminho {s,i,t} e o menor caminho)
      1 - Comparar a capacidade lida do vertice source -> i e i->target.
      2 - Mandar o flow total da capacidade minima.*/

     //if(adj_list[V]->head->e->cap <= adj_list[0]->head->e->cap){
       //sendFlow(cap);
     //}
     //else {
       //sendFlow(adj_list[0].head.e.cap);
     //}

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
  
  
  //addEdge(0, 1, 16 );
  //addEdge(0, 2, 13 );
  //addEdge(1, 2, 10 );
  //addEdge(1, 3, 12 );
  //addEdge(2, 1, 4 );
  //addEdge(2, 4, 14);
  //addEdge(3, 2, 9 );
  //addEdge(3, 5, 20 );
  //addEdge(4, 3, 7 );
  //addEdge(4, 5, 4);


  //addEdge(0, 1, 3 );
  //addEdge(0, 2, 7 ) ;
  //addEdge(1, 3, 9);
  //addEdge(1, 4, 9 );
  //addEdge(2, 1, 9 );
  //addEdge(2, 4, 9);
  //addEdge(2, 5, 4);
  //addEdge(3, 5, 3);
  //addEdge(4, 5, 7 );
  //addEdge(0, 4, 10);  
  
  //addEdge(0, 1, 10);
  //addEdge(0, 2, 10);
  //addEdge(1, 3, 4 );
  //addEdge(1, 4, 8 );
  //addEdge(1, 2, 2 );
  //addEdge(2, 4, 9 );
  //addEdge(3, 5, 10 );
  //addEdge(4, 3, 6 );
  //addEdge(4, 5, 10 );
 
  
  printf("Maximum Flow %d\n", DinicMaxflow(0,5));
  return 0;
}
