#include <stdio.h>
#include <math.h>
#include "jval.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SMALL_NUMBER 20
#define HUGE_NUMBER 100000
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void testJval()
{
    Jval a;
    a.i = 5;
    printf("%d", a.i);
}
int compare(void *x, void *y)
{
    int m = *((int *)x);
    int n = *((int *)y);
    return (m > n)
               ? 1
           : (m == n)
               ? 0
               : -1;
}
int compare_i(Jval *a, Jval *b)
{
    return ((*a).i > (*b).i)
               ? 1
           : ((*a).i == (*b).i)
               ? 0
               : -1;
}
void exch(void *a, void *b)
{
    int temp = *((int *)a);
    *((int *)a) = *((int *)b);
    *((int *)b) = temp;
}
void exch_jval(void *a, void *b)
{
    Jval temp = *((Jval *)a);
    *((Jval *)a) = *((Jval *)b);
    *((Jval *)b) = temp;
}
Jval *create_array_i(int size)
{
    Jval *arr = (Jval *)malloc(sizeof(Jval) * size);
    srand(time(NULL));
    for (int j = 0; j < size; j++)
    {
        arr[j].i = rand() % size;
    }
    return arr;
}
int search_gen(Jval arr[], int l, int r, int x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        if (arr[mid].i == x)
            return mid;
        if (arr[mid].i > x)
            return search_gen(arr, l, mid - 1, x);
        return search_gen(arr, mid + 1, r, x);
    }
    return -1;
}
void sort_gen(Jval x[], int l, int r, int (*compare)(Jval *, Jval *))
{
    int i, j, p, q, k;
    Jval pivot;
    i = l - 1;
    j = r;
    p = l - 1;
    q = r;
    pivot = x[r];
    if (r <= l)
        return;
    for (;;)
    {
        while (compare(&x[++i], &pivot) == -1)
            ;
        while (compare(&x[--j], &pivot) == 1)
        {
            if (j == l)
                break;
        }
        if (i >= j)
            break;
        exch_jval(&x[i], &x[j]);
        if (compare(&x[i], &pivot) == 0)
        {
            p++;
            exch_jval(&x[p], &x[i]);
        }
        if (compare(&x[j], &pivot) == 0)
        {
            q--;
            exch_jval(&x[q], &x[j]);
        }
    }
    exch_jval(&x[i], &x[r]);
    j = i - 1;
    i++;
    for (k = l; k < p; k++, j--)
        exch_jval(&x[k], &x[j]);
    for (k = r - 1; k > q; k--, i++)
        exch_jval(&x[i], &x[k]);
    sort_gen(x, l, j, compare);
    sort_gen(x, i, r, compare);
}
void printSortJval(Jval arr[], int length)
{
    for (int j = 0; j < length; j++)
    {
        printf("%d ", arr[j].i);
    }
    printf("\n");
}
int main()
{
    int n = SMALL_NUMBER;
    Jval *arr3 = create_array_i(n);
    sort_gen(arr3, 0, n - 1, compare_i);
    printSortJval(arr3, n);
}