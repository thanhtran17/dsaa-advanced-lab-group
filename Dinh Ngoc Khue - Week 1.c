#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//// swap
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//// 2 way
int partition (int arr[], int l, int r)
{
    int pivot = arr[r];
    int i = l;
    int j = r - 1;
    while(1){
        while(i <= j && arr[i] < pivot) i++;
        while(i <= j && arr[j] > pivot) j--;
        if (i >= j) break;
        swap(&arr[i], &arr[j]);
        i++;
        j--;
    }
    swap(&arr[i], &arr[r]);
    return i;
}
 
void quickSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int pi = partition(arr, l, r);
        quickSort(arr, l, pi - 1);
        quickSort(arr, pi + 1, r);
    }
}

//// 3 way
void sort(int a[], int l, int r) {
	if (r <= l) return;
	int i = l-1, j = r;
	int p = l-1, q = r;
	int k;
	while(1)	{
		while (a[++i] < a[r]);
		while (a[r] < a[--j]) if (j == l) break;
		if (i >= j) break;
		swap(&a[i],&a[j]);
		if (a[i]==a[r]) swap(&a[++p],&a[i]);
		if (a[j]==a[r]) swap(&a[--q],&a[j]);
	}
	swap(&a[i],&a[r]);
	j = i - 1;
	i = i + 1;	
	
	for (k = l ; k <= p; k++) swap(&a[k],&a[j--]);		
	for (k = r-1; k >= q; k--) swap(&a[k],&a[i++]);
	
	sort(a, l, j);
	sort(a, i, r);
}

//// create array
int *createArray(int n){
	int i;
	int *a=(int*)malloc(n*sizeof(int));
	srand(time(NULL));
	for (i=0;i<n;i++) *(a+i)=rand()%10;
	return a;
}

//// dumb array
int *dumbArray(int *a,int n){
	int *b=(int*)malloc(n*sizeof(int));
	memcpy(b,a,n*sizeof(int));
	return b;
}

int int_compare(void const* x,void const* y){
	int m,n;
	m=*((int*)x);
	n=*((int*)y);
	if(m==n)return 0;
	return m>n?1:-1;
}
//// main
int main()
{	
    int *a,*b,*c,i;
    int n=10000000;

    a=createArray(n);
    b=dumbArray(a,n);
    c=dumbArray(a,n);
   	//// sort by 2 way qs ////
    double start1=clock();
    quickSort(a, 0, n-1);
    double end1=clock();
    double time_use1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    
    //// sort by 3 way qs ////
   	double start2=clock();
    sort(b, 0, n-1);
    double end2=clock();
    double time_use2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    
    //// sort by built-in qs
   	double start3=clock();
    qsort(c, n-1,sizeof(int),int_compare);
    double end3=clock();
	double time_use3 = (double)(end3 - start3) / CLOCKS_PER_SEC;
	
	//// print
	printf("Time use by kinds of Quicksort\n");
	printf("2 ways:%f sec\n3 ways:%f sec\nBuilt-in: %f sec",time_use1,time_use2,time_use3);
	
	//// free
    free(a);
    free(b);
    free(c);
    return 0;
}
