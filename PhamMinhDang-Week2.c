#include <stdio.h>
#include <math.h>
#include "jval.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SMALL_NUMBER 20
#define HUGE_NUMBER 1000000
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void testJval()
{
    Jval a;
    a.i = 5;
    printf("%d", a.i);
}
int compare(void *x, void *y)
{
    int m = *((int *)x);
    int n = *((int *)y);
    return (m > n)
               ? 1
           : (m == n)
               ? 0
               : -1;
}
int compare_i(Jval *a, Jval *b)
{
    return ((*a).i > (*b).i)
               ? 1
           : ((*a).i == (*b).i)
               ? 0
               : -1;
}
void exch(void *a, void *b)
{
    int temp = *((int *)a);
    *((int *)a) = *((int *)b);
    *((int *)b) = temp;
}
void exch_jval(void *a, void *b)
{
    Jval temp = *((Jval *)a);
    *((Jval *)a) = *((Jval *)b);
    *((Jval *)b) = temp;
}
Jval *create_array_i(int size)
{
    Jval *arr = (Jval *)malloc(sizeof(Jval) * size);
    srand(time(NULL));
    for (int j = 0; j < size; j++)
    {
        arr[j].i = rand() % size;
    }
    return arr;
}
int search_gen(Jval arr[], int l, int r, int x)
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;
        if (arr[mid].i == x)
            return mid;
        if (arr[mid].i > x)
            return search_gen(arr, l, mid - 1, x);
        return search_gen(arr, mid + 1, r, x);
    }
    return -1;
}
void sort_gen(Jval x[], int l, int r, int (*compare)(Jval *, Jval *))
{
    int i, j, p, q, k;
    Jval pivot;
    i = l - 1;
    j = r;
    p = l - 1;
    q = r;
    pivot = x[r];
    if (r <= l)
        return;
    for (;;)
    {
        while (compare(&x[++i], &pivot) == -1)
            ;
        while (compare(&x[--j], &pivot) == 1)
        {
            if (j == l)
                break;
        }
        if (i >= j)
            break;
        exch_jval(&x[i], &x[j]);
        if (compare(&x[i], &pivot) == 0)
        {
            p++;
            exch_jval(&x[p], &x[i]);
        }
        if (compare(&x[j], &pivot) == 0)
        {
            q--;
            exch_jval(&x[q], &x[j]);
        }
    }
    exch_jval(&x[i], &x[r]);
    j = i - 1;
    i++;
    for (k = l; k < p; k++, j--)
        exch_jval(&x[k], &x[j]);
    for (k = r - 1; k > q; k--, i++)
        exch_jval(&x[i], &x[k]);
    sort_i(x, l, j, compare);
    sort_i(x, i, r, compare);
}
// 3-way partition based quick sort
void quicksort3Way(int x[], int l, int r, int (*compare)(void *, void *), void exch(void *, void *))
{
    int i, j, p, q, pivot, k;
    i = l - 1;
    j = r;
    p = l - 1;
    q = r;
    pivot = x[r];
    if (r <= l)
        return;
    for (;;)
    {
        while (compare(&x[++i], &pivot) == -1)
            ;
        while (compare(&x[--j], &pivot) == 1)
        {
            if (j == l)
                break;
        }
        if (i >= j)
            break;
        exch(&x[i], &x[j]);
        if (compare(&x[i], &pivot) == 0)
        {
            p++;
            exch(&x[p], &x[i]);
        }
        if (compare(&x[j], &pivot) == 0)
        {
            q--;
            exch(&x[q], &x[j]);
        }
    }
    exch(&x[i], &x[r]);
    j = i - 1;
    i++;
    for (k = l; k < p; k++, j--)
        exch(&x[k], &x[j]);
    for (k = r - 1; k > q; k--, i++)
        exch(&x[i], &x[k]);
    quicksort3Way(x, l, j, compare, exch);
    quicksort3Way(x, i, r, compare, exch);
}
void printSort(int arr[], int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
void printSortJval(Jval arr[], int length)
{
    for (int j = 0; j < length; j++)
    {
        printf("%d ", arr[j].i);
    }
    printf("\n");
}
// Heapsort: O(nlogn) for any cases
// inplace but not stable
void heapify(int arr[], int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest])
        largest = l;
    if (r < n && arr[r] > arr[largest])
        largest = r;
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}
void heap_sort(int arr[], int length)
{
    for (int i = length / 2 - 1; i >= 0; i--)
    {
        heapify(arr, length, i);
    }
    for (int i = length - 1; i > 0; i--)
    {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// Quick sort: O(nlogn)
// Worst case: O(n^2);
// in place but not stable
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int left = low;
    int right = high;
    while (1)
    {
        while (left <= right && arr[left] < pivot)
            left++;
        while (right >= left && arr[right] > pivot)
            right--;
        if (left >= right)
            break;
        swap(&arr[left], &arr[right]);
        left++;
        right--;
    }
    swap(&arr[left], &arr[high]);
    return left; // Trả về chỉ số sẽ dùng để chia đổi mảng
}
void quick_sort(int arr[], int low, int height)
{
    if (low < height)
    {
        int pi = partition(arr, low, height);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, height);
    }
}
// Merge sort: O(nlogn)
// stable but not in place
void merge(int arr[], int low, int mid, int height)
{
    // Declare variables
    int temp[10000];
    int first_1 = low;
    int first_2 = mid + 1;
    int end_1 = mid;
    int end_2 = height;
    int index = first_1;
    for (; (first_1 <= end_1) && (first_2 <= end_2); index++)
    {
        if (arr[first_1] < arr[first_2])
        {
            temp[index] = arr[first_1];
            first_1++;
        }
        else
        {
            temp[index] = arr[first_2];
            first_2++;
        }
    }
    for (; first_1 <= end_1; first_1++, index++)
    {
        temp[index] = arr[first_1];
    }
    for (; first_2 <= end_2; first_2++, index++)
    {
        temp[index] = arr[first_2];
    }
    for (index = low; index <= height; index++)
    {
        arr[index] = temp[index];
    }
}
void merge_sort(int arr[], int low, int height)
{
    if (low < height)
    {
        int mid = (low + height) / 2;
        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, height);
        merge(arr, low, mid, height);
    }
}

