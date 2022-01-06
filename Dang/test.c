#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include <math.h>
#include <stdbool.h>
#define INT_MAX 999999
typedef struct node
{
  double val;
  int parent;
} node_;
typedef struct
{
  double weight;
  int node_key_a;
  int node_key_b;
} edge_;
int edge_number = 0;
typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;
void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}
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
typedef struct
{
  int size;
  int *nodes;
} HuffmanTreeArray;
Coding huffmanTable[256];
Graph createGraph();
JRB addVertex(Graph graph, int id, char name);
char getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
HuffmanTree makeHuffman(char *buffer, int size);
void createHuffmanTable(HuffmanTree htree, Coding *htable);
int KrusalTree(Graph graph, double output[256], int start);
void compress(char *buffer, char *output, Coding huffmanTable[]);
void greedyColoring(Graph graph, int size);
double getEdgeValue(Graph graph, int v1, int v2);
void createHuffmanArray(HuffmanTree H, int root_key, HuffmanTreeArray arr, int *current_length);
int outdegree(Graph graph, int v, int *output);
int indegree(Graph graph, int v, int *output, double *weight);
void readFile(Graph graph);
void printCastle(Graph graph);
void insertionSort(int a[], int n, node_ node[]);
void bestWaysBetweenTwoCastles(Graph graph, int v1, int v2);
void addEdgeSingle(Graph graph, int v1, int v2, double weight);
void bestWalkWaysBetweenTwoCastles(Graph graph, int v1, int v2);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
void findWalkWay(Graph graph);
void findMaxWays(Graph graph);
double getEdge(Graph graph, int v1, int v2);
int main()
{
  FILE *fp_2 = fopen("output.txt", "r");
  FILE *fp_1 = fopen("input.txt", "r");

  Graph graph = createGraph();
  int output[256];
  int total;
  int choice;
  int n = 0;
  int v1, v2;
  int id;
  char name[256];
  char code[256] = "";
  double weight[256];
  char output_huffman[256] = "";
  int path[256];
  double output_d[256];
  int length;
  readFile(graph);
  printCastle(graph);
  findWalkWay(graph);
  findMaxWays(graph);
  bestWaysBetweenTwoCastles(graph, 2, 8);
  bestWalkWaysBetweenTwoCastles(graph, 2, 8);
  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
  // Huffman
  char buffer[256] = "we will attack tomorrow";
  HuffmanTree huf = makeHuffman(buffer, strlen(buffer));
  printf("Buffer: %s\n", buffer);
  createHuffmanTable(huf, huffmanTable);
  compress(buffer, output_huffman, huffmanTable);
  printf("%s\n", output_huffman);
  return 0;
}
double getEdge(Graph graph, int v1, int v2)
{
  JRB tree = jrb_find_int(graph.edges, v1);
  JRB node = jrb_find_int(jval_v(tree->val), v2);
  return jval_d(node->val);
}

