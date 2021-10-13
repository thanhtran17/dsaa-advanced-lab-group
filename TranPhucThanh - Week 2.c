#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
//----------------------------------------------------------------------------
int compare(void *x, void *y)
{
	int m = *((int*) x),
			n = *((int*) y);
	if (m == n) 
		return 0;
	
	return m > n ? 1 : -1;
}
//----------------------------------------------------------------------------
int search(
           void* buf, 
           int size, 
           int l, int r, 
           void *item, 
           int (*int_compare) (void*, void*)
          )
{
  int i, res;
  if (l <= r) {
    i = (l + r) / 2;
    res = int_compare(item, (char*) buf + (size * i));
    if (res == 0)
      return i;
    else if (res < 0)
      return search(buf, size, l, i - 1, item, int_compare);
    else 
      return search(buf, size, i + 1, r, item, int_compare);
  }
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
int main()
{
  int i, res, item = 11;
  // int n = 7;
  // int a[7] = {10, 80, 30, 90, 40, 50, 70};

  int n = 100;
  int a[100];
  srand(time(NULL));
  for (i = 0; i < n; i++)
    a[i] = rand() % n;
  
  qsort(a, n, sizeof(int), compare);
  printArray(a, n);
  res = search(a, sizeof(int), 0, n - 1, &item, compare);
  printf("Index for item: \n%d\n", res);
  return 0;
}
//----------------------------------------------------------------------------

