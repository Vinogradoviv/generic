#ifndef PACKING_H
#define	PACKING_H

static const int PACK_ALG_NOPE = 0;
static const int PACK_ALG_HUFF = 1;

void nope_compress(FILE* input_file, FILE* output_file);
int pack(unsigned short int fileCount, char** input_filename_arr, char* output_file_name, int pack_alg) ;
int unpack(char* filename);

#endif