void insertionSort(int a[], int n, node_ node[])
{
  int temp;
  int j;
  for (int i = 0; i < n; i++)
  {
    temp = a[i];
    for (j = i - 1; j >= 0; j--)
    {
      if (node[a[j]].val < node[temp].val)
        a[j + 1] = a[j];
      else
        break;
    }
    a[j + 1] = temp;
  }
}
double shortestPath(Graph graph, int s, int t, int *path, int *length)
{
  JRB node;
  node_ node_arr[100];
  int queue[1000];
  int number = 0;
  *length = 0;
  int u;
  int v;
  int total;
  int outdegree_arr[30];
  jrb_traverse(node, graph.vertices)
  {
    queue[number++] = jval_i(node->key);
    node_arr[jval_i(node->key)].val = INT_MAX;
    node_arr[jval_i(node->key)].parent = -1;
  };
  node_arr[s].val = 0;
  while (number != 0)
  {
    insertionSort(queue, number, node_arr);
    u = queue[number - 1];
    total = outdegree(graph, u, outdegree_arr);
    for (int k = 0; k < total; k++)
    {
      v = outdegree_arr[k];
      if (node_arr[v].val > node_arr[u].val + getEdgeValue(graph, u, v))
      {
        node_arr[v].val = node_arr[u].val + getEdgeValue(graph, u, v);
        node_arr[v].parent = u;
      }
    }
    number--;
  };
  node_ destination = node_arr[t];
  int total_weight = destination.val;
  while (destination.parent != -1)
  {
    int a = destination.parent;
    path[(*length)++] = a;
    destination = node_arr[a];
  }
  return total_weight;
};
void bestWalkWaysBetweenTwoCastles(Graph graph, int v1, int v2)
{
  Graph new_graph = createGraph();
  FILE *fp = fopen("dothi.txt", "r");
  int vertexCount;
  int edgeCount;
  fscanf(fp, "%d %d", &vertexCount, &edgeCount);
  printf("V_c: %d E_c: %d\n", vertexCount, edgeCount);
  int a;
  int b;
  int c;
  while (!feof(fp))
  {
    fscanf(fp, "%d %d %d", &a, &b, &c);
    addVertex(new_graph, a, '-');
    addVertex(new_graph, b, '-');
    if (c >= 50)
    {
      addEdge(new_graph, a, b, (double)c);
    }
  }
  int path[256];
  int length = 0;
  double total = shortestPath(new_graph, v1, v2, path, &length);
  if (total == INT_MAX)
  {
    printf("ROUTE NOT FOUND\n");
    jrb_free_tree(new_graph.edges);
    jrb_free_tree(new_graph.vertices);
    return;
  }
  printf("Length: %g\n", total);
  for (int i = length - 1; i >= 0; i--)
  {
    printf("%d->", path[i]);
  }
  printf("%d\n", v2);
  jrb_free_tree(new_graph.edges);
  jrb_free_tree(new_graph.vertices);
  return;
}
void bestWaysBetweenTwoCastles(Graph graph, int v1, int v2)
{
  int path[256];
  int length = 0;
  double total = shortestPath(graph, v1, v2, path, &length);
  if (total == INT_MAX)
  {
    printf("ROUTE NOT FOUND");
    return;
  }
  printf("Length: %g\n", total);
  for (int i = length - 1; i >= 0; i--)
  {
    printf("%d->", path[i]);
  }
  printf("%d\n", v2);
  return;
}
int indegree(Graph graph, int v, int *output, double *weight)
{
  int total = 0;
  JRB node = NULL;
  jrb_traverse(node, graph.edges)
  {
    if (jval_i(node->key) == v)
    {
      continue;
    }
    JRB tree = jval_v(node->val);
    JRB child;
    jrb_traverse(child, tree)
    {
      if (jval_i(child->key) == v)
      {
        weight[total] = jval_d(child->val);
        output[total] = jval_i(child->key);
        total++;
      }
    }
  }

  return total;
}
void findWalkWay(Graph graph)
{
  int output[256];
  double weight[256];
  JRB node = NULL;
  int check = 0;
  int n;
  jrb_traverse(node, graph.vertices)
  {
    n = indegree(graph, jval_i(node->key), output, weight);
    for (int i = 0; i < n; i++)
    {
      if (weight[i] < 50)
      {
        check = 1;
      }
    }
    if (check == 0)
    {
      printf("%d ", jval_i(node->key));
    }
    check = 0;
  }
}
void findMaxWays(Graph graph)
{
  int output[256];
  double weight[256];
  int ways[256];
  JRB node = NULL;
  int max = 0;
  int n;
  jrb_traverse(node, graph.vertices)
  {
    n = indegree(graph, jval_i(node->key), output, weight);
    if (max < n)
    {
      max = n;
    }
  }
  printf("Max: ");
  jrb_traverse(node, graph.vertices)
  {
    if (indegree(graph, jval_i(node->key), output, weight) == max)
    {
      printf("%d ", jval_i(node->key));
    }
  }
  printf("\n");
}
void readFile(Graph graph)
{
  FILE *fp = fopen("dothi.txt", "r");
  int vertexCount;
  int edgeCount;
  fscanf(fp, "%d %d", &vertexCount, &edgeCount);
  printf("V_c: %d E_c: %d\n", vertexCount, edgeCount);
  int a;
  int b;
  int c;
  while (!feof(fp))
  {
    fscanf(fp, "%d %d %d", &a, &b, &c);
    addVertex(graph, a, '-');
    addVertex(graph, b, '-');
    addEdge(graph, a, b, (double)c);
  }
}
void printCastle(Graph graph)
{
  JRB node = NULL;
  jrb_traverse(node, graph.edges)
  {
    printf("Castle %d: ", jval_i(node->key));
    JRB tree = jval_v(node->val);
    JRB child;
    jrb_traverse(child, tree)
    {
      printf("%d ", jval_i(child->key));
    }
    printf("\n");
  }
}
int getVertexId(Graph graph, char val)
{
  JRB node;
  int id = 0;
  jrb_traverse(node, graph.vertices)
  {
    if (val == jval_c(node->val))
    {
      id = jval_i(node->key);
    }
  }
  return id;
}
double getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
    return 0;
  JRB endPoint = jrb_find_int(jval_v(node->val), v2);
  return (endPoint == NULL) ? 0 : jval_d(endPoint->val);
}
void insertQueue(JRB q, int id, int freq)
{
  JRB node;
  node = jrb_find_int(q, id);
  if (node == NULL)
  {
    jrb_insert_int(q, id, new_jval_i(freq));
  }
  else
  {
    int newFreq = jval_i(node->val);
    newFreq += freq;
    node->val = new_jval_i(newFreq);
  }
}
int findMinInQueue(JRB queue, int *freq)
{
  JRB node;
  int min = 999999999;
  int id = 0;
  jrb_traverse(node, queue)
  {
    if (jval_i(node->val) < min)
    {
      min = jval_i(node->val);
      id = jval_i(node->key);
    }
  }
  *freq = min;
  return id;
}
void delInQueue(JRB queue, int id)
{
  JRB node;
  node = jrb_find_int(queue, id);
  jrb_delete_node(node);
}

