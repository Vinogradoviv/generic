#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include "packing.h"
#include "utils.h"

const char* file_signature = "UPA";
int isSolid = 0;

int pack(unsigned char fileCount, char** input_filename_arr, char* output_file_name, int pack_alg) {
    
    printf("packing files:\n");
    for(int i = 0; i < fileCount; i++) {
        printf("%s ", input_filename_arr[i]);
    }
    printf("\n");
    FILE* output_file = fopen(output_file_name, "wb");
    
    //Сигнатура
    fprintf(output_file, "%s", file_signature);
    
    //алгоритм сжатия
    unsigned long long (*compress_function)(FILE*, FILE*);
    if(pack_alg == PACK_ALG_NOPE) {
        compress_function = nope_compress;
        fprintf(output_file, "NOPE");
    }
    if(pack_alg == PACK_ALG_HUFF) {
        compress_function = huff_compress;
        fprintf(output_file, "HUFF");
    }
    
    //склеивать ли файлы
    fprintf(output_file, "%d", isSolid);
    
    //количество файлов
    fprintf(output_file, "%c", fileCount);
    
    //Упаковка файлов
    for(int i = 0; i < fileCount; i++) {
        
        FILE* input_file = fopen(input_filename_arr[i], "rb");
        
        //Имя файла
        unsigned char filename_length = strlen(input_filename_arr[i]);
        fprintf(output_file, "%c", filename_length);
        fprintf(output_file, "%s", input_filename_arr[i]);
        printf("\nFilename: %s\n", input_filename_arr[i]);
        
        //Размер сжатых данных
        int packed_data_size_offset = ftell(output_file);
        unsigned long long packed_data_size = 0;
        writeLongLongInt(output_file, packed_data_size);
        //fprintf(output_file, "%llu", packed_data_size);
        
        //Размер исходных данных
        int original_data_size_offset = ftell(output_file);
        fseek(input_file, 0, SEEK_END);
        unsigned long long original_data_size = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);
        printf("Original data size: %llu\n", original_data_size);
        writeLongLongInt(output_file, original_data_size);
        
        //Атрибуты файла
        unsigned char attributes = 0;
        fprintf(output_file, "%c", attributes);
        
        //Дата
        int filedate = -1;
        writeInt(output_file, filedate);
        fclose(input_file);
        
        //Сжатие файла
        input_file = fopen(input_filename_arr[i], "rb");
        packed_data_size = compress_function(input_file, output_file);
        unsigned long long packed_stream_end_offset = ftell(output_file);
        fseek(output_file, packed_data_size_offset, SEEK_SET);
        writeLongLongInt(output_file, packed_data_size);
        fseek(output_file, original_data_size_offset, SEEK_SET);
        writeLongLongInt(output_file, original_data_size);
        printf("Packed data size: %llu\n", packed_data_size);
        fclose(input_file);
        fseek(output_file, packed_stream_end_offset, SEEK_SET);
    }
    
    fclose(output_file);
    
}

int unpack(char* filename) {
    printf("\nunpacking file: %s\n", filename);
    
    FILE* input_file = fopen(filename, "rb");
    fseek(input_file, 3, SEEK_SET);
    
    void (*decompress_function)(FILE*, FILE*, unsigned long long stream_size);
    char* alg_str = (char*)malloc(4*sizeof(char));
    for(int i = 0; i < 4; i++) {
        alg_str[i] = getc(input_file);
    }
    if(strcmp(alg_str, "NOPE") == 0)
        decompress_function = nope_decompress;
    if(strcmp(alg_str, "HUFF") == 0)
        decompress_function = huff_decompress;
    
    fseek(input_file, 1, SEEK_CUR);
    unsigned short int file_count = getc(input_file);
    printf("File count: %d\n", file_count);
    
    for(int i = 0; i < file_count; i++) {
    //for(int i = 0; i < 1; i++) {
        int file_name_length = getc(input_file);
        printf("\nFilename length: %d\n", file_name_length);
        char* output_file_name = (char*)malloc(file_name_length*sizeof(char));
        for(int i = 0; i < file_name_length; i++) {
            output_file_name[i] = getc(input_file);
        }
        printf("Filename: %s\n", output_file_name);
        FILE* output_file = fopen(strcat(output_file_name, "_unpacked"), "wb");
        unsigned long long packed_stream_size = readLongLongInt(input_file);
        printf("Stream size: %llu\n", packed_stream_size);
        unsigned long long original_stream_size = readLongLongInt(input_file);
        printf("Original size: %llu\n", original_stream_size);
        int file_attributes = getc(input_file);
        printf("File attributes: %d\n", file_attributes);
        int timedate = readInt(input_file);
        printf("Timedate: %d\n", timedate);
        
        decompress_function(input_file, output_file, packed_stream_size);
        
    }
}