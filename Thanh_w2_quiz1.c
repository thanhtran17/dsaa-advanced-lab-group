#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define num 10000000
//------------------------------------------------------------------------
int compare(void * x, void * y)
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
//------------------------------------------------------------------------
int *createArray(int n){
	int i;
	int *a = (int*) malloc(n * sizeof(int));

	srand(time(NULL));
	for (i = 0; i < n; i++) 
		*(a+i) = 1 + rand() % (num - 1);

	return a;
}
//------------------------------------------------------------------------
int *dumbArray(int *a, int n){
	int *b = (int*) malloc(n * sizeof(int));
	memcpy(b, a, n * sizeof(int));

	return b;
}
//------------------------------------------------------------------------
void printArray(int a[], int n)
{
	for (int i = 0; i < n; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}
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
//------------------------------------------------------------------------
int partition (int a[], int l, int r,                
               void (*exch) (void*, void*))
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
//------------------------------------------------------------------------
void quickSort(void* buf, int l, int r,              
               void (*exch) (void*, void*))
{
  if (l < r){
    int pi = partition(buf, l, r, exch);
    quickSort(buf, l, pi - 1, exch);
    quickSort(buf, pi + 1, r, exch);
  }
}
//------------------------------------------------------------------------
void quickSort3(int a[], int l, int r) {
	if (r > l){ 
		int i = l - 1, j = r;
		int p = l - 1, q = r;
		int k;

		while(true) {
			while (a[++i] < a[r]);
			while (a[r] < a[--j]) 
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
	
		quickSort3(a, l, j);
		quickSort3(a, i, r);
	}
}
//------------------------------------------------------------------------
int main()
{	
	int *a, *b, *c, i, item = 12, res;
	double start2, end2, start3, end3, start_bi, end_bi,
				 intervalTwoWay, intervalThreeWay, intervalBuiltIn;

  a = createArray(num);
  b = dumbArray(a, num);
  c = dumbArray(a, num);
  //
  start2 = clock();
  //printArray(a, num);
	quickSort(a, 0, num - 1, exch);
  //printArray(a, num);
  end2 = clock();
  intervalTwoWay = (double) (end2 - start2) / CLOCKS_PER_SEC;
  printf("Index for item: \n%d\n", search(a, sizeof(int), 0, num - 1, &item, compare));
	
  //
  start3 = clock();
  //printArray(b, num);
  quickSort3(b, 0, num - 1);
  //printArray(b, num);
	end3 = clock();
  intervalThreeWay = (double) (end3 - start3) / CLOCKS_PER_SEC;
  printf("Index for item: \n%d\n", search(b, sizeof(int), 0, num - 1, &item, compare));

  //
	start_bi = clock();
  //printArray(c, num);
  qsort(c, num - 1, sizeof(int), compare);
  //printArray(c, num);
	end_bi = clock();
  intervalBuiltIn = (double) (end_bi - start_bi) / CLOCKS_PER_SEC;
  printf("Index for item: \n%d\n", search(c, sizeof(int), 0, num - 1, &item, compare));
  
  //
	printf("2-way partition quicksort takes: %f seconds \n", intervalTwoWay);
	printf("3-way partition quicksort takes: %f seconds \n", intervalThreeWay);
	printf("built-in quicksort takes: %f seconds \n", intervalBuiltIn);

	free(a);
	free(b);
	free(c);
	
  return 0;
}