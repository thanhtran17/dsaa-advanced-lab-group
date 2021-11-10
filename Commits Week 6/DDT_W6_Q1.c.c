#include<stdio.h>
#include<stdlib.h>
//---------------------------------------------------
typedef struct {
  int* matrix;
  int size;
} Graph;
//---------------------------------------------------
Graph createGraph(int size)  
{
  Graph root;
  root.matrix = (int*) calloc (size * size, sizeof(int));
  root.size = size;
  
  return root;
}
//---------------------------------------------------
void addEdge (Graph graph, int v1, int v2)
{
  int n = graph.size;
  graph.matrix[v1 * n + v2] = 1;
  graph.matrix[v2 * n + v1] = 1;
}
//---------------------------------------------------
int adjacent(Graph graph, int v1, int v2)
{
  int n = graph.size;
  if (graph.matrix[n * v1 + v2] == 1)
    return 1;
  else 
    return 0;
}
//---------------------------------------------------
int getAdjacentVertices(Graph graph, int vertex, int *output)
{
  int j = 0;
  for (int i = 0; i < graph.size; i++) {
    if (adjacent(graph, vertex, i)) {
      output[j] = i;
      j++;
    }
  }

  return j;
}
//---------------------------------------------------
void deleteGraph(Graph graph)
{
  free(graph.matrix);
}
//---------------------------------------------------
int main()
{
  int i, n, output[10], choice;
  int xi, yi, enteredAdjacent; 

  Graph g = createGraph(10);

  printf("---Menu---\n");
  printf("1. Add edge\n");
  printf("2. Print graph\n");
  printf("3. Find adjacent vertices\n");
  printf("4. Delete graph and exit\n");

  while (1) {
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice){  
      case 1: 
        printf("--> Enter edge (x, y): ");
        scanf("%d%d", &xi, &yi);
        addEdge(g, xi, yi);
        printf("\n");
        break;
      case 2: 
        printf("--> Graph:\n\n");
        for (int i = 0; i < 10; i++){
          for (int j = 0; j < 10; j++)  
            printf("%5d", g.matrix[i * 10 + j]);
          printf("\n");
        }
        printf("\n");
        break;
      case 3:
        printf("--> Enter vertice to find adjacent vertices: ");
        scanf("%d", &enteredAdjacent);
        n = getAdjacentVertices (g, enteredAdjacent, output);

        if (n == 0) {
          printf("No adjacent vertices of the selected node\n");
        } 
        else {
          printf("Adjacent vertices of the selected node:\n");
          for (i=0; i<n; i++)   
            printf("%2d", output[i]);
          printf("\n");
        }
        break;
      case 4: 
        deleteGraph(g);
        printf("--> Graph deleted");
        return 0;
    }
  }
}
  // addEdge(g, 0, 1);
  // addEdge(g, 0, 2);
  // addEdge(g, 1, 2);
  // addEdge(g, 1, 3);

  // //Print graph  
  // printf("Graph:\n\n");
  // for (int i = 0; i < 10; i++){
  //   for (int j = 0; j < 10; j++)  
  //     printf("%5d", g.matrix[i * 10 + j]);
  //   printf("\n");
  // }

  // // Adjacent vertices  
  // n = getAdjacentVertices (g, 3, output);

  // if (n == 0) {
  //   printf("No adjacent vertices of the selected node\n");
  // } 
  // else {
  //   printf("\nAdjacent vertices of the selected node:");
  //   for (i=0; i<n; i++)   
  //     printf("%2d", output[i]);
  // }

  // printf("\n\n");
