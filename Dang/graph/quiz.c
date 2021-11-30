#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"

typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;
Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
int main()
{
  Graph graph = createGraph();
  int output[256];
  int total;
  int choice;
  int v1, v2;
  int id;
  char name[256];
  do
  {
    printf("1. Add Vertex\n");
    printf("2. Add Edges\n");
    printf("3. Has Edge\n");
    printf("4. Get Vertex\n");
    printf("5. Indegree\n");
    printf("6. Outdegree\n");
    printf("7. Out\n");
    printf("Input choice: \n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      printf("Input id and name: \n");
      scanf("%d %s", &id, name);
      addVertex(graph, id, name);
      break;
    case 2:
      printf("Input 2 vertices v1 and v2: \n");
      scanf("%d %d", &v1, &v2);
      addEdge(graph, v1, v2);
      break;
    case 3:
      printf("Input 2 vertices v1 and v2: \n");
      scanf("%d %d", &v1, &v2);
      total = hasEdge(graph, v1, v2);
      if (total == 1)
      {
        printf("There exists edge between %d and %d\n", v1, v2);
      }
      else
      {
        printf("There doesn't exist edge between %d and %d\n", v1, v2);
      }
      break;
    case 4:
      printf("Input id: \n");
      scanf("%d", &id);
      if (getVertex(graph, id) == NULL)
      {
        printf("There doesn't exist vertex %d", id);
      }
      else
      {
        printf("Name of vertex is %s\n", getVertex(graph, id));
      }
      break;
    case 5:
      printf("Input id: \n");
      scanf("%d", &id);
      printf("There are %d vertices to %d\n", indegree(graph, id, output), id);
      break;
    case 6:
      printf("Input id: \n");
      scanf("%d", &id);
      printf("There are %d vertices from %d\n", outdegree(graph, id, output), id);
      break;
    case 7:
      return 0;
    }
  } while (1);
}
Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}
void addVertex(Graph graph, int id, char *name)
{
  JRB node = jrb_find_int(graph.vertices, id);
  if (node == NULL)
  {
    jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
    return;
  }
  strcpy(jval_s(node->val), name);
}
char *getVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);
  return (node == NULL) ? NULL : jval_s(node->val);
}
void addEdge(Graph graph, int v1, int v2)
{
  // Add to vertex 1
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
  {
    JRB tree = make_jrb();
    jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    jrb_insert_int(tree, v2, new_jval_i(1));
  }
  else
  {
    JRB tree = (JRB)jval_v(node->val);
    jrb_insert_int(tree, v2, new_jval_i(1));
  }
}
int hasEdge(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
    return 0;
  JRB endPoint = jrb_find_int(jval_v(node->val), v2);
  return (endPoint == NULL) ? 0 : 1;
}
int indegree(Graph graph, int v, int *output)
{
  int total = 0;
  JRB node;
  jrb_traverse(node, graph.edges)
  {
    if (node->key.i == v)
    {
      continue;
    }
    if (jrb_find_int(jval_v(node->val), v) != NULL)
    {
      output[total++] = jval_i(node->key);
    }
  }
  return total;
}
int outdegree(Graph graph, int v, int *output)
{
  int total = 0;
  JRB node = jrb_find_int(graph.edges, v);
  JRB tree = jval_v(node->val);
  jrb_traverse(node, tree)
  {
    output[total++] = jval_i(node->key);
  }
  return total;
}