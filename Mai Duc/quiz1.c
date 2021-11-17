#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "jval.h"
#include "jrb.h"
#include "dllist.h"
//------------------------------------------------------
typedef JRB Graph;
//------------------------------------------------------
Graph createGraph()
{
  return make_jrb();
}
//------------------------------------------------------
void addEdge(Graph g, int v1, int v2)
{
  JRB node1 = jrb_find_int(g, v1);
  JRB node2 = jrb_find_int(g, v2);

  if (node1 == NULL){
    JRB tree1 = make_jrb();
    jrb_insert_int(g, v1, new_jval_v(tree1));
    jrb_insert_int(tree1, v2, new_jval_i(1));
  }
  else {
    JRB tree1 = (JRB)jval_v(node1->val);
    jrb_insert_int(tree1, v2, new_jval_i(1));
  }

  if (node2 == NULL){
    JRB tree2 = make_jrb();
    jrb_insert_int(g, v2, new_jval_v(tree2));
    jrb_insert_int(tree2, v1, new_jval_i(1));
  }
  else {
    JRB tree2 = (JRB)jval_v(node2->val);
    jrb_insert_int(tree2, v1, new_jval_i(1));
  }
}
//------------------------------------------------------
void printGraph(Graph g)
{
  JRB col, row;
  jrb_traverse(col, g){
    printf("%d:   ", col->key);
    row = (JRB)jval_v(col->val);
    if (row){
      JRB rowHead;
      jrb_traverse(rowHead, row){
        printf("%d   ", rowHead->key);
      }
    }
    printf("\n\n");
  }
}
//------------------------------------------------------
int getAdjacentVertices(Graph g, int vertex, int *output)
{
  JRB node = jrb_find_int(g, vertex);
  JRB tree = (JRB)jval_v(node->val);
  
  int count = 0;
  jrb_traverse(node, tree){
    output[count++] = jval_i(node->key);
  }

  return count;
}
//------------------------------------------------------
void deleteGraph(Graph g)
{
  JRB node;
  jrb_traverse(node, g){
    jrb_free_tree(jval_v(node->val));
  }
}
//------------------------------------------------------
void printVertex(int v) 
{
  printf("%4d", v); 
}
//------------------------------------------------------
void BFS(Graph graph, int start, int stop, void (*func)(int))
{
    int a[100], vertex, output[100], n;
    memset(a, 0, sizeof(int) * 100);

    Dllist item;
    Dllist queue = new_dllist();

    dll_append(queue, new_jval_i(start));

    while (!dll_empty(queue)){
        item = dll_first(queue);
        vertex = jval_i(item->val);
        dll_delete_node(item); 
        if (a[vertex] == 0){
            func(vertex);
            a[vertex] = 1;
            if (vertex == stop) 
              return;
            n = getAdjacentVertices(graph, vertex, output);
            for (int i = 0; i < n; i++){
                if (a[output[i]] != 1) 
                    dll_append(queue, new_jval_i(output[i]));
            }
        }
    } 
}
//----------------------------------------------------------
void DFS(Graph graph, int start, int stop, void (*func)(int)){
  int a[100]; //ham luu cac dinh da xet
  memset(a, 0, sizeof(int)*100);
  Dllist item;
  Dllist stack = new_dllist();
  int output[100];
  int vertex;
  int n;

  dll_prepend(stack, new_jval_i(start));
  while (!dll_empty(stack)){
    item = dll_first(stack);
    vertex = jval_i(item->val);
    dll_delete_node(item);
    if (a[vertex] == 0){
      func(vertex);
      a[vertex] = 1;
      if (vertex == stop) return;
      n = getAdjacentVertices(graph, vertex, output);
      for (int i = 0; i < n; i++){
        if (a[output[i]] == 0) dll_prepend(stack, new_jval_i(output[i])); 
      }
    }
  }
}
//----------------------------------------------------------
int main(){
  int i, n, output[10], choice;
  int xi, yi, enteredAdjacent; 
	int start, stop;
  Graph g = createGraph(10);

  printf("---Menu---\n");
  printf("1. Add edge\n");
  printf("2. Print BFS\n");
  printf("3. Print DFS\n");
  printf("4. Find adjacent vertices\n");
  printf("5. Delete graph and exit\n");

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
        printf("-->Input start vertex and stop vertex: \n");
				scanf("%d %d", &start, &stop);
				if (stop == -1)
					printf("Print from all vertices from vertex till end %d\n", start);
				else
					printf("Print from all vertices from vertex %d to vertex %d\n", start, stop);
				BFS(g, start, stop, printVertex);
				printf("\n");
        break;
      case 3: 
        printf("-->Input start vertex and stop vertex: \n");
				scanf("%d %d", &start, &stop);
				if (stop == -1)
					printf("Print from all vertices from vertex till end %d\n", start);
				else
					printf("Print from all vertices from vertex %d to vertex %d\n", start, stop);
				DFS(g, start, stop, printVertex);
				printf("\n");
        break;
      case 4:
        printf("--> Enter vertice to find adjacent vertices: ");
        scanf("%d", &enteredAdjacent);
        n = getAdjacentVertices(g, enteredAdjacent, output);

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
      case 5: 
        deleteGraph(g);
        printf("--> Graph deleted");
        return 0;
    }
  }
}