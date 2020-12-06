#include <stdio.h>
#include <stdlib.h>

#define BUBBLE_SORT_LENGTH 6
#define INSERTION_SORT_LENGTH 9
#define MERGE_SORT_LENGTH 5
#define QUICK_SORT_LENGTH 5
#define RADIX_SORT_LENGTH 5
#define ASC_LENGTH 3
#define DES_LENGTH 3
#define NUMBER_OF_SYMBOLS_IN_CHARSET 128

void swap(char **first_str, char **second_str);

int compare_asc(const char *string1, const char *string2);

int compare_des(const char *string1, const char *string2);