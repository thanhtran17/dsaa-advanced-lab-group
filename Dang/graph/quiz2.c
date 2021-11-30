#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"

JRB createGraph()
{
  return make_jrb();
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
int getAdjacent(JRB graph, int vertex, int *output)
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
int main()
{
  JRB graph = createGraph();
  int output[256];
  int total;
  int choice;
  int v1, v2;
  do
  {
    printf("1. Add Edges\n");
    printf("2. Get adjacents\n");
    printf("3. Print Graph\n");
    printf("4. Out\n");
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
      total = getAdjacent(graph, v1, output);
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
      freeGraph(graph);
      return 0;
    }
  } while (1);
}