#include <stdio.h>
#include <cstring>
#include <cstdlib>

unsigned long long nope_compress(FILE* input_file, FILE* output_file) {
    unsigned long long packed_data_size = 0;
    while(1) {
        char c = getc(input_file);
        if(c >= 0) {
            fprintf(output_file, "%c", c);
            packed_data_size++;
        }
        else
            break;
    }
    return packed_data_size;
}

void nope_decompress(FILE* input_file, FILE* output_file, unsigned long long stream_size) {
    unsigned long long offset = 0;
    while(offset < stream_size) {
        unsigned char c = getc(input_file);
        fprintf(output_file, "%c", c);
        offset++;
    }
    fclose(output_file);
}