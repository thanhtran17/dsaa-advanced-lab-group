#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "stack.h"
#include "dllist.h"
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
int DAG(Graph graph);
void printVertex(Graph graph, int id);
int DFS(Graph graph, int start);
void topologicalSort(Graph g, int *output, int *n);
int main()
{
  Graph graph = createGraph();
  int output[256];
  int total;
  int choice;
  int n = 0;
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
    printf("7. Topological sort\n");
    printf("8. Out\n");
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
      if (outdegree(graph, id, output) != -1)
      {
        printf("There are %d vertices from %d\n", outdegree(graph, id, output), id);
      }
      else
      {
        printf("No vertices go from %d vertex \n", id);
      }
      break;
    case 8:
      return 0;
    case 7:
      if (DAG(graph) == 1)
      {
        printf("Can't make topological sort because of circle graph\n");
        break;
      }
      topologicalSort(graph, output, &n);
      for (int i = 0; i < n; i++)
      {
        printf("%s\n", getVertex(graph, output[i]));
      }
      break;
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
  JRB node = NULL;
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
  if (node == NULL)
  {
    return -1;
  }
  JRB tree = (JRB)jval_v(node->val);
  jrb_traverse(node, tree)
  {
    output[total++] = jval_i(node->key);
  }
  return total;
}
int DAG(Graph graph)
{
  JRB node;
  int a = 0;
  jrb_traverse(node, graph.vertices)
  {
    a = DFS(graph, jval_i(node->key));
  }
  return a;
}
void printVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);
  if (node == NULL)
  {
    printf("There are no id like this\n");
    return;
  }
  printf("%s\n", jval_s(node->val));
  return;
}
int DFS(Graph graph, int start)
{
  int a[100]; // ham luu cac dinh da xet
  memset(a, 0, sizeof(int) * 100);
  Dllist item;
  Dllist stack = new_dllist();
  int output[100];
  int vertex;
  int n;
  int re = 0;
  dll_prepend(stack, new_jval_i(start));
  while (!dll_empty(stack))
  {
    item = dll_first(stack);
    vertex = jval_i(item->val);
    dll_delete_node(item);
    if (a[vertex] == 0)
    {
      // printVertex(graph, vertex);
      a[vertex] = 1;
      n = outdegree(graph, vertex, output);
      for (int i = 0; i <= n; i++)
      {
        if (output[i] == start)
        {
          re = 1;
        }
        if (a[output[i]] == 0)
        {
          dll_prepend(stack, new_jval_i(output[i]));
        }
      }
    }
  }
  return re;
}
void topologicalSort(Graph graph, int *output, int *n)
{
  Dllist head;
  JRB node;
  int t[30];
  int count;
  int number[100];
  int temp;
  Dllist queue = new_dllist();
  jrb_traverse(node, graph.vertices)
  {
    temp = indegree(graph, jval_i(node->key), t);
    if (temp == 0)
    {
      dll_append(queue, node->key);
    }
    number[jval_i(node->key)] = temp;
  }
  while (!dll_empty(queue))
  {
    head = dll_first(queue);
    dll_delete_node(head);
    output[(*n)++] = jval_i(head->val); // node->key (Jval)
    count = outdegree(graph, jval_i(head->val), t);
    for (int i = 0; i < count; i++)
    {
      number[t[i]]--;
      if (number[t[i]] == 0)
      {
        dll_append(queue, new_jval_i(t[i]));
      }
    }
  }
};