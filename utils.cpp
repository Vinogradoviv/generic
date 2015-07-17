#include <cstdio>
#include <cmath>

void writeLongLongInt(FILE* output_file, unsigned long long d) {
    unsigned char bytes[8];
    for(int i = 0; i < 8; i++) {
        unsigned long long mask = (unsigned long long)pow(2, 8*(8-i)) - (unsigned long long)pow(2, 8*(8-i-1));
        bytes[i] = (d & mask) >> 8*(8-i-1);
    }
    for(int i = 0; i < 8; i++) {
        fprintf(output_file, "%c", bytes[i]);
    }
}

unsigned long long readLongLongInt(FILE* input_file) {
    unsigned long long d = 0;
    for(int i = 0; i < 8; i++) {
        unsigned char c = getc(input_file);
        d |= c << 8*(8-i-1);
    }
    return d;
}

void writeInt(FILE* output_file, int d) {
    unsigned char bytes[4];
    for(int i = 0; i < 4; i++) {
        unsigned long long mask = (int)pow(2, 4*(i-4)) - (int)pow(2, 4*(i-4-1));
        bytes[i] = (d | mask) >> (int)pow(2, 4*(i-4-1));
    }
    for(int i = 0; i < 4; i++) {
        fprintf(output_file, "%c", bytes[i]);
    }
}

int readInt(FILE* input_file) {
    int d = 0;
    for(int i = 0; i < 4; i++) {
        unsigned char c = getc(input_file);
        d |= c << 4*(4-i-1);
    }
    return d;
}
