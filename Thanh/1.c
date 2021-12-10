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
//------------------------------------------------------
Graph createGraph();
void topologicalSort(Graph g);
void dropGraph(Graph graph);

void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int indegree(Graph graph, int v, int output[]);
int outdegree(Graph graph, int v, int output[]);

int DFS(Graph graph, int start);
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
  JRB node;

  printf("---Menu---\n");
  printf("1. Add Vertex\n");
  printf("2. Add Edge\n");
  printf("3. Check edge\n");
  printf("4. Indgree and Outdegree of Vertex\n");
  printf("5. Delete graph and exit\n");
  do {
    printf("Enter your choice: \n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      clear();
      printf("Enter name of the vertex: \n");
      scanf("%s", name);
      printf("Enter id of the vertex: \n");
      scanf("%d", &id);
      addVertex(g, id, strdup(name));
      break;
    case 2:
      printf("Enter id of 2 vertices: \n");
      scanf("%d %d", &v1, &v2);
      addEdge(g, v1, v2);
      break;
    case 3: 
      printf("Enter id of 2 vertices: \n");
      scanf("%d %d", &v1, &v2);
      int check = hasEdge(g, v1, v2);
      if (check == 1) printf("Edge existed.\n");
      else printf("No edge between 2 vertices.\n");
      break;
    case 4:
      printf("Enter the vertex's id: \n");
      scanf("%d", &v1);
      printf("In: %d\t Out: %d\n", indegree(g, v1, output), outdegree(g, v1, output));
      break;
    default:
      dropGraph(g);
      printf("\n");

      printf("Delete and Exit\n");
      break;
    }
  } while (choice != 5);
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
  JRB node;

  jrb_traverse(node, graph.edges)
  {
    if (node->key.i == v)
      continue;
    if (jrb_find_int((JRB) (jval_v(node->val)), v) != NULL) // tìm giá trị v trong cây node
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
