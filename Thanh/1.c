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
  Graph graph;
  JRB root;
} HuffmanTree;

typedef struct {
  int size;
  char bits[10];
} Coding;
//------------------------------------------------------
Graph createGraph();
void dropGraph(Graph graph);

void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);

void swap(int *a ,int *b);

void insertQueue(JRB q, int id, int freq);
int findMinInQueue(JRB queue, int *freq);
void delInQueue(JRB queue, int id);
HuffmanTree makeHuffman(char *buffer, int size);

void find_code(HuffmanTree H, int ID, Coding htable[], char code[]);
void createHuffmanTable(HuffmanTree H, Coding htable[]);
void compress(char *input, char *output, Coding huffmanTable[]);
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
//------------------------------------------------------
void swap(int *a ,int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
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
double getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  
  if (node == NULL)
    return 0;

  JRB end = jrb_find_int(jval_v(node->val), v2);

  return (end == NULL) ? 0 : jval_d(end->val);
}
//------------------------------------------------------
