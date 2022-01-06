#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"
//------------------------------------------------------
void clear(){
   while (getchar() != '\n');
}
//------------------------------------------------------
typedef struct {
  JRB edges;
  JRB vertices;
} Graph;

typedef struct {
  int d;
  int parent;
} node_;
//------------------------------------------------------
Graph createGraph();
void printMatrix(Graph g);
void dropGraph(Graph graph);
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void getAdjacentVertices(Graph g);
void mostIndegree(Graph g, int* maxIndegree);
void addEdge(Graph graph, int v1, int v2, int weight);
int hasEdge(Graph graph, int v1, int v2);
int getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int output[]);
int outdegree(Graph graph, int v, int output[]);
int DFS(Graph g, int start);
void insertionSort(int a[], int n, node_ node[]);
int shortestPath(Graph graph, int s, int t, int *path, int *length);
int shortestWalkingPath(Graph graph, int s, int t, int *pathW, int *lengthW);
//------------------------------------------------------
int main()
{
  // OPEN INPUT FILE
  char* inputPath = "dothi.txt";
  int m, n, a[500];

  int matrix[255][255];
  FILE *fi = fopen(inputPath, "rt");
  
  if (fi == NULL){
    printf("ERROR opening input file\n");
    return -1;
  }

  // GRAPH HANDLING
  Graph g = createGraph();
  int id;
  char buf[BUFSIZ];
  char name[10];
  int choice;
  int v1, v2, time;
  int nonWalking[255];
  int maxIndegree = -1;
  int output[100], numOfAdj;
  int id1, id2, path[255], length, totaltime;
  int id1W, id2W, pathW[255], lengthW, totaltimeW;
  JRB node;
  
  do {
    printf("-----------------Menu-----------------\n");
    printf("1. Read File & Print Matrix\n");
    printf("2. Print Adjacent Vertices\n");
    printf("3. a. Walking\n   b. Highest Indegree\n");
    printf("4. Shortest Path (s to t)\n");
    printf("5. Shortest Walking Path (s to t)\n");
    printf("6. Delete graph and exit\n");
    printf("--------------------------------------\n");
    printf("--> Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    // Read file and print matrix
    case 1:
      printf("\nREAD FILE AND PRINT MATRIX\n\n");
      clear();
      fscanf(fi, "%d %d", &m, &n);

      for (int i = 1; i <= m; i++){
          sprintf(buf, "%d", i);
          addVertex(g, i, buf);
      }

      for (int i = 0; i < n; i++){
        fscanf(fi, "%d %d %d", &v1, &v2, &time);

        if (time < 50) {
          nonWalking[v1] = 1;
          nonWalking[v2] = 1;
        }
       
        addEdge(g, v1, v2, time);

        matrix[v1][v2] = time;
        matrix[v2][v1] = time;
      }

      printf("     ");
      for (int i = 1; i <= m; i++){
        printf("%4d:", i);
      }
      printf("\n\n");

      for (int i = 1; i <= m; i++) {
        printf("%d:   ", i);
        for (int j = 1; j <= m; j++) {
          printf("%4d ", matrix[i][j]);
        }
        printf("\n");
      }
      printf("\n");

      break;
    case 2:
      printf("\nPRINT ADJACENT VERTICES\n\n");
      getAdjacentVertices(g);
      printf("\n");
      break;
    case 3: 
      printf("\na. WALKING VERTICES: \n\n");
      for (int i = 1; i <= m; i++) {
        if (nonWalking[i] != 1){
          printf("%4d", i);
        }
      }
      printf("\n\n");
      printf("\nb. VERTEX WITH MOST INDEGREE: \n\n");
      mostIndegree(g, &maxIndegree);
      
      break;
    case 4:
      printf("\nSHORTEST PATH FROM S TO T\n\n");
      printf("--> Enter start and destination: ");
      scanf("%d %d", &id1, &id2);

      totaltime = shortestPath(g, id1, id2, path, &length);

      if (totaltime == 999999) {
        printf("   ERR! Route not found\n\n");
      }
      else {
        printf("\n   Total time: %d hours\n\n   Path      : ", totaltime);
        for (int i = length - 1; i >= 0; i--){
          printf("%d -> ", path[i]);
        }
        printf("%d\n\n", id2);
      }

     break;
    case 5:
      printf("\nSHORTEST \"WALKING\" PATH FROM S TO T\n\n");
      printf("--> Enter start and destination: ");
      scanf("%d %d", &id1, &id2);

      totaltimeW = shortestWalkingPath(g, id1, id2, pathW, &lengthW);

      if (totaltimeW == 999999) {
        printf("   ERR! Route not found\n\n");
      }
      else {
        printf("\n   Total time: %d hours\n\n   Path      : ", totaltimeW);
        for (int i = lengthW - 1; i >= 0; i--){
          printf("%d -> ", pathW[i]);
        }
        printf("%d\n\n", id2);
      }

      break;
    }
  } while (choice != 6);

  return 0;
}
//------------------------------------------------------
Graph createGraph()
{
  Graph g;
  g.edges    = make_jrb();
  g.vertices = make_jrb();
  return g;
}
//------------------------------------------------------
void dropGraph(Graph graph)
{
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
  return; // NOTE
}
//------------------------------------------------------
void addVertex(Graph graph, int id, char *name)
{
  JRB node = jrb_find_int(graph.vertices, id);

  if (node == NULL){
    jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
  }
  else{
    strcpy(jval_s(node->val), name);
  }
}
//------------------------------------------------------
char *getVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);

  if (node == NULL){
    return NULL;
  }
  else{
    return (jval_s(node->val));
  }
}
//------------------------------------------------------
void addEdge(Graph graph, int v1, int v2, int weight){
  JRB node1 = jrb_find_int(graph.edges, v1);
  JRB node2 = jrb_find_int(graph.edges, v2);

  if (node1 == NULL){
    JRB tree1 = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(tree1));
    jrb_insert_int(tree1, v2, new_jval_i(weight));
  }
  else {
    JRB tree1 = (JRB)jval_v(node1->val);
    jrb_insert_int(tree1, v2, new_jval_i(weight));
  }

  if (node2 == NULL){
    JRB tree2 = make_jrb();
    jrb_insert_int(graph.edges, v2, new_jval_v(tree2));
    jrb_insert_int(tree2, v1, new_jval_i(weight));
  }
  else {
    JRB tree2 = (JRB)jval_v(node2->val);
    jrb_insert_int(tree2, v1, new_jval_i(weight));
  }
}
//------------------------------------------------------
int hasEdge(Graph graph, int v1, int v2){
  JRB node = jrb_find_int(graph.edges, v1);
  JRB tree = (JRB)jval_v(node->val); 
  
  if (jrb_find_int(tree, v2) == NULL) 
    return 0;

  return 1; 
}
//------------------------------------------------------
int indegree(Graph graph, int v, int output[])
{
  int total = 0;
  JRB node = NULL;

  jrb_traverse(node, graph.edges)
  {
    if (node->key.i == v)
      continue;
    if (jrb_find_int((JRB) (jval_v(node->val)), v) != NULL) // tìm giá trị v trong cây node
      output[total++] = jval_i(node->key);
  }

  return total;
}
//------------------------------------------------------
int outdegree(Graph graph, int v, int output[])
{
  int total = 0;
  JRB node = jrb_find_int(graph.edges, v);
  
  if (node == NULL)
   return -1;

  JRB tree = (JRB) jval_v(node->val);

  jrb_traverse(node, tree)
  {
    output[total++] = jval_i(node->key);
  }

  return total;
}
//------------------------------------------------------
int DFS(Graph g, int start)
{
  int visited[100], vertex, output[100], n, res;
  memset(visited, 0, sizeof(int) * 100);

  Dllist node;
  Dllist stack = new_dllist();

  dll_prepend(stack, new_jval_i(start));

  while (!dll_empty(stack)){
    node = dll_first(stack);
    vertex = jval_i(node->val);
    dll_delete_node(node);
    if (visited[vertex] == 0){
      visited[vertex] = 1;
      n  = outdegree(g, vertex, output);
      for (int i = 0; i < n; i++) {
        if (output[i] == start){
          res = 1;
        }
        if (visited[output[i]] == 0) {
          dll_prepend(stack, new_jval_i(output[i]));
        }
      }
    }
  }

  return res;
}
//------------------------------------------------------
void getAdjacentVertices(Graph g)
{
  JRB temp;

  jrb_traverse(temp, g.vertices){
    int output[100];
    int size = outdegree(g, temp->key.i, output);
    
    printf("Castle %d: ", temp->key.i);

    if (size > 0){
      for (int i = 0; i < size; i++){
        printf("%4d", output[i]);
      }
    }  
    else {
      printf("No adjacent vertices");
    }

    printf("\n");
  }
}
//------------------------------------------------------
void mostIndegree(Graph g, int* maxIndegree)
{
  JRB node; 

  jrb_traverse(node, g.vertices) {
    int out[255];
    int nOi = indegree(g, jval_i(node->key), out);
    
    if (nOi >= (*maxIndegree)) {
      (*maxIndegree) = nOi;
    }
  }

  jrb_traverse(node, g.vertices) {
    int out[255];
    int nOi = indegree(g, jval_i(node->key), out);
    
    if (nOi == (*maxIndegree)) {
      printf("%4d", jval_i(node->key));
    }
  }

  printf("\n\n");
}
//------------------------------------------------------
int shortestPath(Graph graph, int s, int t, int *path, int *length)
{
  JRB ver_tmp;
  node_ node[100];
  int pQ[1000];
  int number = 0; // index trong pQueue
  *length = 0;

  // set tổng giá trị đường đi nhỏ nhất của từng vertex thành infinity
  jrb_traverse(ver_tmp, graph.vertices){
    pQ[number++] = jval_i(ver_tmp->key);
    node[jval_i(ver_tmp->key)].d = 999999;
    node[jval_i(ver_tmp->key)].parent = -1;
  }

  // set giá trị đường đi ở điểm xuất phát là 0
  node[s].d = 0;

  // v là 1 adjacent của u
  int u, v;
  int outdegreeArray[100];
  int total;

  // lặp lại đến khi nào pQueue hết phần tử
  while (number != 0) {
    insertionSort(pQ, number, node);
    u = pQ[number-1];
    // tìm các outD của phần tử đầu trong pQueue
    total = outdegree(graph, u, outdegreeArray);
    // duyệt toàn bộ các outD của phân tử đang xét trong pQueue
    for (int k = 0; k < total; k++) {
      v = outdegreeArray[k];
      if (node[v].d > node[u].d + getEdgeValue(graph, u, v)) {
        node[v].d = node[u].d + getEdgeValue(graph, u, v);
        node[v].parent = u;
      }
    }
    number--;
  }

  node_ destination = node[t];
  int total_weight = destination.d;

  while (destination.parent != -1) {
    path[(*length)++] = destination.parent;
    destination = node[destination.parent];
  }

  return total_weight;
}
//------------------------------------------------------
void insertionSort(int a[], int n, node_ node[])
{
  int temp;
  int j;

  for (int i = 0; i < n; i++){
    temp = a[i];
    for (j = i - 1; j >= 0; j--){
      if (node[a[j]].d < node[temp].d){
        a[j + 1] = a[j];
      } else{
        break;
      }
    }
    a[j + 1] = temp;
  }
}
//------------------------------------------------------
int getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  
  if (node == NULL)
    return 0;

  JRB end = jrb_find_int(jval_v(node->val), v2);

  return (end == NULL) ? 0 : jval_i(end->val);
}
//------------------------------------------------------
int shortestWalkingPath(Graph graph, int s, int t, int *pathW, int *lengthW)
{
  JRB ver_tmp;
  node_ node[100];
  int pQ[1000];
  int number = 0; // index trong pQueue
  *lengthW = 0;

  // set tổng giá trị đường đi nhỏ nhất của từng vertex thành infinity
  jrb_traverse(ver_tmp, graph.vertices){
    pQ[number++] = jval_i(ver_tmp->key);
    node[jval_i(ver_tmp->key)].d = 999999;
    node[jval_i(ver_tmp->key)].parent = -1;
  }

  // set giá trị đường đi ở điểm xuất phát là 0
  node[s].d = 0;

  // v là 1 adjacent của u
  int u, v;
  int outdegreeArray[100];
  int total;

  // lặp lại đến khi nào pQueue hết phần tử
  while (number != 0) {
    insertionSort(pQ, number, node);
    u = pQ[number-1];
    // tìm các outD của phần tử đầu trong pQueue
    total = outdegree(graph, u, outdegreeArray);
    // duyệt toàn bộ các outD của phân tử đang xét trong pQueue
    for (int k = 0; k < total; k++) {
      v = outdegreeArray[k];
      if ((node[v].d > node[u].d + getEdgeValue(graph, u, v))) {
        if (getEdgeValue(graph, u, v) >= 50) {
          node[v].d = node[u].d + getEdgeValue(graph, u, v);
          node[v].parent = u;
        }
      }
    }
    number--;
  }

  node_ destination = node[t];
  int total_weight = destination.d;

  while (destination.parent != -1) {
    pathW[(*lengthW)++] = destination.parent;
    destination = node[destination.parent];
  }

  return total_weight;
}