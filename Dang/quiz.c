#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "stack.h"
#include "dllist.h"
#include <math.h>
#include <stdbool.h>
struct item
{
  double val;
  int parent;
};
int edge_number = 0;

typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;
void swap(int a, int b)
{
  int temp = a;
  a = b;
  b = temp;
}
Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
int DAG(Graph graph);
void printVertex(Graph graph, int id);
int DFS(Graph graph, int start);
void topologicalSort(Graph g, int *output, int *n);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
int minDistance(Graph graph, double dist[], bool sptSet[]);
void printSolution(Graph graph, double dist[]);
void dijkstra(Graph graph, int src);

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
  double weight;
  int path[256];
  int length;
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
  for (int i = length; i >= 0; i--)
  {
    printf("%s->", getVertex(graph, path[i]));
  }
  printf("t\n");
  // do
  // {
  //   printf("1. Add Vertex\n");
  //   printf("2. Add Edges\n");
  //   printf("3. Has Edge\n");
  //   printf("4. Get Vertex\n");
  //   printf("5. Indegree\n");
  //   printf("6. Outdegree\n");
  //   printf("7. Topological sort\n");
  //   printf("8. Out\n");
  //   printf("Input choice: \n");
  //   scanf("%d", &choice);
  //   switch (choice)
  //   {
  //   case 1:
  //     printf("Input id and name: \n");
  //     scanf("%d %s", &id, name);
  //     addVertex(graph, id, name);
  //     break;
  //   case 2:
  //     printf("Input 2 vertices v1 and v2: \n");
  //     scanf("%d %d %g", &v1, &v2, &weight);
  //     addEdge(graph, v1, v2, weight);
  //     break;
  //   case 3:
  //     printf("Input 2 vertices v1 and v2: \n");
  //     scanf("%d %d", &v1, &v2);
  //     total = getEdgeValue(graph, v1, v2);
  //     if (total == 1)
  //     {
  //       printf("There exists edge between %d and %d\n", v1, v2);
  //     }
  //     else
  //     {
  //       printf("There doesn't exist edge between %d and %d\n", v1, v2);
  //     }
  //     break;
  //   case 4:
  //     printf("Input id: \n");
  //     scanf("%d", &id);
  //     if (getVertex(graph, id) == NULL)
  //     {
  //       printf("There doesn't exist vertex %d", id);
  //     }
  //     else
  //     {
  //       printf("Name of vertex is %s\n", getVertex(graph, id));
  //     }
  //     break;
  //   case 5:
  //     printf("Input id: \n");
  //     scanf("%d", &id);
  //     printf("There are %d vertices to %d\n", indegree(graph, id, output), id);
  //     break;
  //   case 6:
  //     printf("Input id: \n");
  //     scanf("%d", &id);
  //     if (outdegree(graph, id, output) != -1)
  //     {
  //       printf("There are %d vertices from %d\n", outdegree(graph, id, output), id);
  //     }
  //     else
  //     {
  //       printf("No vertices go from %d vertex \n", id);
  //     }
  //     break;
  //   case 8:
  //     return 0;
  //   case 7:
  //     if (DAG(graph) == 1)
  //     {
  //       printf("Can't make topological sort because of circle graph\n");
  //       break;
  //     }
  //     topologicalSort(graph, output, &n);
  //     for (int i = 0; i < n; i++)
  //     {
  //       printf("%s\n", getVertex(graph, output[i]));
  //     }
  //     break;
  //   }
  // } while (1);
}
int minDistance(Graph graph, double dist[], bool sptSet[])
{
  JRB node = NULL;
  double min = INFINITY, min_index;
  jrb_traverse(node, graph.vertices)
  {
    int v = jval_i(node->key);
    if (sptSet[v] == false && dist[v] <= min)
      min = dist[v],
      min_index = v;
  }
  return min_index;
}
void printSolution(Graph graph, double dist[])
{
  JRB node = NULL;
  printf("Vertex \t\t Distance from Source\n");
  jrb_traverse(node, graph.vertices)
  {
    int i = jval_i(node->key);
    printf("%s \t\t %g\n", getVertex(graph, i), dist[i]);
  }
}

void dijkstra(Graph graph, int src)
{
  JRB node = NULL;
  double dist[100];
  bool sptSet[100];
  for (int i = 0; i < 100; i++)
    dist[i] = INFINITY, sptSet[i] = false;
  dist[src] = 0;
  jrb_traverse(node, graph.vertices)
  {
    JRB tree = NULL;
    int u = minDistance(graph, dist, sptSet);
    sptSet[u] = true;
    jrb_traverse(tree, graph.vertices)
    {
      int v = jval_i(tree->key);
      if (!sptSet[v] && getEdgeValue(graph, u, v) && dist[u] != INFINITY && dist[u] + getEdgeValue(graph, u, v) < dist[v])
        dist[v] = dist[u] + getEdgeValue(graph, u, v);
    }
  }
  printSolution(graph, dist);
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
  edge_number++;
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
void addEdge(Graph graph, int v1, int v2, double weight)
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
    JRB tree = (JRB)jval_v(node->val);
    jrb_insert_int(tree, v2, new_jval_d(weight));
  }
}
double getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
    return 0;
  JRB endPoint = jrb_find_int(jval_v(node->val), v2);
  return (endPoint == NULL) ? 0 : jval_d(endPoint->val);
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
void heapify(int arr[], int n, int i, struct item node[])
{
  int smallest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;
  if (l < n && arr[l] < arr[smallest])
    smallest = l;
  if (r < n && arr[r] < arr[smallest])
    smallest = r;
  if (smallest != i)
  {
    swap(arr[i], arr[smallest]);
    heapify(arr, n, smallest, node);
  }
}

void heapSort(int arr[], int n, struct item node[])
{
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i, node);

  for (int i = n - 1; i >= 0; i--)
  {
    swap(arr[0], arr[i]);

    heapify(arr, i, 0, node);
  }
}
void insertionSort(int a[], int n, struct item node[])
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
  struct item node_arr[100];
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
    node_arr[jval_i(node->key)].val = INFINITY;
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
  struct item destination = node_arr[t];
  int total_weight = destination.val;
  while (destination.parent != -1)
  {
    int a = destination.parent;
    path[(*length)++] = a;
    destination = node_arr[a];
  }
  return total_weight;
};
