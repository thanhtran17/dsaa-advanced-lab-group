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

void copyData(char *bus[], char *file1[], char *file2[]);
void trim(char *line);
void readFile(FILE *f1, FILE *f2, FILE *f3);
void dataProcess(Graph g, JRB bus);