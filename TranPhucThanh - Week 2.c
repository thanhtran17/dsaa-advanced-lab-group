#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include <math.h>
#include "jval.h"
#include <string.h>
#define num 76000
//----------------------------------------------------------------------------
//---------------------------- CÁC HÀM BỔ TRỢ --------------------------------
int compare(void *x, void *y)
{
	int m = *((int*) x),
			n = *((int*) y);
	if (m == n) 
		return 0;
	
	return m > n ? 1 : -1;
}
//----------------------------------------------------------------------------
void exch(void *a, void *b)
{
    int temp = *((int *)a);
    *((int *)a) = *((int *)b);
    *((int *)b) = temp;
}
//----------------------------------------------------------------------------
int *createArray(int n){
	int i;
	int *a = (int*) malloc(n * sizeof(int));

	srand(time(NULL));
	for (i = 0; i < n; i++) 
		*(a+i) = 1 + rand() % 9;

	return a;
}
//----------------------------------------------------------------------------
int *dumbArray(int *a, int n){
	int *b = (int*) malloc(n * sizeof(int));
	memcpy(b, a, n * sizeof(int));

	return b;
}
//----------------------------------------------------------------------------
void printArray(int a[], int n)
{
  printf("Array: ");
	for (int i = 0; i < n; i++){
		printf("%d ", a[i]);
	}
	printf("\n\n");
}
//----------------------------------------------------------------------------
//------------------------------ SEARCHES ------------------------------------
int search(
           void* buf, 
           int size, 
           int l, int r, 
           void *item, 
           int (*compare) (void*, void*)
          )
{
  int i, res;
  if (l <= r) {
    i = (l + r) / 2;
    res = compare(item, (char*) buf + (size * i));
    if (res == 0)
      return i;
    else if (res < 0)
      return search(buf, size, l, i - 1, item, compare);
    else 
      return search(buf, size, i + 1, r, item, compare);
  }
}
//----------------------------------------------------------------------------
//-------------------------------- SORTS -------------------------------------
int partition (
                int a[], int l, int r, 
                void (*exch) (void*, void*)
              )
{
  int pivot = a[r];
  int left = l,
  		right = r - 1;

  while(true) {
    while(left <= right && a[left] < pivot) left++;
    while(left <= right && a[right] > pivot) right--;

    if (left >= right) break;
    exch(&a[left], &a[right]);
    left++;
    right--;
  }

	exch(&a[left], &a[r]);

  return left;
}
//----------------------------------------------------------------------------
void quickSort(
               void* buf, 
               int l, int r,
               void (*exch) (void*, void*)
              )
{
  if (l < r){
    int pi = partition(buf, l, r, exch);
    quickSort(buf, l, pi - 1, exch);
    quickSort(buf, pi + 1, r, exch);
  }
}
//----------------------------------------------------------------------------
void quickSort3(
               int a[], 
               int l, int r,
               void (*exch) (void*, void*)
              )
{
	if (r > l){ 
		int i = l - 1, j = r;
		int p = l - 1, q = r;
		int k;

		while(true) {
			while (a[++i] < a[r]);
			while (a[l] < a[--j]) 
				if (j == l) break;

			if (i >= j) break;
			exch(&a[i], &a[j]);

			if (a[i] == a[r])
		  	exch(&a[++p], &a[i]);
			if (a[j] == a[r]) 
				exch(&a[--q], &a[j]);
		}

		exch(&a[i], &a[r]);
		j = i - 1;
		i = i + 1;
	
		for (k = l ; k <= p; k++) 
			exch(&a[k], &a[j--]);		
		for (k = r - 1; k >= q; k--) 
			exch(&a[k], &a[i++]);
	
		quickSort3(a, l, j, exch);
		quickSort3(a, i, r, exch);
	}
}
//----------------------------------------------------------------------------
//--------------------------------- MAIN -------------------------------------
int main()
{
  int i, res, item = 11;
  int *a, *b, *c;
	double start2, end2, start3, end3, start_bi, end_bi,
				 intervalTwoWay, intervalThreeWay, intervalBuiltIn;

  //
  a = createArray(num);
  b = dumbArray(a, num);
  c = dumbArray(a, num);
  //
  srand(time(NULL));
  for (i = 0; i < num; i++)
    a[i] = rand() % num;
  
  // Built-in qsort
 	start_bi = clock();
  qsort(a, num, sizeof(int), compare);
  printf("-- built-in qsort -- \n");
  //printArray(a, num);
 	end_bi = clock();
  intervalBuiltIn = (double) (end_bi - start_bi) / CLOCKS_PER_SEC;
  res = search(a, sizeof(int), 0, num - 1, &item, compare);
  printf("Index for item: \n%d\n", res);
  printf("--------------------------------------\n");

  // 2-way qsort
  start2 = clock();
  quickSort(a, 0, num - 1, exch);
  printf("-- 2-way qsort -- \n");
  //printArray(a, num);
  end2 = clock();
  intervalTwoWay = (double) (end2 - start2) / CLOCKS_PER_SEC;
  res = search(a, sizeof(int), 0, num - 1, &item, compare);
  printf("Index for item: \n%d\n", res);
  printf("--------------------------------------\n");

  // 3-way qsort
 	start3 = clock();
  quickSort3(a, 0, num - 1, exch);
  printf("-- 3-way qsort -- \n");
  // printArray(a, num);
	end3 = clock();
  intervalThreeWay = (double) (end3 - start3) / CLOCKS_PER_SEC;
  res = search(a, sizeof(int), 0, num - 1, &item, compare);
  printf("Index for item: \n%d\n", res);
  printf("--------------------------------------\n");
  //
  printf("2-way partition quicksort takes: %f seconds \n", intervalTwoWay);
	printf("3-way partition quicksort takes: %f seconds \n", intervalThreeWay);
	printf("built-in quicksort takes: %f seconds \n", intervalBuiltIn);
  //
  free(a);
  free(b);
  free(c);

  return 0;
}
//----------------------------------------------------------------------------

