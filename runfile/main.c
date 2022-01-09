#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include "bus.h"

#define DATA 10000
//-------------------------------------------------------------------
char *buses[DATA]; 
char *readHolder1[DATA]; // go
char *readHolder2[DATA]; // back

int busLine[DATA][DATA];  // keep the number of lines that a bus takes
int sizeHolder = 0;     // size of above vars
int sizeBuses = 0;      // number of buses
int row = 0, col = 0;
//-------------------------------------------------------------------
void copyData(char *bus[], char* file1[], char* file2[]) 
{
	for (int i = 0; i < sizeHolder; i++) {
		file1[i] = strdup(readHolder1[i]);
		file2[i] = strdup(readHolder2[i]);
	}

	for (int i = 0; i < sizeBuses; i++) {
		bus[i] = strdup(buses[i]);
	}
}
//-------------------------------------------------------------------
void trim(char *line)
{
	int i = strlen(line);

	while (i > 0 && isspace(line[i - 1])) {
		--i;
	}

	line[i] = '\0';
	i = strspn ( line, " \t\n\v" );
	memmove ( line, line + i, strlen ( line + i ) + 1 );
}
//-------------------------------------------------------------------
void readFile(FILE *f1, FILE *f2) {
	int i = 0, k = 0, flag = 0;
	char *reader = (char*)malloc(sizeof(char));
	int fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
	buses[k] = strdup(reader);
	k++;
	busLine[row][col] = k;

	while (fscanfChecker == 1) {
		flag = 0;
		fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
		if (fscanfChecker != 1) break;
		if (strchr(strdup(reader), '-') != NULL) {
			readHolder1[i] = strdup(reader);
			flag = 1;
			i++;
			busLine[row][col + 1] = busLine[row][col + 1] + 1;
		}
		if (flag == 0) {
			buses[k] = strdup(reader);
			row++;
			k++;
			busLine[row][col] = k;
		}
	}
	sizeHolder = i; sizeBuses = k;
	i = 0;
	fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
	while (fscanfChecker == 1) {
		fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
		if (fscanfChecker != 1) break;
		if (strchr(strdup(reader), '-') != NULL) {
			readHolder2[i] = strdup(reader);
			i++;
		}
	}
}
//-------------------------------------------------------------------
void dataProcess(Graph g, JRB bus) {
	int i = 0, count = 0;
	int vertexID = 0;
	char *linesplitter[DATA];
	char *temp = "-";

	for (i = 0; i <= row; i++) {
		for (int m = 0; m < busLine[i][col + 1]; m++) {
			int j = 0, k = 0;
			linesplitter[j] = strtok(readHolder1[count], temp);
			while (linesplitter[j] != NULL) {
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}
			linesplitter[j] = strtok(readHolder2[count], temp);
			while (linesplitter[j] != NULL) {
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}
			
			char weight[10];
			weight[0] = (i + 1) + '0';
			weight[1] = '\0';
			for (int k = 0; k < j; k++) {
				trim(linesplitter[k]);
				addStations(g, linesplitter[k], &vertexID);
			}
			
			for (int k = 0; k < j - 1; k++) {
				addEdges(g, bus, linesplitter[k], linesplitter[k + 1], i);
			}
			count++;
		}
		
	}
}
//-------------------------------------------------------------------
void printFileData(char *bus[], char* file1[], char* file2[]) 
{
	int count = 0;
	for (int i = 0; i < sizeBuses; i++) {
		printf("BUS: %s\n", bus[i]);
		for (int j = 0; j < busLine[i][col + 1]; j++) {
			printf("CHIEU DI: %s\n\n", file1[count]);
			printf("CHIEU VE: %s\n\n", file2[count]);
			count++;
		}
	}
}
//-------------------------------------------------------------------
void printBuses(JRB bus) 
{
	JRB node;
	jrb_traverse(node, bus) {
		printf("%s - %s\n", jval_s(node->key), jval_s(node->val));
	}
}
//-------------------------------------------------------------------
void printPath(Graph g, JRB busTable, int *sizeOutput, int *output)
{
	if (*sizeOutput <= 0)
		return;

	int i = *sizeOutput - 1;;

	printf("\nPATH BY NAME: \n\n");

	for (i = *sizeOutput - 1; i > 0; i--) {
		char*station1 = getVertex(g, output[i]);

		if (station1 == NULL) {
			printf("station1 at printPath is null\n");
			exit(0);
		}

		char*edgeValue = getEdgeValue(g, output[i], output[i - 1]);

		if (edgeValue == NULL) {
			printf("Edge value is null\n");
			exit(0);
		}
		
		printf("--> %s (%s) \n", station1, jval_s(jrb_find_str(busTable, strdup(edgeValue))->val));
	}

	printf("--> %s\n", getVertex(g, output[i]));
}
//-------------------------------------------------------------------
void printVertices(Graph g)
{
	JRB node;
	jrb_traverse(node, g.vertices) {
		printf("%4d - %s\n", jval_i(node->key), jval_s(node->val));
	}
}
//-------------------------------------------------------------------
void printEdges(Graph g) 
{
	JRB node;
	JRB node2;

	jrb_traverse(node, g.vertices) {
		jrb_traverse(node2, g.vertices) {
			char *temp = getEdgeValue(g, jval_i(node->key), jval_i(node2->key));
			if (temp != NULL)
				printf("%s\n", temp);
		}
	}
}
//-------------------------------------------------------------------
void printShortestPath(Graph g, int start, int stop, int *size, int *path) 
{
	JRB d = make_jrb();
	JRB parent_node = make_jrb();
	int shortest = shortestPath(g, start, stop, d, parent_node);

	if (shortest == 0) 
		printf("\nROUTE NOT FOUND!\n");
	else {
		int current = 0, parent = 0;

		current = stop;
		parent = jval_i(jrb_find_int(parent_node, current)->val);
		path[*size] = current;

		while (current != parent) {
			path[*size] = current;
			current = parent;
			(*size)++;
			parent = jval_i(jrb_find_int(parent_node, current)->val);
			path[*size] = current;
		}

		(*size)++;
		path[*size] = start;
		
		printf("\nPATH BY ID  :\n\n   ");

		for (int j = (*size) - 1; j >= 0; j--) {
			printf("%d ", path[j]);
		}
		printf("\n");
	}

	jrb_free_tree(d);
	jrb_free_tree(parent_node);
}
//-------------------------------------------------------------------
int main() 
{
	// Graph variables
	Graph g = createGraph();
	JRB busTable = createBusTable();
	JRB lineTable = make_jrb();
	int choice = 0, id1 = 0, id2 = 0, flag = 0;
	char *name1 = (char*)malloc(sizeof(char));
	char *name2 = (char*)malloc(sizeof(char));
	char *bus[DATA];
	char *file1[DATA];
	char *file2[DATA];
	
	char busstopS[DATA], busB[DATA];

	// File Handling
	FILE *f1, *f2;

	f1 = fopen("../data/departure.txt", "rt"); 
	f2 = fopen("../data/return.txt", "rt");

	if (f1 == NULL) {
		printf("\n   ERROR! Departure file not found\n");
		exit(0);
	}

	if (f2 == NULL) {
		printf("\n   ERROR! Return file not found\n");
		exit(0);
	}

	// Procedures
	readFile(f1, f2);
	copyData(bus, file1, file2);
	makeBusTable(busTable, sizeBuses, buses);
	dataProcess(g, busTable);
	adjustBusVal(busTable);
	
	// Menu
	do {
		printf("\n---------------------------------------------------\n");
		printf("DSAA ADVANCED LAB - DDT'S PROJECT 2 - HANOI BUSLINE \n");
		printf("---------------------------------------------------\n\n");
		printf("1. Bus stations at which bus B stops.\n");
		printf("2. Shortest path from 1 to 2.\n");
		printf("3. Buses which stop at bus station S.\n");
		printf("4. Delete graph and exit the project.\n\n");
		printf("---------------------------------------------------\n");
		printf("--> Enter your choice: ");
		scanf("%d", &choice);
		printf("---------------------------------------------------\n");

		switch(choice) {
			case 1:
				printFileData(bus, file1, file2);
				printBuses(busTable);
				break;

			case 2:
				printVertices(g);

				printf("\nSHORTEST PATH\n\n");
				printf("--> Enter starting point ID: ");
				scanf("%d", &id1); 
				getchar();

				if (getVertex(g, id1) == NULL) 
					printf("  ERR! Bus station not found!\n");
				else {
					printf("--> Enter destination ID   : ");
					scanf("%d", &id2); 
					getchar();

					if (getVertex(g, id2) == NULL) {
						printf("\n  ERR! Bus station not found!\n\n");
					}
					else if (id1 == id2) {
						printf("\n  ERR! Starting point and Destination ID could not be the same!\n\n");
					}
					else {
						int output[DATA], outputSize = 0;

						printShortestPath(g, id1, id2, &outputSize, output);
						printPath(g, busTable, &outputSize, output);
					}
				}
				break;
			
			case 3: 
				printf("  --> Enter the ID of bus station S: ");
				scanf(" %[^\n]*c", busstopS);
				break;

			case 4:
				jrb_free_tree(busTable);
				freeGraph(g);
				printf("Graph deleted! Exit the Program...\n");
				return 0;
				break;
		}
	} 
	while (choice != 5);

	return 0;
}
//-------------------------------------------------------------------
