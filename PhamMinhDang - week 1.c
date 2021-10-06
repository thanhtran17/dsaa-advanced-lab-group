#include <stdio.h>
#include <math.h>
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

// 3-way partition based quick sort
void quicksort3Way(int x[], int l, int r)
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
        while (x[++i] < pivot)
            ;
        while (x[--j] > pivot)
        {
            if (j == l)
                break;
        }
        if (i >= j)
            break;
        swap(&x[i], &x[j]);
        if (x[i] == pivot)
        {
            p++;
            swap(&x[p], &x[i]);
        }
        if (x[j] == pivot)
        {
            q--;
            swap(&x[q], &x[j]);
        }
    }
    swap(&x[i], &x[r]);
    j = i - 1;
    i++;
    for (k = l; k < p; k++, j--)
        swap(&x[k], &x[j]);
    for (k = r - 1; k > q; k--, i++)
        swap(&x[i], &x[k]);
    quicksort3Way(x, l, j);
    quicksort3Way(x, i, r);
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
        *(arr + i) = rand() % 10;
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
    int n = 10000000;
    arr1 = create_array(n);
    arr2 = dump_array(arr1, n);
    time_t start;
    time_t end;
    start = clock();
    quicksort3Way(arr1, 0, n - 1); // sort 3 ways
    end = clock();
    printf("3 Ways run in %f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);
    time_t start1;
    time_t end1;
    start1 = clock();
    quick_sort(arr2, 0, n - 1); // sort 2 ways
    end1 = clock();
    printf("2 Ways run in %f seconds.\n", (double)(end1 - start1) / CLOCKS_PER_SEC);
}