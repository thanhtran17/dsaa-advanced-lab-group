#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int int_compare(void  *a, void *b){
    int m = *((int*)a);
    int n = *((int*)b);
    if (m == n) return 0;
    return m > n ? 1 : -1;
}

void exch(void *buf, int size, int i, int j){
    char temp = *((char*)(buf+size*i));
    *((char*)(buf+size*i)) = *((char*)(buf+size*j));
    *((char*)(buf+size*j)) = temp;
}

void quicksort(void* a, int size, int l, int r, int (*compare)(void*, void*)) {
	if (r <= l) return;
	int i = l-1, j = r;
	int p = l-1, q = r;
	while(1){
		while (compare((char*)a+(++i)*size, (char*)a+r*size) < 0);
		while (compare((char*)a+r*size, (char*)a+(--j)*size) < 0)
            if (j == l) break;
		if (i >= j) break;
		exch(a, size, i, j);
		if (compare((char*)a+i*size, (char*)a+r*size)==0)
		   exch(a, size, ++p, i);
		if (compare((char*)a+j*size, (char*)a+r*size)==0)
		   exch(a, size, --q, j);
	}

	exch(a, size, i, r);
	j = i - 1;
	i = i + 1;
	for (int k = l ; k <= p; k++) exch(a, size, k, j--);
	for (int k = r-1; k >= q; k--) exch(a, size, k, i++);
	quicksort(a, size, l, j, compare);
	quicksort(a, size, i, r, compare);
}

int *createarr(int size){
    int *arr = (int*)malloc(size * sizeof(int));
    srand(time(NULL));
    for(int i=0; i < size; i++){
        *(arr+i) = rand()%10;
    }
    return arr;
}

void printarr(int a[], int n){
    for (int i = 0; i < n; i++) printf("%d\t", a[i]);
    printf("\n");
}

int main(){
    int *a;
    int n = 100;
    a = createarr(n);
    printarr(a, n);
    quicksort(a, sizeof(int), 0, n-1, int_compare);
    printarr(a, n);
}