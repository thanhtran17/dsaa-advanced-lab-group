#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define num 10000000
//------------------------------------------------------------------------
int *createArray(int n){
	int i;
	int *a = (int*) malloc(n * sizeof(int));

	srand(time(NULL));
	for (i = 0; i < n; i++) 
		*(a+i) = 1 + rand() % 9;

	return a;
}
//------------------------------------------------------------------------
int *dumbArray(int *a, int n){
	int *b = (int*) malloc(n * sizeof(int));
	memcpy(b, a, n * sizeof(int));

	return b;
}
//------------------------------------------------------------------------
void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}
//------------------------------------------------------------------------
int partition (int a[], int low, int high)
{
  int pivot = a[high];
  int left = low,
  		right = high - 1;

  while(true) {
    while(left <= right && a[left] < pivot) left++;
    while(left <= right && a[right] > pivot) right--;

    if (left >= right) break;
    swap(&a[left], &a[right]);
    left++;
    right--;
  }
  
	swap(&a[left], &a[high]);
  return left;
}
//------------------------------------------------------------------------
void quickSort(int a[], int low, int high)
{
  if (low < high){
    int pi = partition(a, low, high);
    quickSort(a, low, pi - 1);
    quickSort(a, pi + 1, high);
  }
}
//------------------------------------------------------------------------
int partition3 (int a[], int low, int high)
{
	
}
//------------------------------------------------------------------------
void quickSort3(int a[], int low, int high) {
	if (high > low){ 
		int i = low - 1, j = high;
		int p = low - 1, q = high;
		int k;

		while(true) {
			while (a[++i] < a[high]);
			while (a[high] < a[--j]) 
				if (j == low) break;

			if (i >= j) break;
			swap(&a[i], &a[j]);

			if (a[i] == a[high])
		  	swap(&a[++p], &a[i]);
			if (a[j] == a[high]) 
				swap(&a[--q], &a[j]);
		}

		swap(&a[i], &a[high]);
		j = i - 1;
		i = i + 1;
	
		for (k = low ; k <= p; k++) 
			swap(&a[k], &a[j--]);		
		for (k = high - 1; k >= q; k--) 
			swap(&a[k], &a[i++]);
	
		quickSort3(a, low, j);
		quickSort3(a, i, high);
	}
}
//------------------------------------------------------------------------
int int_compare(void const* x, void const* y)
{
	int m = *((int*) x),
			n = *((int*) y);
	if (m == n) 
		return 0;
	
	return m > n ? 1 : -1;
}
//------------------------------------------------------------------------
void printArray(int a[], int n)
{
	for (int i = 0; i < n; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}
//------------------------------------------------------------------------
int main()
{	
	int *a, *b, *c, i;
	double start2, end2, start3, end3, start_bi, end_bi,
				 intervalTwoWay, intervalThreeWay, intervalBuiltIn;

  a = createArray(num);
  b = dumbArray(a, num);
  c = dumbArray(a, num);

	start2 = clock();
  //printArray(a, num);
	quickSort(a, 0, num - 1);
  //printArray(a, num);
  end2 = clock();
  intervalTwoWay = (double) (end2 - start2) / CLOCKS_PER_SEC;
  
	start3 = clock();
  //printArray(b, num);
  quickSort3(b, 0, num - 1);
  //printArray(b, num);
	end3 = clock();
  intervalThreeWay = (double) (end3 - start3) / CLOCKS_PER_SEC;

	start_bi = clock();
  //printArray(b, num);
  qsort(c, num - 1, sizeof(int), int_compare);
  //printArray(b, num);
	end_bi = clock();
  intervalBuiltIn = (double) (end_bi - start_bi) / CLOCKS_PER_SEC;

	printf("2-way partition quicksort takes: %f seconds \n", intervalTwoWay);
	printf("3-way partition quicksort takes: %f seconds \n", intervalThreeWay);
	printf("built-in quicksort takes: %f seconds \n", intervalBuiltIn);

	free(a);
	free(b);
	free(c);
	
  return 0;
}