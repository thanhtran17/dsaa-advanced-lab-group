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
//-------------------------------------------------------------------
