#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "jrb.h"
#include "jval.h"
#include "stack.h"
#include <stdbool.h>
#include "queue.h"
//------------------GLOBAL VARIABLES-------------------------//
int *queue;
int *stack;
bool *visited;
bool *visited_st;
int output[256];

//------------------FUNCTION HANDLER-------------------------//

JRB createGraph();
void printVertex(int a);
void addEdges(JRB graph, int v1, int v2);
int getAdjacent(JRB graph, int vertex);
void freeGraph(JRB graph);
void printGraphs(JRB graph);
void BFS(JRB graph, int start, int stop, void (*printVertex)(int));
void DFS(JRB graph, int start, int stop, void (*func)(int));
//------------------------------------------------------------//

int main()
{
  JRB graph = createGraph();
  int total;
  int choice;
  int start;
  int stop;
  int v1, v2;

  queue = (int *)malloc(100000 * sizeof(int));
  stack = (int *)malloc(100000 * sizeof(int));

  visited = (bool *)malloc(100000 * sizeof(bool));
  visited_st = (bool *)malloc(100000 * sizeof(bool));
  memset(queue, -1, 100000 * sizeof(int));
  memset(visited, false, 100000 * sizeof(bool));
  memset(visited_st, false, 100000 * sizeof(bool));

  do
  {
    printf("1. Add Edges\n");
    printf("2. Get adjacents\n");
    printf("3. Print Graph\n");
    printf("4. BFS Print Graph and Out\n");
    printf("5. DFS Print Graph and Out\n");
    printf("6. Out\n");
    printf("Input choice: \n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
      printf("Input edges v1 and v2: \n");
      scanf("%d %d", &v1, &v2);
      addEdges(graph, v1, v2);
      break;
    case 2:
      printf("Input vertex: \n");
      scanf("%d", &v1);
      total = getAdjacent(graph, v1);
      if (total == 0)
      {
        printf("There is no adjacent at this point. \n");
      }
      else
      {
        printf("Adjacent vertices of node %d: ", v1);
        for (int i = 0; i < total; i++)
        {
          printf("%d ", output[i]);
        }
        printf("\n");
      }
      break;
    case 3:
      printGraphs(graph);
      break;
    case 4:
      printf("Input starter point and stop point: \n");
      scanf("%d %d", &start, &stop);
      if (stop == -1)
      {
        printf("Print from all vertices from vertex till end %d\n", start);
      }
      else
      {
        printf("Print from all vertices from vertex %d to vertex %d\n", start, stop);
      }
      BFS(graph, start, stop, printVertex);
      printf("\n");
      printf("Goodbye!\n");
      return 0;
    case 5:
      printf("Input starter point and stop point: \n");
      scanf("%d %d", &start, &stop);
      if (stop == -1)
      {
        printf("Print from all vertices from vertex till end %d\n", start);
      }
      else
      {
        printf("Print from all vertices from vertex %d to vertex %d\n", start, stop);
      }
      DFS(graph, start, -1, printVertex);
      printf("\n");
      printf("Goodbye!\n");
      return 0;
      ;
    case 6:
      freeGraph(graph);
      return 0;
    }
  } while (1);
}

//----------FUNCTIONS-------------//
void DFS(JRB graph, int start, int stop, void (*printVertex)(int))
{
  memset(visited_st, false, 100000 * sizeof(bool));
  visited_st[start] = true;
  push(stack, start);

  while (!isEmptySt(stack))
  {
    start = pop(stack);
    if (start == -1)
    {
      return;
    }
    else if (start == stop)
    {
      printVertex(start);
      return;
    }
    printVertex(start);

    int total = getAdjacent(graph, start);
    for (int i = 0; i < total; i++)
    {
      if (!visited_st[output[i]])
      {
        visited_st[output[i]] = true;
        push(stack, output[i]);
      }
    }
  }
  return;
}

void BFS(JRB graph, int start, int stop, void (*printVertex)(int))
{
  memset(visited, false, 100000 * sizeof(bool));
  visited[start] = true;
  enqueue(queue, start);
  while (!isEmpty(queue))
  {
    start = dequeue(queue);
    if (start == -1)
    {
      return;
    }
    else if (start == stop)
    {
      printVertex(start);
      return;
    }
    printVertex(start);

    int total = getAdjacent(graph, start);
    for (int i = 0; i < total; i++)
    {
      if (!visited[output[i]])
      {
        visited[output[i]] = true;
        enqueue(queue, output[i]);
      }
    }
  }
  return;
}

JRB createGraph()
{
  return make_jrb();
}
void printVertex(int a)
{
  printf("%d ", a);
}
void addOneEdge(JRB graph, int v1, int v2)
{
  // Add to vertex 1
  JRB node = jrb_find_int(graph, v1);
  if (node == NULL)
  {
    JRB tree = make_jrb();
    jrb_insert_int(graph, v1, new_jval_v(tree));
    jrb_insert_int(tree, v2, new_jval_i(1));
  }
  else
  {
    JRB tree = (JRB)jval_v(node->val);
    jrb_insert_int(tree, v2, new_jval_i(1));
  }
}
void addEdges(JRB graph, int v1, int v2)
{
  addOneEdge(graph, v1, v2);
  addOneEdge(graph, v2, v1);
}
int getAdjacent(JRB graph, int vertex)
{
  JRB node = jrb_find_int(graph, vertex);
  JRB tree = (JRB)jval_v(node->val);
  int total = 0;
  jrb_traverse(node, tree)
  {
    output[total++] = jval_i(node->key);
  }
  return total;
}
void freeGraph(JRB graph)
{
  JRB node;
  jrb_traverse(node, graph)
  {
    jrb_free_tree(jval_v(node->val));
  }
}
void printGraphs(JRB graph)
{
  JRB node;
  JRB tree;
  jrb_traverse(node, graph)
  {
    printf("Node: %d : {", node->key);
    tree = (JRB)jval_v(node->val);
    if (tree)
    {
      JRB node_new;

      jrb_traverse(node_new, tree)
      {
        printf("%d, ", node_new->key);
      }
    }
    printf("}\n");
  }
}