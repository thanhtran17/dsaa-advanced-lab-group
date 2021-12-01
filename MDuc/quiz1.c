#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"
//------------------------------------------------------
typedef struct {
  JRB edges;
  JRB vertices;
} Graph;
//------------------------------------------------------
Graph createGraph();
void topologicalSort(Graph g);
void dropGraph(Graph graph);

void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
//------------------------------------------------------
int main()
{
  int output[10]; 
  Graph g = createGraph();
  addVertex(g, 0, "V0");
  addVertex(g, 1, "V1");
  addVertex(g, 2, "V2");
  addVertex(g, 3, "V3");
  addEdge(g, 1, 0);
  addEdge(g, 1, 2);
  addEdge(g, 2, 0);
  addEdge(g, 1, 3);
  int in = indegree(g, 0, output);
  printf("%d\n", in);
  for (int i=0; i < in; i++){
    printf("%3d", output[i]);
  }
  printf("\n");
  dropGraph(g);
  return 0;
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
void addEdge(Graph graph, int v1, int v2){
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL){
    JRB tree = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    jrb_insert_int(tree, v2, new_jval_i(1));
  }
  else {
    JRB tree = (JRB)jval_v(node->val);
    jrb_insert_int(tree, v2, new_jval_i(1));
  }
}
//------------------------------------------------------
int hasEdge(Graph graph, int v1, int v2){
  JRB node = jrb_find_int(graph.edges, v1);
  JRB tree = (JRB)jval_v(node->val);
  if (jrb_find_int(tree, v2) == NULL) return 0;
  return 1; 
}
//------------------------------------------------------
int indegree(Graph graph, int v1, int* output){
  int total = 0;
  JRB node;
  jrb_traverse(node, graph.edges){
    JRB tree = (JRB)jval_v(node->val);
    JRB node2;
    jrb_traverse(node2, tree){
      if(jval_i(node2->key) == v1) output[total++] = jval_i(node->key);
    }
  }
  return total;
};
//------------------------------------------------------
int outdegree(Graph graph, int v1, int* output){
  int total = 0;
  JRB node = jrb_find_int(graph.edges, v1);
  JRB tree = (JRB)jval_v(node->val);
  jrb_traverse(node, tree){
    output[total++] = jval_i(node->key);
  }
  return total;
}
//------------------------------------------------------
int DFS(Graph graph, int start, int stop){
  int a[100]; //ham luu cac dinh da xet
  memset(a, 0, sizeof(int)*100);
  Dllist item;
  Dllist stack = new_dllist();
  int output[100];
  int vertex;
  int n;

  dll_prepend(stack, new_jval_i(start));
  while (!dll_empty(stack)){
    item = dll_first(stack);
    vertex = jval_i(item->val);
    dll_delete_node(item);
    if (a[vertex] == 0){
      a[vertex] = 1;
      if (vertex == stop) return;
      n = getAdjacentVertices(graph, vertex, output);
      for (int i = 0; i < n; i++){
        if (a[output[i]] == 0) dll_prepend(stack, new_jval_i(output[i]));
        if (a[output[i]] == 1) return 1;
      }
    }
  }
  return 0;
}
//------------------------------------------------------
