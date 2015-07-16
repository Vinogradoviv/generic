#ifndef PACKING_H
#define	PACKING_H

#include <stdio.h>

static const int PACK_ALG_NOPE = 0;
static const int PACK_ALG_HUFF = 1;

unsigned long long nope_compress(FILE* input_file, FILE* output_file);
unsigned long long huff_compress(FILE* input_file, FILE* output_file);
void nope_decompress(FILE* input_file, FILE* output_file, unsigned long long stream_size);
void huff_decompress(FILE* input_file, FILE* output_file, unsigned long long stream_size);
int pack(unsigned char fileCount, char** input_filename_arr, char* output_file_name, int pack_alg);
int unpack(char* filename);

#endif