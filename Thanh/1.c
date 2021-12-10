#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"
#include <math.h>
#include <stdbool.h>
//------------------------------------------------------
void clear(){
   while (getchar() != '\n');
}
//------------------------------------------------------
typedef struct {
  JRB edges;
  JRB vertices;
} Graph;
//------------------------------------------------------
Graph createGraph();
void dropGraph(Graph graph);
void topologicalSort(Graph g, int output[], int* n);

void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int indegree(Graph graph, int v, int output[]);
int outdegree(Graph graph, int v, int output[]);

int DFS(Graph g, int start);
int DAG(Graph graph);
//------------------------------------------------------
int main()
{
  int output[10]; 
  Graph g = createGraph();
  int id;
  char name[10];
  int choice;
  int v1, v2;
  int count = 0, topo[10];
  JRB node;

  printf("---Menu---\n");
  printf("1. Add Vertex\n");
  printf("2. Add Edge\n");
  printf("3. Check edge\n");
  printf("4. Indgree and Outdegree of Vertex\n");
  printf("5. Topological Sort\n");
  printf("6. Delete graph and exit\n");
  do {
    printf("--> Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      clear();
      printf("Enter name of the vertex: ");
      scanf("%s", name);
      printf("Enter id of the vertex: ");
      scanf("%d", &id);
      addVertex(g, id, strdup(name));
      break;
    case 2:
      printf("Enter id of 2 vertices: ");
      scanf("%d %d", &v1, &v2);
      addEdge(g, v1, v2);
      break;
    case 3: 
      printf("Enter id of 2 vertices: ");
      scanf("%d %d", &v1, &v2);
      int check = hasEdge(g, v1, v2);
      if (check == 1) printf("Edge existed.\n");
      else printf("No edge between 2 vertices.\n");
      break;
    case 4:
      printf("Enter the vertex's id: ");
      scanf("%d", &v1);
      printf("In: %d\t Out: %d\n", indegree(g, v1, output), outdegree(g, v1, output));
      break;
    case 5:
      if (DAG(g) == 1) {
        printf("ERROR! Circle graph!\n");
        break;
      }
      topologicalSort(g, topo, &count);
      printf("The topological order:\n");
      for (int i = 0; i < count; i++){
        printf("%s ", getVertex(g, topo[i]));
      }
      printf("\n");
      break;
    default:
      dropGraph(g);
      printf("\n");
      printf("Delete and Exit\n");
      break;
    }
  } while (choice != 6);
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
  JRB tree = (JRB)jval_v(node->val); // tạo 1 tree bằng đầu với node v1
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
