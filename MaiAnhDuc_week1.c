#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void printarr(int a[], int n){
	for (int i = 0; i < n; i++){
		printf("%d\t", a[i]);
	}
    printf("\n");
}

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition_2way(int arr[], int low, int high){
    int pivot = arr[high];
    int i = low;
    int j = high - 1;
    while(1){
        while(i <= j && arr[i] < pivot) i++;
        while(i <= j && arr[j] > pivot) j--;
        if (i >= j) break;
        swap(&arr[i], &arr[j]);
        i++;
        j--;
    }
    swap(&arr[i], &arr[high]);
    return i;
}

void quicksort(int a[], int low, int high){
	if (low < high){
		int mid = partition_2way(a, low, high);
		quicksort(a, low, mid-1);
		quicksort(a, mid+1, high);
	}
}


void partition3way (int a[], int l, int r){
    if (r <= l) return;
    int i = l-1; int j = r;
    int p = l-1; int q = r;
    while(1){
        while (a[++i] < a[r]);
        while (a[r] < a[--j]) {
            if (j == l) break;
        }
        if (i >= j) break;
        swap(&a[i],&a[j]);
		if (a[i]==a[r]) swap(&a[++p],&a[i]);
		if (a[j]==a[r]) swap(&a[--q],&a[j]);
    }

    swap(&a[i], &a[r]);
    j = i - 1;
    i = i + 1;
    for (int k = l; k <= p; k++) swap(&a[k], &a[--j]);
    for (int k = r-1; k >= q; k--) swap(&a[k], &a[++i]);
    partition3way(a, l, j);
    partition3way(a, i, r); 
}


int *createarr(int size){
    int *arr = (int*)malloc(size * sizeof(int));
    srand(time(NULL));
    for(int i=0; i < size; i++){
        *(arr+i) = rand()%10;
    }
    return arr;
}

int *dumparr(int *a, int size){
    int *arr = (int*)malloc(size * sizeof(int));
    memcpy(a, arr, size*sizeof(int));
    return arr;
}

int main(){
    int n = 10000000;
    int *a1;
    int *a2;
    a1 = createarr(n);
    a2 = dumparr(a1, n);

    double start1 = clock();
    quicksort(a1, 0, n-1);
    double end1 = clock();
    double time_2way = (double)(end1 - start1) / CLOCKS_PER_SEC;

    double s2 = clock();
    partition3way(a2, 0, n-1);
    double e2 = clock();
    double time_3way = (double)(e2-s2) / CLOCKS_PER_SEC;
    printf("Time of 2 way: %f seconds\n", time_2way);
    printf("Time of 3 way: %f seconds", time_3way);

}
