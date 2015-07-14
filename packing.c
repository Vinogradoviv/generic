#include <stdio.h>
#include "packing.h"

const char* file_signature = "UPA";
int isSolid = 0;

int pack(unsigned short int fileCount, char** input_filename_arr, char* output_file_name, int pack_alg) {
    
    printf("packing files:\n");
    for(int i = 0; i < fileCount; i++) {
        printf("%s ", input_filename_arr[i]);
    }
    printf("\n");
    FILE* output_file = fopen(output_file_name, "wb");
    
    //Сигнатура
    fprintf(output_file, "%s", file_signature);
    
    //алгоритм сжатия
    void (*compress_function)(FILE*, FILE*);
    if(pack_alg == PACK_ALG_NOPE) {
        compress_function = nope_compress;
        fprintf(output_file, "NOPE");
    }
    if(pack_alg == PACK_ALG_HUFF) {
        //compress_function = huff_compress;
        printf("Huffman algorithm is unsupported\n");
        fclose(output_file);
        return 1;
    }
    
    //склеивать ли файлы
    fprintf(output_file, "%d", isSolid);
    
    //количество файлов
    fprintf(output_file, "%d", fileCount);
    
    //Упаковка файлов
    for(int i = 0; i < fileCount; i++) {
        FILE* input_file = fopen(input_filename_arr[i], "rb");
        compress_function(input_file, output_file);
    }
    
}

int unpack(char* filename) {
    printf("unpacking file: %s\n", filename);
}