HuffmanTree makeHuffman(char *buffer, int size)
{
  HuffmanTree H;
  H.graph = createGraph();
  H.root = make_jrb();
  JRB queue = make_jrb();
  JRB noder = NULL;
  int count = 0;
  for (int i = 0; i < size; i++)
  {
    int id = getVertexId(H.graph, buffer[i]);
    if (id == 0)
    {
      count++;
      id = count;
      addVertex(H.graph, id, buffer[i]);
      insertQueue(queue, id, 1);
    }
    else
      insertQueue(queue, id, 1);
  }
  while (1)
  {
    int node_1;
    int node_2;
    int id_node_1 = findMinInQueue(queue, &node_1);
    if (id_node_1 == 0)
    {
      break;
    }
    delInQueue(queue, id_node_1);
    int id_node_2 = findMinInQueue(queue, &node_2);
    if (id_node_2 == 0)
    {
      break;
    }
    delInQueue(queue, id_node_2);
    count++;
    insertQueue(queue, count, node_1 + node_2);
    addVertex(H.graph, count, 'i');
    addEdgeSingle(H.graph, count, id_node_1, 0);
    addEdgeSingle(H.graph, count, id_node_2, 1);
  }
  H.root = jrb_find_int(H.graph.vertices, count);
  return H;
};

Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}
JRB addVertex(Graph graph, int id, char name)
{
  edge_number++;
  JRB node = jrb_find_int(graph.vertices, id);
  if (node == NULL)
  {
    return jrb_insert_int(graph.vertices, id, new_jval_c(name));
  }
  node->val = new_jval_c(name);
  return node;
}
char getVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);
  return (node == NULL) ? -1 : jval_c(node->val);
}
void addEdgeSingle(Graph graph, int v1, int v2, double weight)
{
  // Add to vertex 1
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
  {
    JRB tree = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
  else
  {
    JRB child;
    JRB tree = (JRB)jval_v(node->val);
    int check = 0;
    jrb_traverse(child, tree)
    {
      if (jval_i(child->key) == v2)
      {
        check = 1;
      }
    }
    if (check == 0)
    {
      jrb_insert_int(tree, v2, new_jval_d(weight));
    }
  }
}
void addEdge(Graph graph, int v1, int v2, double weight)
{
  addEdgeSingle(graph, v1, v2, weight);
  addEdgeSingle(graph, v2, v1, weight);
}
void printVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);
  if (node == NULL)
  {
    printf("There are no id like this\n");
    return;
  }
  printf("%c\n", jval_c(node->val));
  return;
}
int outdegree(Graph graph, int v, int *output)
{
  int total = 0;
  JRB node = jrb_find_int(graph.edges, v);
  if (node == NULL)
  {
    return -1;
  }
  JRB tree = (JRB)jval_v(node->val);
  jrb_traverse(node, tree)
  {
    output[total++] = jval_i(node->key);
  }
  // for (int i = 0; i < total; i++)
  // {
  //   printf("%d ", output[i]);
  // }
  // printf("\n");
  return total;
}
void greedyColoring(Graph graph, int size)
{
  JRB node;
  int output[100];
  int length;
  bool available[100];
  int result[100]; // Gia tri cua result[i] chinh la color nth
  int first_id;
  // Get first vertices
  jrb_traverse(node, graph.vertices)
  {
    if (node != NULL)
    {
      break;
    }
  }
  first_id = jval_i(node->key);
  printf("First: %d\n", first_id);
  result[first_id] = 0;
  jrb_traverse(node, graph.vertices)
  {
    if (jval_i(node->key) == first_id)
    {
      continue;
    }
    else
    {
      result[jval_i(node->key)] = -1;
    }
  }
  for (int i = 0; i < size; i++)
  {
    available[i] = false;
  }
  jrb_traverse(node, graph.vertices)
  {
    if (jval_i(node->key) == first_id)
    {
      continue;
    }
    length = outdegree(graph, jval_i(node->key), output);
    for (int i = 0; i < length; i++)
    {
      if (result[output[i]] != -1)
      {
        available[result[output[i]]] = true;
      }
    }
    int cr = 0;
    while (cr < size)
    {
      if (available[cr] == false)
      {
        break;
      }
      cr++;
    }
    printf("Cr: %d\n", cr);
    result[jval_i(node->key)] = cr;
    for (int i = 0; i < length; i++)
    {
      if (result[output[i]] != -1)
      {
        available[result[output[i]]] = false;
      }
    }
  }
  jrb_traverse(node, graph.vertices)
  {
    printf("Vertex %d --> Color %d\n", jval_i(node->key), result[jval_i(node->key)]);
  }
}
bool hasCycle_rec(Graph graph, int u, bool visited[], int parent)
{
  visited[u] = true;
  JRB vertex = jrb_find_int(graph.edges, u);
  JRB tree = jval_v(vertex->val);
  JRB node = NULL;
  jrb_traverse(node, tree)
  {
    int v = jval_i(node->key);
    if (visited[v] && v != parent)
      return true;
    if (!visited[v] && hasCycle_rec(graph, v, visited, u))
      return true;
  }
  return false;
}
bool hasCycle(Graph graph)
{
  bool visited[256];
  for (int i = 0; i < 256; i++)
  {
    visited[i] = false;
  }
  JRB node;
  jrb_traverse(node, graph.edges)
  {
    int i = jval_i(node->key);
    if (!visited[i] && hasCycle_rec(graph, i, visited, -1))
      return true;
  }
  return false;
}
void find_code(HuffmanTree H, int root_key, Coding *htable, char code[])
{
  JRB node, tree, buf;
  char c;
  node = jrb_find_int(H.graph.edges, root_key);
  if (node == NULL)
  {
    c = getVertex(H.graph, root_key);
    strcpy(htable[c].bits, code);
    htable[c].size = strlen(code);
    printf("%c %s \n", c, htable[c].bits);
  }
  else
  {
    JRB child;
    tree = (JRB)jval_v(node->val);
    char backup[256] = "";
    jrb_traverse(child, tree)
    {
      if (jval_d(child->val) == 0)
      {
        strcpy(backup, code);
        strcat(code, "0");
        find_code(H, jval_i(child->key), htable, code);
        strcpy(code, backup);
      }
      if (jval_d(child->val) == 1)
      {
        strcpy(backup, code);
        strcat(code, "1");
        find_code(H, jval_i(child->key), htable, code);
        strcpy(code, backup);
      }
    }
  }
}
void createHuffmanTable(HuffmanTree H, Coding htable[])
{
  char code[256] = "";
  for (int i = 0; i < 256; i++)
  {
    htable[i].size = 0;
  }
  find_code(H, jval_i(H.root->key), htable, code);
}

void compress(char buffer[], char *output, Coding huffmanTable[])
{
  for (int i = 0; i < strlen(buffer); i++)
  {
    strcat(output, huffmanTable[buffer[i]].bits);
  }
}