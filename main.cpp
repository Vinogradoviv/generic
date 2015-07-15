#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packing.h"

//const char* archive_extension = ".upa";

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("-p - pack, -u - unpack\n");
        
    }
    if(argc > 1) {
        int fileCount = argc - 2;
        char** filename_arr = (char**)malloc((fileCount)*sizeof(char*));
        for(int i = 0; i < fileCount; i++) {
            filename_arr[i] = argv[i+2];
        }
        char* command = argv[1];
        if(strcmp(argv[1], "-p") == 0)
            pack(fileCount, filename_arr, "output.upa", PACK_ALG_HUFF);
        if(strcmp(argv[1], "-u") == 0)
            unpack(argv[2]);
    }
    
    return 0;
}