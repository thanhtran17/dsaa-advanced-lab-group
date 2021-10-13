#include "jval.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define num 100
//------------------------------------------------------------
Jval* create_array_i (int n) {
	Jval * a = (Jval *) malloc(n * sizeof(Jval));
  
  srand(time(NULL));
	for (int j = 0; j < n; j++) 
    a[j] = new_jval_i(rand() % n);
	return a;
}
//-------------------------------------------------------------
void printArray(Jval a[], int n)
{
	for (int j = 0; j < n; j++){
		printf("%d ", a[j].i);
	}
	printf("\n");
}
//------------------------------------------------------------
int compare_i(Jval *a, Jval *b)
{
  if ((*a).i > (*b).i)
    return 1;
  else if ((*a).i == (*b).i)
    return 0;
  else return -1;
}
//------------------------------------------------------------
void exch_jval(void *a, void *b)
{
    Jval temp = *((Jval *)a);
    *((Jval *)a) = *((Jval *)b);
    *((Jval *)b) = temp;
}
//------------------------------------------------------------
void sort_gen (
               Jval a[], int size, int l, int r, 
               int (*compare_i)(Jval*, Jval*) 
              )
{
    if (r <= l) return;
    
    int i = l - 1, j = r;
    int p = l - 1, q = r; 
    int k;
    Jval pivot = a[r];


    while(true){
      while (compare_i(&a[++i], &a[r]) < 0);
      while (compare_i(&a[--j], &a[r]) > 0) 
        if (j == l) break;

      if (i >= j) break;
      exch_jval(&a[i], &a[j]);
      
      if (compare_i(&a[i], &pivot) == 0)
        exch_jval(&a[++p], &a[i]);
      if (compare_i(&a[j], &pivot) == 0)
        exch_jval(&a[--q], &a[j]);
    }

    exch_jval(&a[i], &a[r]);
    j = i - 1;
    i = i + 1;
    
    for (k = l; k < p; k++, j--)
      exch_jval(&a[k], &a[j]);
    for (k = r - 1; k > q; k--, i++)
      exch_jval(&a[i], &a[k]);

    sort_gen(a, size, l, j, compare_i);
    sort_gen(a, size, i, r, compare_i);
}
//------------------------------------------------------------
void sort_i(Jval a[], int size, int l, int r)
{
  sort_gen(a, size, l, r, compare_i);
}
//------------------------------------------------------------
int main()
{
  Jval *a;

  a = create_array_i(num);
  printArray(a, num);
  sort_i(a, sizeof(int), 0, num);
  printArray(a, num);

  free(a);

  return 0;
}