#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bus.h"
#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#define INT_MAX 9999
#define MAX 100000
//-------------------------------------------------------------------
Graph createGraph()
{
	Graph graph;

	graph.edges = make_jrb();
	graph.vertices = make_jrb();

	return graph;
}
void trim(char *line)
{
	int i = strlen(line);

	while (i > 0 && isspace(line[i - 1]))
	{
		--i;
	}

	line[i] = '\0';
	i = strspn(line, " \t\n\v");
	memmove(line, line + i, strlen(line + i) + 1);
}
//-------------------------------------------------------------------
void addVertex(Graph g, int id, char *name)
{
	JRB v = jrb_find_int(g.vertices, id);

	if (v == NULL)
	{
		jrb_insert_int(g.vertices, id, new_jval_s(name));
	}

	return;
}
//-------------------------------------------------------------------
char *getVertex(Graph g, int id)
{
	JRB v = jrb_find_int(g.vertices, id);

	if (v == NULL)
	{
		return NULL;
	}
	else
	{
		return jval_s(v->val);
	}
}
int getVertexID(Graph g, char *name)
{
	int count = -1;
	JRB node;
	jrb_traverse(node, g.vertices)
	{
		if (strcmp(jval_s(node->val), name) == 0)
		{
			count = jval_i(node->key);
		}
	}
	return count;
}
double getLengthStations(Graph g){
	JRB node;
	double i = 0;
	jrb_traverse(node, g.vertices){
		i++;
	}
	return i;
}
//-------------------------------------------------------------------
void addEdge(Graph g, int v1, int v2, double weight)
{
	JRB tree;
	JRB node = jrb_find_int(g.edges, v1);

	if (node == NULL)
	{
		tree = make_jrb();
		jrb_insert_int(g.edges, v1, new_jval_v(tree));
	}
	else
	{
		tree = (JRB)jval_v(node->val);
	}
	if (v1 != v2)
	{
		jrb_insert_int(tree, v2, new_jval_d(weight));
	}
}
//-------------------------------------------------------------------
int hasEdge(Graph g, int v1, int v2)
{
	JRB node = jrb_find_int(g.edges, v1);

	if (node == NULL)
	{
		return -1;
	}
	else
	{
		JRB tree = (JRB)jval_v(node->val);
		JRB temp = jrb_find_int(tree, v2);

		if (temp == NULL)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
//-------------------------------------------------------------------
char *getEdgeValue(Graph g, int v1, int v2)
{
	if (hasEdge(g, v1, v2) == -1)
	{
		return NULL;
	}
	else if (hasEdge(g, v1, v2) == 0)
	{
		return NULL;
	}
	else
	{
		JRB node = jrb_find_int(g.edges, v1);
		JRB tree = (JRB)jval_v(node->val);
		JRB temp = jrb_find_int(tree, v2);

		return jval_s(temp->val);
	}
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int indegree(Graph g, int v, int *output)
{
	JRB tree, node;
	int total = 0;

	jrb_traverse(node, g.edges)
	{
		tree = (JRB)jval_v(node->val);
		if (jrb_find_int(tree, v))
		{
			output[total] = jval_i(node->key);
			total++;
		}
	}

	return total;
}
//-------------------------------------------------------------------
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
double getEdgeValueDouble(Graph graph, int v1, int v2)
{
	JRB node = jrb_find_int(graph.edges, v1);
	if (node == NULL)
		return 0;
	JRB endPoint = jrb_find_int(jval_v(node->val), v2);
	return (endPoint == NULL) ? 0 : jval_d(endPoint->val);
}
//-------------------------------------------------------------------
void insertionSort(int a[], int n, node_ node[])
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
double shortestPath(Graph graph, int s, int t, int path[], int *length)
{
	JRB node;
	node_ node_arr[1000];
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
		node_arr[jval_i(node->key)].val = INT_MAX;
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
			if (node_arr[v].val > node_arr[u].val + getEdgeValueDouble(graph, u, v))
			{
				node_arr[v].val = node_arr[u].val + getEdgeValueDouble(graph, u, v);
				node_arr[v].parent = u;
			}
		}
		number--;
	};
	node_ destination = node_arr[t];
	int a;
	int total_weight = destination.val;
	while (destination.parent != -1)
	{
		a = destination.parent;
		path[(*length)++] = a;
		destination = node_arr[a];
	}
	// printf("F\n");
	return total_weight;
};
//-------------------------------------------------------------------
void printVertex(int v)
{
	printf("%4d ", v);
}

//-------------------------------------------------------------------
void addStations(Graph g, char *station, int *vertexID)
{
	int flag = 0;
	JRB node;

	jrb_traverse(node, g.vertices)
	{
		if (strcmp(getVertex(g, jval_i(node->key)), station) == 0)
		{
			flag = 1;
			break;
		}
	}

	if (flag == 0)
	{
		addVertex(g, *vertexID, station);
		(*vertexID)++;
	}
}
//-------------------------------------------------------------------
void makeBusesTable(JRB busTable, char *buses[], int size)
{
	JRB node;

	for (int i = 0; i < size; i++)
	{
		if (jrb_find_int(busTable, i + 1) == NULL)
		{
			trim(buses[i]);
			jrb_insert_int(busTable, i + 1, new_jval_s(buses[i]));
		}
	}
}
int checkCollide(JRB table, char *name)
{
	JRB node;
	jrb_traverse(node, table)
	{
		if (strcmp(jval_s(node->val), name) == 0)
		{
			return 1;
		}
	}
	return 0;
}
void makeBusStation(Graph graph, int id_bus, int id_station, char *station_name)
{
	JRB tree;
	JRB node = jrb_find_int(graph.edges, id_bus);
	if (node == NULL)
	{
		tree = make_jrb();
		jrb_insert_int(graph.edges, id_bus, new_jval_v(tree));
	}
	else
	{
		tree = (JRB)jval_v(node->val);
	}
	if (checkCollide(tree, station_name) == 0)
	{
		jrb_insert_int(tree, id_station, new_jval_s(station_name));
	}
}
void findStationsFromBus(Graph graph, JRB busTable, char name[256] , char allStation[1000])
{
	int id_bus = 0;
	JRB node_;
	jrb_traverse(node_, busTable){
		if (strcmp(jval_s(node_->val), name) == 0){
			printf("Find");
			id_bus = jval_i(node_->key);
		}
	}
	printf("%d\n", id_bus);
	JRB node;
	JRB ptr;
	jrb_traverse(node, graph.edges)
	{
		if (jval_i(node->key) == id_bus)
		{
			printf("Tất cả các bến mà xe buýt có địa chỉ %d đi qua\n", id_bus);
			JRB tree = jval_v(node->val);
			jrb_traverse(ptr, tree)
			{
				strcat(allStation, jval_s(ptr->val));
				strcat(allStation, " ");
				printf("%s\n", jval_s(ptr->val));
			}
			return;
		}
	}
	strcpy(allStation, "Không có bến nào đi qua địa chỉ buýt này!");
	printf("Không có bến nào đi qua địa chỉ buýt này!\n");
	return;
}
//-------------------------------------------------------------------
void printAllBus(JRB busTable)
{
	JRB node;
	jrb_traverse(node, busTable)
	{
		printf("%d %s\n", jval_i(node->key), jval_s(node->val));
	}
}
//-------------------------------------------------------------------
int printAllStations(Graph g)
{
	int length = 0;
	JRB node;
	jrb_traverse(node, g.vertices)
	{
		length++;
		printf("%d %s\n", jval_i(node->key), jval_s(node->val));
	}
	return length;
}
int getIdStation(Graph g, char * name)
{
	JRB node;
	jrb_traverse(node, g.vertices)
	{
		if (strcmp(jval_s(node->val), name) == 0){
			return jval_i(node->key);
		}
	}
	return -1;
}
char * getNameStation(Graph g, int id)
{
	JRB node;
	jrb_traverse(node, g.vertices)
	{
		if (jval_i(node->key) == id){
			return jval_s(node->val);
		}
	}
	return "NOT FOUND";
}

//-------------------------------------------------------------------
void findAllBusesGoFromStation(Graph graph, JRB busTable, char allBus[1000], char name[256])
{
	JRB node;
	int flag = 0;
	jrb_traverse(node, graph.edges)
	{
		JRB tree = jval_v(node->val);
		JRB ptr;
		jrb_traverse(ptr, tree)
		{
			if (strcmp(jval_s(ptr->val), name) == 0)
			{
				flag = 1;
				JRB find_node = jrb_find_int(busTable, jval_i(node->key));
				strcat(allBus, jval_s(find_node->val));
				strcat(allBus, " ");
				printf("%s\n", jval_s(find_node->val));
			}
		}
	}
	if (flag == 0)
	{
		strcat(allBus, "Không có station này");
		printf("Không có station này\n");
	}
	return;
}
//-------------------------------------------------------------------
JRB createBusTable()
{
	JRB busTable = make_jrb();
	return busTable;
}
JRB createBusInfo()
{
	JRB busInfo = make_jrb();
	return busInfo;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void freeGraph(Graph graph)
{
	JRB node, tree;

	jrb_traverse(node, graph.edges)
	{
		tree = (JRB)jval_v(node->val);
		jrb_free_tree(tree);
	}

	jrb_free_tree(graph.edges);
	jrb_free_tree(graph.vertices);
}
//-------------------------------------------------------------------
