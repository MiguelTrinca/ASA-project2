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
    struct edge *rev; /* fints directly to the reverse edged */
} *Edge;

int V = 0;
int num_edges = 0;

int main(){
  int m; /*linhas*/
  int n; /*colunas*/
  /*int v;*/
  int i, j, cap;

  /*Edge *edges;*/ /* datastructures that hold */
  /*int *offset;*/ /* and allow access to the vertices */

  /* just storing the structures */
  int *v_source;
  int *v_target;
  int *horizont;
  int *vertical;

  /* input 1: dims da matriz */
  scanf("%d %d", &n, &m);

  /* Number of vertexs*/
  V = m*n + 2;

  /* just storing the structures */
  v_source = (int*) malloc(V*sizeof(int));
  v_target = (int*) malloc(V*sizeof(int));
  horizont = (int*) malloc(V*sizeof(int));
  vertical = (int*) malloc(V*sizeof(int));

  /* input 2: capacidade dos vertices da source (pretos) */
  for (i=0;i<n;i++) { /* itera nas linhas */
    for (j=0;j<m;j++){ /* itera nas colunas */
      scanf("%d", &cap);
      v_source[i*m + j] = 0;
      if (cap!=0) num_edges++;
    } printf("\n");
  } printf("\n");

  /* input 3: capacidade dos vertices do target (pretos) */
  for (i=0;i<n;i++) { /* itera nas linhas */
    for (j=0;j<m;j++){ /* itera nas colunas */
      scanf("%d", &v_target[i*m + j]);
      printf("%d ",v_target[i*m + j]);
    } printf("\n");
  } printf("\n");

  /* input 4: capacidade entre vertices na horizontal */
  for (i=0;i<n;i++) { /* itera nas linhas */
    for (j=1;j<m;j++){ /* itera nas colunas */
      scanf("%d", &horizont[i*m + j]);
      printf("%d ", horizont[i*m + j]);
    } printf("\n");
  } printf("\n");

  /* input 5: capacidade entre vertices na vertical */
  for (i=0;i<n-1;i++) { /* itera nas linhas */
    for (j=1;j<=m;j++){  /* itera nas colunas */
      scanf("%d", &vertical[i*m + j]);
      printf("%d ", vertical[i*m + j]);
    } printf("\n");
  } printf("\n");

  /* creating the datastructure for the edges */
  /*offset = (int*) malloc(V*sizeof(int));
  edges = (Edge*) malloc(num_edges*sizeof(Edge));*/

  printf("SIIIIIIIIIIIM\n");

  return 0;
}