// O(n) for (best case (O(1): swap and O(n) comparisons))
// O(n^2) for worst case and average case
void bubble_sort(int arr[7], int length)
{
    for (int i = 1; i <= length - 1; i++)
    {
        for (int j = 0; j <= length - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// O(n^2) for every cases
// O(n^2) key comparisons but O(n) moves
// Inplace and stable sort
void selection_sort(int arr[7], int length)
{
    int min_pivot;
    // find smallest value
    for (int i = 0; i < length - 1; i++)
    {
        min_pivot = i;
        for (int j = i + 1; j < length; j++)
        {
            if (arr[min_pivot] > arr[j])
            {
                min_pivot = j;
            }
        }
        swap(&arr[i], &arr[min_pivot]);
    }
}
// Big O: O(n^2)
// Best case: O(n)
// Inplace and stable
void insertion_sort(int arr[7], int length)
{
    int key;
    int j;
    for (int i = 1; i < length; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
int *create_array(int size)
{
    int *arr = (int *)malloc(sizeof(int) * size);
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        *(arr + i) = rand() % size;
    }
    return arr;
}
int *dump_array(int *arr, int size)
{
    int *arr_1 = (int *)malloc(sizeof(int) * size);
    memcpy(arr_1, arr, size * sizeof(int));
    return arr_1;
}
int main()
{
    int *arr1;
    int *arr2;
    int n = 100;
    arr1 = create_array(n);
    // arr2 = dump_array(arr1, n);
    // time_t start;
    // time_t end;
    // start = clock();
    quicksort3Way(arr1, 0, n - 1, compare, exch); // sort 3 ways
    printSort(arr1, n);

    Jval *arr3 = create_array_i(n);
    sort_gen(arr3, 0, n - 1, compare_i);
    printSortJval(arr3, n);
    // end = clock();
    // printf("3 Ways run in %f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);
    // time_t start1;
    // time_t end1;
    // start1 = clock();
    // quick_sort(arr2, 0, n - 1); // sort 2 ways
    // end1 = clock();
    // printf("2 Ways run in %f seconds.\n", (double)(end1 - start1) / CLOCKS_PER_SEC);
}