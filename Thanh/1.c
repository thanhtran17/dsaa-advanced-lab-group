#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
//------------------------------------------------------
typedef struct _Graph {
  JRB edges; 
  JRB vertices;
} Graph;
//------------------------------------------------------
void printMenu();

Graph createGraph();
void topologicalSort(Graph g);
void dropGraph(Graph graph);

void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int indegree(Graph graph, int v, int output[]);
int outdegree(Graph graph, int v, int output[]);
int DAG(Graph graph);
//------------------------------------------------------
int main()
{
  int choice;
  
  printMenu();

  do {
    printf("?? Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      printf("--> 1\n");
      break;
    case 2:
      printf("--> 2\n");
      break;
    default: 
      return;
    }
  } 
  while (choice != 0); 

  return 0;
}
//------------------------------------------------------
void printMenu()
{
  printf("---- MENU ---- \n");
  printf("");
}

//------------------------------------------------------
Graph createGraph()
{
  Graph graph;

  graph.edges = make_jrb();
  graph.vertices = make_jrb();

  return graph;
}
//------------------------------------------------------
void dropGraph(Graph graph)
{
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
}
//------------------------------------------------------
void addVertex(Graph graph, int id, char *name)
{
  JRB node = jrb_find_int(graph.vertices, id);

  if (node == NULL){
    jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
  }
  else {
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
  else {
    return (jval_s(node->val));
  }
}
//------------------------------------------------------
void addEdge(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);

  if (node = NULL){
    JRB v1_tree = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(v1_tree));
    jrb_insert_int(v1_tree, v2, new_jval_i(1));
  }
  else {
    JRB v1_tree = (JRB) jval_v(node->val);
    jrb_insert_int(v1_tree, v2, new_jval_i(1));
  }
}
//------------------------------------------------------
int hasEdge(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  JRB v1_tree = (JRB) jval_v(node->val);
  JRB find = jrb_find_int(v1_tree, v2);
  if (find == NULL)
    return 0;
  else 
    return 1;
}
//------------------------------------------------------
int indegree(Graph graph, int v, int output[])
{
  int total = 0;
  JRB node;

  jrb_traverse(node, graph.edges)
  {
    if (node->key.i == v)
      continue;
    if (jrb_find_int((JRB) (jval_v(node->val)), v) != NULL)
      output[total++] = node->key.i;
  }

  return total;
}
//------------------------------------------------------
int outdegree(Graph graph, int v, int output[])
{
  int total = 0;
  JRB node = jrb_find_int(graph.edges, v);
  JRB tree = (JRB) jval_v(node->val);

  jrb_traverse(node, tree)
  {
    output[total++] = jval_i(node->key);
  }

  return total;
}
//------------------------------------------------------
int DAG(Graph graph)
{
  
}
