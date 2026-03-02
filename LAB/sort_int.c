#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int compare(const void* a, const void* b) {
    int32_t x = *(int32_t*)a;
    int32_t y = *(int32_t*)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

void bubble_sort(int32_t* array, size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int32_t temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void save_to_file(const char* filename, int32_t* array, size_t size) {
    FILE* fileout = fopen(filename, "w");
    if (fileout == NULL) {
        perror("Error opening output file");
        exit(1);
    }
    for (size_t i = 0; i < size; i++) {
        fprintf(fileout, "%d\n", array[i]);
    }
    fclose(fileout);
}

int32_t* get_array_from_file(const char* filename, size_t* out_size) {
    FILE* filein = fopen(filename, "r");
    if (filein == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    size_t capacity = 100;
    size_t size = 0;
    int32_t* array = malloc(capacity * sizeof(int32_t));

    int32_t num;
    while (fscanf(filein, "%d", &num) == 1) {
        if (size >= capacity) {
            capacity += 100;
            array = realloc(array, capacity * sizeof(int32_t));
        }
        array[size++] = num;
    }

    fclose(filein);
    *out_size = size;
    return array;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filein.txt> <fileout.txt>\n", argv[0]);
        exit(1);
    }

    size_t size = 0;
    int32_t* array = get_array_from_file(argv[1], &size);

    //qsort(array, size, sizeof(int32_t), compare);
    bubble_sort(array, size);
    
    save_to_file(argv[2], array, size);

    free(array);
    return 0;
}