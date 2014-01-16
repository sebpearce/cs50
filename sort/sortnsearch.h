#include <stdbool.h>

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

bool binary_search(int needle, int haystack[], int size)
{
    // define upper and lower bounds
    int upper = size - 1;
    int lower = 0;

    while (lower <= upper) {

        int mid = (upper + lower) / 2;

        if (needle == haystack[mid]) {
            return true;
        }

        if (needle < haystack[mid]) {
            upper = mid - 1;
        }

        if (needle > haystack[mid]) {
            lower = mid + 1;
        }
    }

    return false;
}

void bubble_sort(int list[], int size)
{
    int i, swaps;
    do {
        swaps = 0;
        for (int i = 1; i < size; i++) {
            if (list[i] < list[i-1]) {
                int temp = list[i-1];
                list[i-1] = list[i];
                list[i] = temp;
                swaps++;
            }
        }
        // uncomment this part to show what's happening
        // for (int i = 0; i < size; i++) {
        //     printf("%2d ", list[i]);
        // }
        // printf("\n");
    } while (swaps > 0);
}

void selection_sort(int list[], int size)
{
    int i = 0;
    int min = 0;
    for (i = 0; i < size-1; i++) {

        // start by setting min = i
        min = i;

        // compare min to all other elements in unsorted subarray
        for (int j = i+1; j < size; j++) {
             // if you find an even smaller element, that is now min
            if (list[j] < list[min]) {
                min = j;
            }
        }
        // swap to put min in correct position in sorted subarray
        int temp = list[i];
        list[i] = list[min];
        list[min] = temp;

        // print array to see what's happening
        // for (int q = 0; q < size; q++) {
        //     printf("%2d ", list[q]);
        // }
        // printf(" (i = %d, min = %d)\n", i, min);
    }
}

void insertion_sort(int list[], int size)
{
    int i = 0;
    int j = 0;
    int el = 0;
    for (i = 0; i < size; i++) {

        el = list[i];
        int j = i;
        while (j > 0 && list[j-1] > el) {
            list[j] = list[j-1];
            j--;
        }
        list[j] = el;
        // print lines to show what's happening
        // for (int q = 0; q < size; q++) {
        //     printf("%2d ", list[q]);
        // }
        // printf("\n");
    }
}

void merge(int array[], int* temp, int start_1, int end_1, int start_2, 
    int end_2)
{
    // Merge sorted subarrays using the auxiliary array 'temp'

    // print to see what's going on
    // printf("Merging %d to %d...", start_1, end_1);
    // printf("and %d to %d", start_2, end_2);
    // if (start_1 == end_1 && start_2 == end_2) {
    //     printf(" (%d and %d)", array[start_1], array[start_2]);
    // }
    // printf("\n");
    int i, j, x;
    i = j = x = 0;
    // While there are elements in both subarrays
    while ((start_1+i) <= end_1 && (start_2+j) <= end_2) {
        // Compare numbers at the start of the subarrays
        if (array[start_1+i] < array[start_2+j]) {
            // Append smaller to merged array
            temp[x] = array[start_1+i];
            x++, i++;
        }
        else {
            temp[x] = array[start_2+j];
            x++, j++;
        }
    }

    // While elements remain in subarray 1 (but not subarray 2)
    while ((start_1+i) <= end_1) {
        // Append element to merged array
        temp[x] = array[start_1+i];
        x++, i++;
    }

    // While elements remain in subarray 2 (but not subarray 1)
    while ((start_2+j) <= end_2) {
        // Append element to merged array
        temp[x] = array[start_2+j]; // the offender
        x++, j++;
    }

    x = 0;
    // Copy values from temp back into array, from start_1 to end_2
    for (int r = start_1; r <= end_2; r++) {
        array[r] = temp[x++];
    }
}

void msort(int array[], int* temp, int start, int end)
{
    if (end > start)
    {
        int middle = (start + end) / 2;

        // sort left half
        msort(array, temp, start, middle);

        // sort right half
        msort(array, temp, middle + 1, end);

        // merge the two halves
        merge(array, temp, start, middle, middle + 1, end);
    }
}

void merge_sort(int list[], int size)
{
    int* temp = malloc(sizeof(int) * size);
    int i = 0;
    for (i = 0; i < size; i++) {
        temp[i] = 0;
    }

    msort(list, temp, 0, size-1);

    free(temp);
}

