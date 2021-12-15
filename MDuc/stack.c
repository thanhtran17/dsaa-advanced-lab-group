#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stack.h"
#include <stdlib.h>
#define MAX 100000
int n = 0;
void initialize(int stack[])
{
    n = 0;
}
int isEmptySt(int stack[])
{
    return n == 0;
}
int isFull(int stack[])
{
    return n == MAX;
}
void push(int stack[], int data)
{
    if (isFull(stack))
        printf("Stack is out of data\n");
    else
    {
        stack[n] = data;
        n++;
    }
}
int pop(int stack[])
{
    if (isEmptySt(stack))
    {
        printf("There is no data left\n");
        return -1;
    }
    else
    {
        --n;
    }
    return stack[n];
}
void printStack(int stack[])
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", stack[i]);
    }
}