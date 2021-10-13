#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#define num 100
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
           int (*compare) (void*, void*)
          )
{
  int i, res;
  if (l <= r) {
    i = (l + r) / 2;
    res = int_compare(item, (char*) buf + (size * i));
    if (res == 0)
      return i;
    else if (res < 0)
      return search(buf, size, l, i - 1, item, compare);
    else 
      return search(buf, size, i + 1, r, item, compare);
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
void exch(void *a, void *b)
{
    int temp = *((int *)a);
    *((int *)a) = *((int *)b);
    *((int *)b) = temp;
}
//----------------------------------------------------------------------------
int main()
{
  int i, res, item = 11;

  int a[num];
  srand(time(NULL));
  for (i = 0; i < num; i++)
    a[i] = rand() % num;
  
  qsort(a, num, sizeof(int), compare);
  printArray(a, num);
  res = search(a, sizeof(int), 0, num - 1, &item, compare);
  printf("Index for item: \n%d\n", res);
  return 0;
}
//----------------------------------------------------------------------------

