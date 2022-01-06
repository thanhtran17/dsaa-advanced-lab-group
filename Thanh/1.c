#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include <math.h>
#include <stdbool.h>
//----------------------------------------------------------
int edge_number = 0;
//----------------------------------------------------------
typedef struct node
{
  double val;
  int parent;
} node_;

typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;
//----------------------------------------------------------
typedef struct
{
  Graph graph;
  JRB root;
} HuffmanTree;
typedef struct
{
  int size;
  char bits[20];
} Coding;
//----------------------------------------------------------
void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}
//----------------------------------------------------------
Coding huffmanTable[256];
Graph createGraph();
JRB addVertex(Graph graph, int id, char name);
char getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
HuffmanTree makeHuffman(char *buffer, int size);
void createHuffmanTable(HuffmanTree htree, Coding *htable);
void compress(char *buffer, char *output, Coding huffmanTable[]);
//----------------------------------------------------------
int main()
{
  Graph graph = createGraph();
  int path[256], output[256], total, choice, n = 0;
  int v1, v2, id, length;
  char output_huffman[256] = "", name[256];
  char buffer[256] = "Lorem Ipsum is containing Lorem Ipsum passages, and mo";
  HuffmanTree huf = makeHuffman(buffer, strlen(buffer));
  
  createHuffmanTable(huf, huffmanTable);
  compress(buffer, output_huffman, huffmanTable);

  printf("Original Data: %s\n", buffer);
  printf("Compressed Data: %s\n", output_huffman);

  return 0;
}
//----------------------------------------------------------
int getVertexId(Graph graph, char val)
{
  JRB node;
  int id = 0;
  
  jrb_traverse(node, graph.vertices){
    if (val == jval_c(node->val)){
      id = jval_i(node->key);
    }
  }

  return id;
}
//----------------------------------------------------------
void insertQueue(JRB q, int id, int freq)
{
  JRB node;
  node = jrb_find_int(q, id);

  if (node == NULL){
    jrb_insert_int(q, id, new_jval_i(freq));
  }
  else{
    int newFreq = jval_i(node->val);
    newFreq += freq;
    node->val = new_jval_i(newFreq);
  }
}
//----------------------------------------------------------
int findMinInQueue(JRB queue, int *freq)
{
  JRB node;
  int min = 999999999;
  int id = 0;

  jrb_traverse(node, queue){
    if (jval_i(node->val) < min)
    {
      min = jval_i(node->val);
      id = jval_i(node->key);
    }
  }

  *freq = min;

  return id;
}
//----------------------------------------------------------
void delInQueue(JRB queue, int id)
{
  JRB node;

  node = jrb_find_int(queue, id);
  jrb_delete_node(node);
}
//----------------------------------------------------------
void find_code(HuffmanTree H, int root_key, Coding *htable, char code[])
{
  JRB node, tree, buf;
  char c;

  node = jrb_find_int(H.graph.edges, root_key);

  if (node == NULL){
    c = getVertex(H.graph, root_key);
    strcpy(htable[c].bits, code);
    htable[c].size = strlen(code);
    printf("%c %s \n", c, htable[c].bits);
  }
  else{
    JRB child;
    char backup[256] = "";

    tree = (JRB)jval_v(node->val);

    jrb_traverse(child, tree){
      if (jval_d(child->val) == 0) {
        strcpy(backup, code);
        strcat(code, "0");
        find_code(H, jval_i(child->key), htable, code);
        strcpy(code, backup);
      }
      if (jval_d(child->val) == 1) {
        strcpy(backup, code);
        strcat(code, "1");
        find_code(H, jval_i(child->key), htable, code);
        strcpy(code, backup);
      }
    }
  }
}
//----------------------------------------------------------
void createHuffmanTable(HuffmanTree H, Coding htable[])
{
  char code[256] = "";

  for (int i = 0; i < 256; i++){
    htable[i].size = 0;
  }

  find_code(H, jval_i(H.root->key), htable, code);
}
//----------------------------------------------------------
void compress(char buffer[], char *output, Coding huffmanTable[])
{
  for (int i = 0; i < strlen(buffer); i++){
    strcat(output, huffmanTable[buffer[i]].bits);
  }
}
//----------------------------------------------------------
HuffmanTree makeHuffman(char *buffer, int size)
{
  HuffmanTree H;
  H.graph = createGraph();
  H.root = make_jrb();
  JRB queue = make_jrb();
  JRB noder = NULL;
  int count = 0;

  for (int i = 0; i < size; i++){
    int id = getVertexId(H.graph, buffer[i]);

    if (id == 0){
      count++;
      id = count;
      addVertex(H.graph, id, buffer[i]);
      insertQueue(queue, id, 1);
    }
    else {
      insertQueue(queue, id, 1);
    }
  }

  while (1){
    int node_1, node_2;

    int id_node_1 = findMinInQueue(queue, &node_1);
    if (id_node_1 == 0)
      break;
    delInQueue(queue, id_node_1);

    int id_node_2 = findMinInQueue(queue, &node_2);
    if (id_node_2 == 0)
      break;
    delInQueue(queue, id_node_2);

    count++;
    insertQueue(queue, count, node_1 + node_2);
    addVertex(H.graph, count, 'i');
    addEdge(H.graph, count, id_node_1, 0);
    addEdge(H.graph, count, id_node_2, 1);
  }

  H.root = jrb_find_int(H.graph.vertices, count);

  return H;
};
//----------------------------------------------------------
Graph createGraph()
{
  Graph g;

  g.edges    = make_jrb();
  g.vertices = make_jrb();

  return g;
}
//----------------------------------------------------------
JRB addVertex(Graph graph, int id, char name)
{
  edge_number++;
  JRB node = jrb_find_int(graph.vertices, id);

  if (node == NULL){
    return jrb_insert_int(graph.vertices, id, new_jval_c(name));
  }
  node->val = new_jval_c(name);

  return node;
}
//----------------------------------------------------------
char getVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);

  return (node == NULL) ? -1 : jval_c(node->val);
}
//----------------------------------------------------------
void addEdge(Graph graph, int v1, int v2, double weight)
{
  // Add to vertex 1
  JRB node = jrb_find_int(graph.edges, v1);

  if (node == NULL) {
    JRB tree = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
  else{
    JRB tree = (JRB)jval_v(node->val);
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
}
//----------------------------------------------------------
void printVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);
  
  if (node == NULL){
    printf("There are no id like this\n");

    return;
  }

  printf("%c\n", jval_c(node->val));

  return;
}
//----------------------------------------------------------
