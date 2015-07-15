#include <stdio.h>

void nope_compress(FILE* input_file, FILE* output_file) {
    while(1) {
        char c = getc(input_file);
        if(c >= 0)
            fprintf(output_file, "%c", c);
        else
            break;
    }
    
}