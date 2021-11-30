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
int DAG(Graph graph);
//------------------------------------------------------
int main()
{
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
