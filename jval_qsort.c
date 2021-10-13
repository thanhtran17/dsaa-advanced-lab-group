#include "jval.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Jval *crear_arr(int n){
    Jval *arr = (Jval*)malloc(n * sizeof(Jval));
    srand(time(NULL));
    for (int i = 0; i<n; i++){
        *(arr+i) = new_jval_i(rand()%100);
    return arr;
    }
}

int int_compare(Jval*a, Jval*b) {
    if ( jval_i(*a) == jval_i(*b)) return 0;
	if ( jval_i(*a) < jval_i(*b)) return -1;    
	else return 1;
}

void swap(void *a, void *b){
    Jval temp = *((Jval*)a);
    *((Jval*)b) = *((Jval*)b);
    *((Jval*)b) = temp;
}

void quicksort(Jval *a, int l, int r, int (*compare)(Jval*, Jval*)) {
	if (r <= l) return;
	int i = l-1, j = r;
	int p = l-1, q = r;
	while(1){
		while (compare(&a[++i], &a[r]) < 0);
		while (compare(&a[r], &a[--j]) < 0)
            if (j == l) break;
		if (i >= j) break;
		swap(&a[i], &a[j]);
		if (compare(&a[i], &a[r])==0)
		   swap(&a[++p], &a[i]);
		if (compare(&a[j], &a[r])==0)
		   swap(&a[--q], &a[j]);
	}

	swap(&a[i], &a[r]);
	j = i - 1;
	i = i + 1;
	for (int k = l ; k <= p; k++) swap(&a[k], &a[--j]);
	for (int k = r-1; k >= q; k--) swap(&a[k], &a[++i]);
	quicksort(a, l, j, compare);
	quicksort(a, i, r, compare);
}

void printarr(Jval a[], int n){
    for (int i = 0; i<n; i++){
        printf("%d\t", a[i].i);
    }
    printf("\n");
}
int main(){
    Jval *a;
    int n = 100;
    a = createarr(n);
    //printarr(a, n);
    quicksort(a, 0, n-1, int_compare);
    printarr(a, n);
    free(a);
}