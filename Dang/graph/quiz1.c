#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
  int *matrix;
  int sizemax;
} Graph;

int menu();
Graph createGraph(int maxsize);
void addEdge(Graph a, int v1, int v2);
void print(Graph a);
int adjacent(Graph a, int v1, int v2);
int getAdjacent(Graph a, int vertex, int *output);
void dropGraph(Graph a);

int main()
{
  Graph graph;
  int *output, choice, maxsize;
  int edge1, edge2;
  do
  {
    choice = menu();
    switch (choice)
    {
    case 1:
      printf("Enter max size of Graph: ");
      scanf("%d", &maxsize);
      graph = createGraph(maxsize);
      output = (int *)malloc(sizeof(int) * maxsize * maxsize);
      memset(output, 0, sizeof(output) * maxsize * maxsize);
      printf("Created successfully\n\n");
      break;
    case 2:
      printf("Enter edge-edge: ");
      scanf("%d-%d", &edge1, &edge2);
      if (edge1 >= maxsize || edge2 >= maxsize)
        printf("Edge must be less than maxsize\n\n");
      else
      {
        addEdge(graph, edge1, edge2);
        printf("Add edges successfully\n\n");
      }
      break;
    case 3:
      printf("Print the graph like matrix:\n");
      print(graph);
      printf("\n");
      break;
    case 4:
      printf("Enter edge-edge: ");
      scanf("%d-%d", &edge1, &edge2);
      if (adjacent(graph, edge1, edge2))
        printf("%d and %d are adjacent\n\n", edge1, edge2);
      else
        printf("%d and %d are not adjacent\n\n", edge1, edge2);
      break;
    case 5:
      printf("Enter vertex: ");
      scanf("%d", &edge1);
      int total = getAdjacent(graph, edge1, output);
      if (total == 0)
        printf("%d has no adjacent\n\n", edge1);
      else
      {
        printf("Adjacent of %d are: ", edge1);
        int i;
        for (i = 0; i < total; i++)
          printf("%d\t", output[i]);
        printf("\n");
      }
      break;
    case 6:
      printf("Thanks for using\n\n");
      dropGraph(graph);
      break;
    default:
      printf("Wrong option, try again\n\n");
      break;
    }
  } while (choice != 6);
  return 0;
}

int menu()
{
  int chocie;
  printf("--------------MENU------------\n");
  printf("1. Create graph\n");
  printf("2. Add edges\n");
  printf("3. Print the graph by matrix\n");
  printf("4. Check adjacent\n");
  printf("5. Find adjacent\n");
  printf("6. Exit and drop graph\n");
  printf("-------------------------------\n\n");
  printf("Your choice: ");
  scanf("%d", &chocie);
  return chocie;
}

Graph createGraph(int maxsize)
{
  Graph a;
  int size = maxsize * maxsize;
  a.sizemax = size;
  a.matrix = (int *)malloc(sizeof(int) * size);
  memset(a.matrix, 0, sizeof(a.matrix) * size);
  return a;
}

void addEdge(Graph a, int v1, int v2)
{
  int size = sqrt(a.sizemax);
  a.matrix[v1 * size + v2] = 1;
  a.matrix[v2 * size + v1] = 1;
}

void print(Graph a)
{
  int i, j;
  int size = sqrt(a.sizemax);
  for (i = 0; i < size; i++)
    printf("\t%d", i);
  printf("\n");
  for (i = 0; i < size; i++)
  {
    printf("%d:\t", i);
    for (j = size * i; j < size * (i + 1); j++)
      printf("%d\t", a.matrix[j]);
    printf("\n");
  }
}

int adjacent(Graph a, int v1, int v2)
{
  int size = sqrt(a.sizemax);
  if (a.matrix[v1 * size + v2] == 0)
    return 0;
  return 1;
}

int getAdjacent(Graph a, int vertex, int *output)
{
  int size = sqrt(a.sizemax);
  int total = 0, i;
  for (i = vertex * size; i < (vertex + 1) * size; i++)
    if (a.matrix[i] == 1)
    {
      output[total] = i - vertex * size;
      total++;
    }
  return total;
}

void dropGraph(Graph a)
{
  a.sizemax = 0;
  free(a.matrix);
}