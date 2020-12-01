#include "sortings.h"
#include "strings_comparer.h"

void free_of_array(strings_array_t array_of_strings, array_size_t number_of_lines) {
    for (int i = 0; i < (int) number_of_lines; i++) {
        free(array_of_strings[i]);
    }
    free(array_of_strings);
}

strings_array_t allocate_memory(int entered_number_of_lines) {
    strings_array_t array_of_strings = calloc(sizeof(char *), entered_number_of_lines);
    if (!array_of_strings) {
        fprintf(stdout, "Cannot allocate memory for results string in all experiment\n");
        return NULL;
    }
    for (int i = 0; i < entered_number_of_lines; i++) {
        array_of_strings[i] = calloc(sizeof(char), MAX_INPUT_STRING_SIZE);
        if (!array_of_strings[i]) {
            for (int j = 0; j < i; j++) {
                free(array_of_strings[i]);
            }
            free(array_of_strings);
            return NULL;
        }
    }
    return array_of_strings;
}

void swap(char **first_str, char **second_str) {
    char *tmp = *second_str;
    *second_str = *first_str;
    *first_str = tmp;
}

int compare_des(const char *string1, const char *string2) {
    unsigned string_length;
    if (strlen(string1) > strlen(string2)) {
        string_length = strlen(string2);
    } else {
        string_length = strlen(string1);
    }
    for (int i = 0; i < (int) string_length; i++) {
        if (string1[i] > string2[i]) {
            return -1;
        }
        if (string1[i] < string2[i]) {
            return 1;
        }
    }
    return 0;
}

int compare_asc(const char *string1, const char *string2) {
    unsigned string_length;
    if (strlen(string1) > strlen(string2)) {
        string_length = strlen(string2);
    } else {
        string_length = strlen(string1);
    }
    for (int i = 0; i < (int) string_length; i++) {
        if (string1[i] < string2[i]) {
            return -1;
        }
        if (string1[i] > string2[i]) {
            return 1;
        }
    }
    return 0;
}

size_t reading_file(char *name_input, strings_array_t array_of_strings, array_size_t number_of_lines) {
    FILE *file;
    if ((file = fopen(name_input, "rb")) == NULL) {
        fprintf(stdout, "Cannot open file. No file with name %s exists.", name_input);
        return 0;
    }
    char symbol;
    int count = 0;
    size_t actual_number_of_lines = 0;
    for (int i = 0; i < (int) number_of_lines && !feof(file); i++) {
        actual_number_of_lines++;
        for (int j = 0; j < MAX_INPUT_STRING_SIZE + 2; j++) {
            if ((symbol = (char) fgetc(file)) != EOF) {
                array_of_strings[i][j] = symbol;
                count = i;
                if (symbol == '\n') {
                    break;
                }
            } else {
                if (count == i) {
                    array_of_strings[i][j] = '\r';
                    array_of_strings[i][j + 1] = '\n';
                } else {
                    actual_number_of_lines = count;
                }
                break;
            }
        }
    }
    fclose(file);
    return actual_number_of_lines;
}

int writing_file(char *name_output, strings_array_t array_of_strings, array_size_t number_of_lines) {
    FILE *new_file;
    if ((new_file = fopen(name_output, "wb")) == NULL) {
        fprintf(stdout, "Failed to create file");
        return -1;
    }
    for (int i = 0; i < (int) number_of_lines; i++) {
        fputs(array_of_strings[i], new_file);
    }
    fclose(new_file);
    return 0;
}

int main(int argc, char **argv) {
    comparator_func_t comparator;
    array_size_t entered_number_of_lines;
    array_size_t actual_number_of_lines;
    int bubble_count = 0;
    int insertion_count = 0;
    int merge_count = 0;
    int quick_count = 0;
    int radix_count = 0;
    int asc_count = 0;
    int des_count = 0;
    if (argc != 6) {
        fprintf(stdout,
                "Use %s, number of lines, <input file>.txt, <output file>.txt, algorithm name, comparator name\n",
                argv[0]);
        return -1;
    }
    entered_number_of_lines = strtol(argv[1], NULL, 10);
    if (entered_number_of_lines == 0) {
        fprintf(stdout, "The number of compared strings cannot be 0, perhaps you entered NOT NUMBER.\n");
        return -1;
    }
    if (strncmp(argv[4], "bubble", BUBBLE_SORT_LENGTH) == 0) {
        bubble_count += 1;
    } else if (strncmp(argv[4], "insertion", INSERTION_SORT_LENGTH) == 0) {
        insertion_count += 1;
    } else if (strncmp(argv[4], "merge", MERGE_SORT_LENGTH) == 0) {
        merge_count += 1;
    } else if (strncmp(argv[4], "quick", QUICK_SORT_LENGTH) == 0) {
        quick_count += 1;
    } else if (strncmp(argv[4], "radix", RADIX_SORT_LENGTH) == 0) {
        radix_count += 1;
    }
    if (bubble_count == 0 && insertion_count == 0 && merge_count == 0 && quick_count == 0 && radix_count == 0) {
        fprintf(stdout, "Available algorithms: bubble, insertion, merge, quick, radix. \n");
        return -1;
    }
    if (strncmp(argv[5], "asc", ASC_LENGTH) == 0) {
        asc_count += 1;
    } else if (strncmp(argv[5], "des", DES_LENGTH) == 0) {
        des_count += 1;
    }
    if (asc_count == 0 && des_count == 0) {
        fprintf(stdout, "Available comparators: asc, des. \n");
        return -1;
    }
    if (asc_count == 1) {
        comparator = compare_asc;
    } else {
        comparator = compare_des;
    }
    strings_array_t array_of_strings = NULL;
    if (!(array_of_strings = allocate_memory(entered_number_of_lines))) {
        return -1;
    }
    if ((actual_number_of_lines = reading_file(argv[2], array_of_strings, entered_number_of_lines)) == 0) {
        free_of_array(array_of_strings, entered_number_of_lines);
        return -1;
    }
    if (actual_number_of_lines < entered_number_of_lines) {
        fprintf(stdout, "The actual number of lines is less than what was entered.\n");
        free_of_array(array_of_strings, entered_number_of_lines);
        return -1;
    }
    if (bubble_count == 1) {
        bubble(array_of_strings, actual_number_of_lines, comparator);
    } else if (insertion_count == 1) {
        insertion(array_of_strings, actual_number_of_lines, comparator);
    } else if (merge_count == 1) {
        merge(array_of_strings, actual_number_of_lines, comparator);
    } else if (quick_count == 1) {
        quick(array_of_strings, actual_number_of_lines, comparator);
    } else if (radix_count == 1) {
        radix(array_of_strings, actual_number_of_lines, comparator);
    }
    if (writing_file(argv[3], array_of_strings, actual_number_of_lines)) {
        free_of_array(array_of_strings, entered_number_of_lines);
        return -1;
    }
    free_of_array(array_of_strings, entered_number_of_lines);
    return 0;
}