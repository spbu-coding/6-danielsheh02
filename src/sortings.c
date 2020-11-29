#include "sortings.h"
#include "strings_comparer.h"

void mergeSort2(strings_array_t array_of_strings, int l, int m, int r, comparator_func_t comparator) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    char *L[n1], *R[n2];
    for (i = 0; i < n1; i++) {
        L[i] = array_of_strings[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = array_of_strings[m + 1 + j];
    }
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (comparator(L[i], R[j]) < 0) {
            array_of_strings[k] = L[i];
            i++;
        } else {
            array_of_strings[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        array_of_strings[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        array_of_strings[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(strings_array_t array_of_strings, int l, int r, comparator_func_t comparator) {
    if (l >= r) {
        return;
    }
    int m = (l + r - 1) / 2;
    mergeSort(array_of_strings, l, m, comparator);
    mergeSort(array_of_strings, m + 1, r, comparator);
    mergeSort2(array_of_strings, l, m, r, comparator);
}

void merge(strings_array_t array_of_strings, array_size_t number_of_lines, comparator_func_t comparator) {
    mergeSort(array_of_strings, 0, (int) number_of_lines - 1, comparator);
}

int partition_quick(strings_array_t array_of_strings, int low, int high, comparator_func_t comparator) {
    char *pivot = array_of_strings[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (comparator(array_of_strings[j], pivot) < 0) {
            i++;
            swap(&array_of_strings[i], &array_of_strings[j]);
        }
    }
    swap(&array_of_strings[i + 1], &array_of_strings[high]);
    return (i + 1);
}

void quickSort(strings_array_t array_of_strings, int low, int high, comparator_func_t comparator) {
    if (low < high) {
        int pi = partition_quick(array_of_strings, low, high, comparator);
        quickSort(array_of_strings, low, pi - 1, comparator);
        quickSort(array_of_strings, pi + 1, high, comparator);
    }
}

void quick(strings_array_t array_of_strings, array_size_t number_of_lines, comparator_func_t comparator) {
    quickSort(array_of_strings, 0, (int) number_of_lines - 1, comparator);
}

void insertion(strings_array_t array_of_strings, array_size_t number_of_lines, comparator_func_t comparator) {
    int i, j;
    char *key;
    for (i = 1; i < number_of_lines; i++) {
        key = array_of_strings[i];
        j = i - 1;
        while (j >= 0 && (comparator(array_of_strings[j], key) > 0)) {
            array_of_strings[j + 1] = array_of_strings[j];
            j = j - 1;
        }
        array_of_strings[j + 1] = key;
    }
}

void bubble(strings_array_t array_of_strings, size_t number_of_lines, comparator_func_t comparator) {
    for (int i = 0; i < number_of_lines - 1; i++) {
        for (int j = 0; j < number_of_lines - i - 1; j++) {
            if (comparator(array_of_strings[j], array_of_strings[j + 1]) > 0) {
                swap(&array_of_strings[j], &array_of_strings[j + 1]);
            }
        }
    }
}

unsigned long getMax_radix(strings_array_t array_of_strings, size_t number_of_lines) {
    unsigned long max_string = strlen(array_of_strings[0]);
    for (int i = 1; i < number_of_lines; i++) {
        if (strlen(array_of_strings[i]) > max_string) {
            max_string = strlen(array_of_strings[i]);
        }
    }
    return max_string;
}

void countSort_radix(strings_array_t array_of_strings, size_t number_of_lines, int column_number,
                     comparator_func_t comparator) {
    char *output[number_of_lines];
    int i, count[NUMBER_OF_SYMBOLS_IN_CHARSET] = {0};

    for (i = 0; i < number_of_lines; i++) {
        count[(array_of_strings[i][column_number])]++;
    }

    for (i = 1; i < NUMBER_OF_SYMBOLS_IN_CHARSET; i++) {
        count[i] += count[i - 1];
    }

    for (i = number_of_lines - 1; i >= 0; i--) {
        output[count[array_of_strings[i][column_number]] - 1] = array_of_strings[i];
        count[array_of_strings[i][column_number]]--;
    }

    for (i = 0; i < number_of_lines; i++) {
        if (column_number > 0 || comparator == compare_asc) {
            array_of_strings[i] = output[i];
        } else {
            array_of_strings[i] = output[number_of_lines - i - 1];
        }
    }
}

void radix(strings_array_t array_of_strings, size_t number_of_lines, comparator_func_t comparator) {
    unsigned long max_string = getMax_radix(array_of_strings, number_of_lines);
    for (int column_number = max_string - 1; column_number >= 0; column_number--) {
        countSort_radix(array_of_strings, number_of_lines, column_number, comparator);
    }
}