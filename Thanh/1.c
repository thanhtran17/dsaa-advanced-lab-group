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
//------------------------------------------------------
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
int getVertexID(Graph graph, char c);

void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);

void swap(int *a ,int *b);

void insertQueue(JRB q, int id, int freq);
int findMinInQueue(JRB queue, int *freq);
void delInQueue(JRB queue, int id);
HuffmanTree makeHuffman(char *buffer, int size);

void find_code(HuffmanTree H, int ID, Coding htable[], char code[]);
void createHuffmanTable(HuffmanTree H, Coding htable[]);
void compress(char input[], char output[], Coding huffmanTable[]);//------------------------------------------------------
int main()
{
  // create a huffman tree;
  char buffer[512] = "Home Watch TV Show Family Guy - Season 1 - Episode 1 : Death Has a Shadow";
  HuffmanTree huf = makeHuffman(buffer, strlen(buffer));

  // create a code table for each characters
  char output[512];
  Coding huffmanTable[256];  
  createHuffmanTable(huf, huffmanTable);
  compress(buffer, output, huffmanTable);

  // print output 
  printf("  Original data: %s\n", buffer);
  printf("Compressed data: %s\n", output);

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
int getVertexID(Graph graph, char c)
{
  JRB node;
  int id = 0;

  jrb_traverse(node, graph.vertices)
  {
    if (c == jval_c(node->val))
    {
      id = jval_i(node->key);
    }
  }

  return id;
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
void insertQueue(JRB q, int id, int freq)
{
  JRB node = jrb_find_int(q, id);

  if (node == NULL) {
    jrb_insert_int(q, id, new_jval_i(freq));
  }
  else {
    // cộng dồn value của node
    int newFreq = jval_i(node->val); // trả về kiểu int
    newFreq += freq; 
    node->val = new_jval_i(newFreq); // -> đưa về kiểu jval_i
  }
}
//------------------------------------------------------
int findMinInQueue(JRB queue, int *freq)
{
  JRB node;
  int min = 2147483647, id  = 0;

  jrb_traverse(node, queue){
    if (jval_i(node->val) < min){
      min = jval_i(node->val);
      id = jval_i(node->key);
    }
  }

  *freq = min;

  return id;
}
//------------------------------------------------------
void delInQueue(JRB queue, int id)
{
  JRB node;

  node = jrb_find_int(queue, id);
  jrb_delete_node(node);
}
//------------------------------------------------------
HuffmanTree makeHuffman(char *buffer, int size)
{
  // Create a Huffmantree
  HuffmanTree H;
  H.graph.edges    = make_jrb();
  H.graph.vertices = make_jrb();

  // Make a queue;
  JRB queue = make_jrb();

  // Insert each character and its freq into queue
  int count = 0;
  for (int i = 0; i < size; i++) {
    int id = getVertexID(H.graph, buffer[i]);
    if (id == 0) {
      count++;
      id = count;
      addVertex(H.graph, id, buffer[i]);
      insertQueue(queue, id, 1);
    }
    else {
      insertQueue(queue, id , 1);
    }
  }

  // build the huffman tree
  
  H.root = jrb_find_int(H.graph.vertices, count);

  return H;
}
//------------------------------------------------------
void compress(char input[], char output[], Coding huffmanTable[])
{
  for (int i = 0; i < strlen(input); i++)
  {
    strcat(output, huffmanTable[input[i]].bits);
  }
}
//------------------------------------------------------
void createHuffmanTable(HuffmanTree H, Coding htable[])
{

}
