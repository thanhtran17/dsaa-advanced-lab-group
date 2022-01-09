#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include "bus.h"
#define INT_MAX 9999
#define DATA 1000
//-------------------------------------------------------------------
typedef struct distance
{
	char v1[256];
	char v2[256];
	double d;
} Distance;
char *buses[DATA];
char *readHolder1[DATA]; // go
char *readHolder2[DATA]; // back
Distance data[DATA];
int sizeData = 0;
int busLine[DATA][DATA]; // keep the number of lines that a bus takes
int sizeHolder = 0;			 // size of above vars
int sizeBuses = 0;			 // number of buses
int row = 0, col = 0;
// Lưu trữ dữ liệu bus và các tuyến nó đi qua
Graph b_s;
//-------------------------------------------------------------------
void copyData(char *bus[], char *file1[], char *file2[])
{
	for (int i = 0; i < sizeHolder; i++)
	{
		file1[i] = strdup(readHolder1[i]);
		file2[i] = strdup(readHolder2[i]);
	}

	for (int i = 0; i < sizeBuses; i++)
	{
		bus[i] = strdup(buses[i]);
	}
}
//-------------------------------------------------------------------
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
void readFile(FILE *f1, FILE *f2, FILE *f3)
{
	int i = 0, k = 0, flag = 0;
	char *reader = (char *)malloc(sizeof(char));
	int fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
	buses[k] = strdup(reader);
	k++;
	busLine[row][col] = k;

	while (fscanfChecker == 1)
	{
		flag = 0;
		fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
		if (fscanfChecker != 1)
			break;
		if (strchr(strdup(reader), '-') != NULL)
		{
			readHolder1[i] = strdup(reader);
			flag = 1;
			i++;
			busLine[row][col + 1] = busLine[row][col + 1] + 1;
		}
		if (flag == 0)
		{
			buses[k] = strdup(reader);
			row++;
			k++;
			busLine[row][col] = k;
		}
	}
	sizeHolder = i;
	sizeBuses = k;
	i = 0;
	fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
	while (fscanfChecker == 1)
	{
		fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
		if (fscanfChecker != 1)
			break;
		if (strchr(strdup(reader), '-') != NULL)
		{
			readHolder2[i] = strdup(reader);
			i++;
		}
	}

	// File distance
	char v1[256];
	char v2[256];
	char distance[25];
	while (fscanf(f3, "%s / %s / %s%*c", v1, v2, distance) != -1)
	{
		double d;
		sscanf(distance, "%lf", &d);
		// printf("%s %s %g\n", v1, v2, d);
		strcpy(data[sizeData].v1, v1);
		strcpy(data[sizeData].v2, v2);
		data[sizeData].d = d;
		sizeData++;
	};
}
//-------------------------------------------------------------------
void dataProcess(Graph g, JRB bus)
{
	int i = 0, count = 0;
	int vertexID = 0;
	char *linesplitter[DATA];
	char *temp = "-";
	for (i = 0; i <= row; i++)
	{
		for (int m = 0; m < busLine[i][col + 1]; m++)
		{
			int j = 0, k = 0;
			linesplitter[j] = strtok(readHolder1[count], temp);
			while (linesplitter[j] != NULL)
			{
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}
			linesplitter[j] = strtok(readHolder2[count], temp);
			while (linesplitter[j] != NULL)
			{
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}

			char weight[10];
			weight[0] = (i + 1) + '0';
			weight[1] = '\0';
			for (int k = 0; k < j; k++)
			{
				trim(linesplitter[k]);
				addStations(g, linesplitter[k], &vertexID);
				makeBusStation(b_s, i + 1, vertexID - 1, linesplitter[k]);
			}

			for (int k = 0; k < j - 1; k++)
			{
				// Thêm cạnh giữa 2 vertices liên tiếp
				double a = -1;
				for (int i = 0; i < sizeData; i++)
				{
					if ((strcmp(data[i].v1, linesplitter[k]) == 0) && (strcmp(data[i].v2, linesplitter[k + 1]) == 0))
					{
						a = data[i].d;
					}
				}
				if (a != -1 && strcmp(linesplitter[k], linesplitter[k + 1]) != 0)
				{
					// printf("Thêm edge giữa %s và %s\n", linesplitter[k], linesplitter[k + 1]);
					int x1 = getVertexID(g, linesplitter[k]);
					int x2 = getVertexID(g, linesplitter[k + 1]);
					if (x1 != -1 && x2 != -1 && x1 != x2)
					{
						addEdge(g, x1, x2, a);
					}
					a = -1;
				}
			}
			count++;
		}
	}
}
//-------------------------------------------------------------------
void printFileData(char *bus[], char *file1[], char *file2[])
{
	int count = 0;
	for (int i = 0; i < sizeBuses; i++)
	{
		printf("BUS: %s\n", bus[i]);
		for (int j = 0; j < busLine[i][col + 1]; j++)
		{
			printf("CHIEU DI: %s\n\n", file1[count]);
			printf("CHIEU VE: %s\n\n", file2[count]);
			count++;
		}
	}
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int main()
{
	// Graph variables
	Graph g = createGraph();
	b_s = createGraph();
	JRB busTable = createBusTable();
	JRB busInfo = createBusInfo();
	JRB lineTable = make_jrb();
	int choice = 0, id1 = 0, id2 = 0, flag = 0;
	char *name1 = (char *)malloc(sizeof(char));
	char *name2 = (char *)malloc(sizeof(char));
	char *bus[DATA];
	char *file1[DATA];
	char *file2[DATA];
	int path[256];
	int length = 0;
	double total;
	char busstopS[DATA], busB[DATA];
	char name[256];
	int t1, t2;
	int id_bus;
	// File Handling
	FILE *f1, *f2, *f3;

	f1 = fopen("../data/departure.txt", "rt");
	f2 = fopen("../data/return.txt", "rt");
	f3 = fopen("../data/bus.txt", "rt");

	if (f1 == NULL)
	{
		printf("\nERROR! Departure file not found\n");
		exit(0);
	}

	if (f2 == NULL)
	{
		printf("\nERROR! Return file not found\n");
		exit(0);
	}
	if (f3 == NULL)
	{
		printf("\nERROR! Return file not found\n");
		exit(0);
	}

	// Procedures
	readFile(f1, f2, f3);
	copyData(bus, file1, file2);
	dataProcess(g, busTable);
	makeBusesTable(busTable, buses, sizeBuses);
	// findAllBusesGoFromStation(g, busTable, "VAN_PHUC");
	// total = shortestPath(g, 5, 25, path, &length);
	// if (total == INT_MAX)
	// {
	// 	printf("ROUTE NOT FOUND\n");
	// 	return;
	// }
	// printf("Length: %g\n", total);
	// for (int i = length - 1; i >= 0; i--)
	// {
	// 	printf("%d->", path[i]);
	// }
	// printf("%d\n", 103);
	// printAllStations(g);
	// JRB node;
	// jrb_traverse(node, g.edges)
	// {
	// 	JRB tree = jval_v(node->val);
	// 	JRB child;
	// 	jrb_traverse(child, tree)
	// 	{
	// 		printf("%d->%d = %g\n", jval_i(node->key), jval_i(child->key), jval_d(child->val));
	// 	}
	// }
	// findAllBusesGoFromStation(b_s, busTable);
	// printAllBus(busTable);
	// findStationsFromBus(b_s, 9);

	// JRB node;
	// jrb_traverse(node, g.vertices)
	// {
	// 	printf("%d %s\n", jval_i(node->key), jval_s(node->val));
	// }

	// Menu
	do
	{
		printf("\n---------------------------------------------------\n");
		printf("DSAA ADVANCED LAB - DDT'S PROJECT 2 - HANOI BUSLINE \n");
		printf("---------------------------------------------------\n\n");
		printf("1. Tìm đường đi ngắn nhất giữa 2 bến xe buýt\n");
		printf("2. In ra tất cả các xe buýt đi qua 1 bến.\n");
		printf("3. In ra tất cả bến đi mà qua 1 xe buýt\n");
		printf("4. Kết thúc chương trình, xóa toàn bộ dữ liệu\n\n");
		printf("---------------------------------------------------\n");
		printf("--> Lựa chọn của bạn: ");
		scanf("%d", &choice);
		printf("---------------------------------------------------\n");
		switch (choice)
		{
		case 1:
			printAllStations(g);
			printf("Nhập id của 2 bến xe:\n");
			scanf("%d%d", &t1, &t2);
			total = shortestPath(g, t1, t2, path, &length);
			if (total == INT_MAX)
			{
				printf("Không có đường đi nào giữa 2 điểm này\n");
			}
			else
			{
				printf("Độ dài ngắn nhất: %g\n", total);
				for (int i = length - 1; i >= 0; i--)
				{
					printf("%d->", path[i]);
				}
				printf("%d\n", t2);
			}
			break;

		case 2:
			printAllStations(g);
			findAllBusesGoFromStation(b_s, busTable);
			break;
		case 3:
			printAllBus(busTable);
			printf("Nhập địa chỉ của buýt (số):\n");
			scanf("%d", &id_bus);
			findStationsFromBus(b_s, id_bus);
			break;

		case 4:
			jrb_free_tree(busTable);
			jrb_free_tree(busInfo);
			jrb_free_tree(lineTable);
			freeGraph(g);
			freeGraph(b_s);
			printf("Tạm biệt NGÀI \n");
			return 0;
			break;
		}
	} while (choice != 5);
	return 0;
}
//-------------------------------------------------------------------
