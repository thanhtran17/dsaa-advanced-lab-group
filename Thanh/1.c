#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"
#include <math.h>
#include <stdbool.h>
//------------------------------------------------------
typedef struct {
  JRB edges;
  JRB vertices;
} Graph;

typedef struct {
  double d;
  int parent;
} node_;
//------------------------------------------------------
void insertionSort(int a[], int n, node_ node[]);

Graph createGraph();
void dropGraph(Graph graph);
void topologicalSort(Graph g, int output[], int* n);
void printGraph(Graph g);

void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2, double weight);
int hasEdge(Graph graph, int v1, int v2);
double getEdgeValue(Graph graph, int v1, int v2);

int indegree(Graph graph, int v, int output[]);
int outdegree(Graph graph, int v, int output[]);

int DFS(Graph g, int start);
int DAG(Graph graph);

double shortestPath(Graph graph, int s, int t, int *path, int *length);
//------------------------------------------------------
int main()
{
  // general's
  int output[500]; 
  Graph graph = createGraph();
  int id;
  char name[10];
  int choice;
  int v1, v2;
  int count = 0, topo[10];
  JRB node;

  // shortest's 
  double weight;
  int path[500], length;

  addVertex(graph, 1, "s");
  addVertex(graph, 2, "2");
  addVertex(graph, 3, "3");
  addVertex(graph, 4, "4");
  addVertex(graph, 5, "5");
  addVertex(graph, 6, "6");
  addVertex(graph, 7, "7");
  addVertex(graph, 8, "t");

  addEdge(graph, 1, 2, 9);
  addEdge(graph, 2, 3, 24);
  addEdge(graph, 1, 6, 14);
  addEdge(graph, 6, 3, 18);
  addEdge(graph, 6, 5, 30);
  addEdge(graph, 5, 4, 11);
  addEdge(graph, 4, 8, 6);
  addEdge(graph, 4, 3, 6);
  addEdge(graph, 3, 5, 2);
  addEdge(graph, 3, 8, 19);
  addEdge(graph, 5, 8, 16);
  addEdge(graph, 1, 7, 15);
  addEdge(graph, 7, 6, 5);
  addEdge(graph, 7, 5, 20);
  addEdge(graph, 7, 8, 44);

  printf("Value: %g\n", shortestPath(graph, 1, 8, path, &length));

  for (int i = length - 1; i >= 0; i--){
    printf("%s->", getVertex(graph, path[i]));
  }
  
  printf("t\n");

  return 0;
}
////////////////////////////////////////////////
////////////////////////////////////////////////
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
Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
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
void addEdge(Graph graph, int v1, int v2, double weight){
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL){
    JRB tree = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
  else {
    JRB tree = (JRB)jval_v(node->val);
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
}
//------------------------------------------------------
int hasEdge(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  JRB tree = (JRB)jval_v(node->val); // tạo 1 tree bằng đầu với node v1
  if (jrb_find_int(tree, v2) == NULL) 
    return 0;
  return 1; 
}
//------------------------------------------------------
double getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  
  if (node == NULL)
    return 0;

  JRB end = jrb_find_int(jval_v(node->val), v2);

  return (end == NULL) ? 0 : jval_d(end->val);
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
int DAG(Graph graph)
{
  JRB node;
  int exist = 0;
  jrb_traverse(node, graph.vertices)
  {
    exist = DFS(graph, jval_i(node->key));
  }

  return exist;
}
//------------------------------------------------------
void topologicalSort(Graph g, int output[], int* n)
{
  Dllist node;
  Dllist queue = new_dllist();

  JRB vertex;
  int temp[100], mark[100], count;

  jrb_traverse(vertex, g.vertices){
    if (indegree(g, jval_i(vertex->key), temp) == 0) {
      dll_append(queue, vertex->key);
    }
    mark[jval_i(vertex->key)] = indegree(g, jval_i(vertex->key), temp);
  }

  while (!dll_empty(queue)) {
    node = dll_first(queue);
    dll_delete_node(node);
    output[(*n)++] = jval_i(node->val);
    count = outdegree(g, jval_i(node->val), temp);
    for (int i = 0; i < count; i++) {
      mark[temp[i]]--;
      if (mark[temp[i]] == 0)
        dll_append(queue, new_jval_i(temp[i]));
    }
  }
}
//------------------------------------------------------
double shortestPath(Graph graph, int s, int t, int *path, int *length)
{
  JRB ver_tmp;
  node_ node[100];
  int pQ[1000];
  int number = 0; // index trong pQueue
  *length = 0;

  // set tổng giá trị đường đi nhỏ nhất của từng vertex thành infinity
  jrb_traverse(ver_tmp, graph.vertices){
    pQ[number++] = jval_i(ver_tmp->key);
    node[jval_i(ver_tmp->key)].d = INFINITY;
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
