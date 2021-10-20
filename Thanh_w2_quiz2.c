#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jval.h"
#include "jval.c"
//---------------------------------------------------------------------------------
void* memcpy(void* region1,const void* region2,size_t n) {
	const char* first = (const char*) region2;
	const char* last = ((const char*) region2) + n;
	char* result = (char*) region1;
	while (first != last) *result++ = *first++;
	return result;
}
//---------------------------------------------------------------------------------
void swap(Jval buf[],int i,int j){
    Jval cmp = buf[i];
    buf[i] = buf[j];
    buf[j] = cmp;
}
//---------------------------------------------------------------------------------
int int_compare(Jval *a,Jval *b){
	Jval *x,*y;
	x=a;
	y=b;
	if(x->i==y->i)return 0;
	return x->i>y->i?1:-1;
}
//---------------------------------------------------------------------------------
void sort_gen(Jval arr[], int l, int r,int (*compare)(Jval*, Jval*),void (*swap)(Jval*,int,int)) {
	if (r <= l) return;
	int i = l-1, j = r;
	int p = l-1, q = r;
	int k;
	while(1)	{
		while (compare(&arr[++i],&arr[r])<0);
		while (compare(&arr[r],&arr[--j])<0) if (j == l) break;		
		if (i >= j) break;
		swap(arr,i,j);
		if (compare(&arr[i],&arr[r])==0) swap(arr,++p,i);
		if (compare(&arr[j],&arr[r])==0) swap(arr,--q,j);
	}
	swap(arr,i,r);
	j = i - 1;
	i = i + 1;		
	for (k = l ; k <= p; k++) swap(arr,k,j--);
	for (k = r-1; k >= q; k--) swap(arr,k,i++);	
	sort_gen(arr,l, j,compare,swap);
	sort_gen(arr,i, r,compare,swap);
}

//---------------------------------------------------------------------------------
int search_gen(Jval a[],int l, int r,Jval item,int (*compare)(Jval*, Jval*)) {
	int i, res;
	if (r < l) return -1;
	i = (l + r)/2;
	res = compare(&item,&a[i]);
	if (res==0) return i;
	else if (res < 0) return search_gen(a, l, i-1, item, compare);
	else return search_gen(a, i+1, r, item, compare);
}

//---------------------------------------------------------------------------------
int compare_i(Jval* a, Jval* b) {
	if ( jval_i(*a)==jval_i(*b) ) return 0;
	if ( jval_i(*a) < jval_i(*b) ) return -1;
	else return 1;
}

//---------------------------------------------------------------------------------
void sort_i (Jval a[], int l, int r) {
	sort_gen(a, l, r, compare_i,swap);
}

//---------------------------------------------------------------------------------
int search_i (Jval a[], int l, int r, int x) {
	return search_gen(a, l, r, new_jval_i(x), compare_i);
}

//---------------------------------------------------------------------------------
Jval* create_array_i (int n) {
	int i;
	Jval * array = (Jval *) malloc(sizeof(Jval)*n);
	for (i=0; i<n; i++) array[i] = new_jval_i( rand() );
	return array;
}

//---------------------------------------------------------------------------------
int main()
{	
	int i;
    int n=10,item=5,res;

   	Jval *b;
	b=create_array_i(n);
	sort_i(b,0,n-1);
	for(i=0;i<n;i++) printf("%d ",b[i].i);
	
    return 0;
}
//---------------------------------------------------------------------------------
