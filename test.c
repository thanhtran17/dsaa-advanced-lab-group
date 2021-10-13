#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define num 100
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
void exch (void* a, int size, int i, int j)
{  
    char* temp = (char*)malloc(size);
    memcpy(temp, (char*) a + i*size, size);
    memcpy((char*) a + i*size, (char*) a + j*size, size);
    memcpy((char*) a + j*size, temp, size);
    free(temp);
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
void quickSort3(void* a, int size, int l, int r,
                int (*compare) (void*, void*)
                ) 
{
	if (r > l){ 
		int i = l - 1, j = r;
		int p = l - 1, q = r;
		int k;

		while(true) {
      while (compare((char*) a + (++i) * size , (char*) a + r * size) < 0);
			while (compare((char*) a + r * size, (char*) a + (--j) * size) < 0) 
				if (j == l) break;
			if (i >= j) break;
			exch(a, size, i, j);

			if (compare((char*) a + j * size, (char*) a + r * size) == 0)
		  	exch(a, size, ++p, i);
			if (compare((char*) a + j * size, (char*) a + r * size) == 0) 
				exch(a, size, --q, j);
		}

		exch(a, size, i ,r);
		j = i - 1;
		i = i + 1;
	
		for (k = l ; k <= p; k++) 
			exch(a, size, k, j--);		
		for (k = r - 1; k >= q; k--) 
			exch(a, size, k, i++);
	
		quickSort3(a, size, l, j, compare);
		quickSort3(a, size, l, j, compare);
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

  //
  start3 = clock();
  printArray(a, num);
  quickSort3(a, num, 0, num - 1, compare);
  printArray(a, num);
	end3 = clock();
  intervalThreeWay = (double) (end3 - start3) / CLOCKS_PER_SEC;
  printf("Index for item: \n%d\n", search(a, sizeof(int), 0, num - 1, &item, compare));

  //
	start_bi = clock();
  printArray(b, num);
  qsort(b, num - 1, sizeof(int), compare);
  printArray(b, num);
	end_bi = clock();
  intervalBuiltIn = (double) (end_bi - start_bi) / CLOCKS_PER_SEC;
  printf("Index for item: \n%d\n", search(b, sizeof(int), 0, num - 1, &item, compare));
  
  //
	printf("3-way partition quicksort takes: %f seconds \n", intervalThreeWay);
	printf("built-in quicksort takes: %f seconds \n", intervalBuiltIn);

	free(a);
	free(b);
	
  return 0;
}