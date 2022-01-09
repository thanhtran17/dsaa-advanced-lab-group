#include "dllist.h"
#include "jrb.h"
#include "jval.h"

typedef struct
{
	JRB edges;
	JRB vertices;
} Graph;
typedef struct node
{
	double val;
	int parent;
} node_;
Graph createGraph();
void printAllBus(JRB busTable);
void printAllStations(Graph g);
int getVertexID(Graph g, char *name);
double getEdgeValueDouble(Graph graph, int v1, int v2);
void insertionSort(int a[], int n, node_ node[]);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
void findAllBusesGoFromStation(Graph graph, JRB busTable);
void findStationsFromBus(Graph graph, int id_bus);
int checkCollide(JRB table, char *name);
void makeBusesTable(JRB busTable, char *buses[], int size);
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph g, int v1, int v2, double weight);
int hasEdge(Graph graph, int v1, int v2);
char *getEdgeValue(Graph g, int v1, int v2);
void printShortestPath(Graph g, int start, int stop, int *size, int *path);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
JRB createBusTable();
void readFile(FILE *f1, FILE *f2, FILE *f3);
void dataProcess(Graph g, JRB bus);
void freeGraph(Graph graph);
JRB createBusInfo();
void addStations(Graph g, char *station, int *vertexID);
void makeBusStation(Graph graph, int id_bus, int id_station, char *station_name);
