#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "utils.h"

using namespace std;

typedef struct _Element {
    unsigned char c;
    int frequency;
    struct _Element *left, *right;
} Element;

typedef vector<int>* code;

typedef struct _Symbol {
    unsigned char c;
    int code_length;
    int* code;
} Symbol;

const unsigned int MAX_INT = 2000000000;

vector<Element*> leafs;
vector<Symbol*> codes;

Element* findAndDeleteMin() {
    int minFreq = MAX_INT;
    int minIndex = 0;
    Element* minElement = NULL;
    for(int i = 0; i < leafs.size(); i++) {
        if(leafs[i]->frequency < minFreq) {
            minFreq = leafs[i]->frequency;
            minIndex = i;
            minElement = leafs[i];
        }
    }
    leafs.erase(leafs.begin() + minIndex);
    return minElement;
}

void generateTree(Element* e, code current_code) {
    if(!e->left && !e->right) {
        Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
        newSymbol->c = e->c;
        newSymbol->code_length = current_code->size();
        newSymbol->code = (int*)malloc(newSymbol->code_length*sizeof(int));
        for(int i = 0; i < newSymbol->code_length; i++) {
            newSymbol->code[i] = current_code->at(i);
        }
        codes.push_back(newSymbol);
        return;
    }
    if(e->left) {
        current_code->push_back(0);
        generateTree(e->left, current_code);
        current_code->pop_back();
    }
    if(e->right) {
        current_code->push_back(1);
        generateTree(e->right, current_code);
        current_code->pop_back();
    }
}

unsigned long long huff_compress(FILE* input_file, FILE* output_file) {
    
    codes.clear();
    leafs.clear();
    
    //Построение таблицы частот
    int frequencyTable[256];
    for(int i = 0; i < 256; i++) {
        frequencyTable[i] = 0;
    }
    unsigned long long byte_count = 0;
    while(1) {
        int c = getc(input_file);
        if(c >= 0) {
            byte_count++;
            frequencyTable[c]++;
        }
        else
            break;
    }
    
    //Если файл пустой
    if(byte_count == 0) {
        writeLongLongInt(output_file, 0);
        return 0;
    }
    
    //Инициализируем дерево
    for(int i = 0; i < 256; i++) {
        if(frequencyTable[i] > 0) {
            Element* newElement = (Element*)malloc(sizeof(Element));
            newElement->c = i;
            newElement->frequency = frequencyTable[i];
            newElement->left = NULL;
            newElement->right = NULL;
            leafs.push_back(newElement);
        }
    }
    
    //Строим дерево
    while(leafs.size() > 1) {
        
        Element* minElement1 = findAndDeleteMin();
        Element* minElement2 = findAndDeleteMin();
        Element* newParentElement = (Element*)malloc(sizeof(Element));
        newParentElement->frequency = minElement1->frequency + minElement2->frequency;
        newParentElement->left = minElement1;
        newParentElement->right = minElement2;
        leafs.push_back(newParentElement);
        
    }
    
    //Присваивание кодов
    Element* root;
    root = leafs.back();
    vector<int> vec;
    code cur_code = &vec;
    generateTree(root, cur_code);
    
    //Запись дерева
    unsigned long long tree_size = 0;
    unsigned long long tree_begin_offset = ftell(output_file);
    writeLongLongInt(output_file, tree_size);
    for(int i = 0; i < codes.size(); i++) {
        fprintf(output_file, "%c", codes[i]->c);
        fprintf(output_file, "%c", codes[i]->code_length);
        tree_size += 2;
        for(int j = 0; j < codes[i]->code_length; j++) {
            fprintf(output_file, "%c", codes[i]->code[j]);
            tree_size++;
        }
    }
    unsigned long long tree_end_offset = ftell(output_file);
    fseek(output_file, tree_begin_offset, SEEK_SET);
    writeLongLongInt(output_file, tree_size);
    printf("Huffman tree size: %llu\n", tree_size);
    fseek(output_file, tree_end_offset, SEEK_SET);
    
    //Запись потока
    fseek(input_file, 0, SEEK_SET);
    unsigned long long packed_data_size = 0;
    unsigned char output_byte = 0;
    int byte_offset = 0;
    while(1) {
        int input_byte = getc(input_file);
        if(input_byte >= 0) { 
            int code_offset = 0;
            //Поиск кода
            for(int i = 0; i < codes.size(); i++) {
                if(input_byte == codes[i]->c) {
                    //Запись бита
                    while(1) {
                        if(code_offset >= codes[i]->code_length) {
                            break;
                        }
                        if(byte_offset > 7) {
                            fprintf(output_file, "%c", invertByte(output_byte));
                            packed_data_size++;
                            output_byte = 0;
                            byte_offset = 0;
                        }
                        output_byte |= (unsigned char)(pow(2, 8 - (byte_offset+1)))*codes[i]->code[code_offset];
                        byte_offset++;
                        code_offset++;
                    }
                    break;
                }
            }
        } else {
            break;
        }
    }
    
    //Оставшийся байт
    if(byte_offset > 0) {
        fprintf(output_file, "%c", output_byte);
        packed_data_size++;
    }
    
    return packed_data_size + tree_size + 8;
    
    printf("\n");
    
}
void huff_decompress(FILE* input_file, FILE* output_file, unsigned long long stream_size, unsigned long long original_size) {
    
    unsigned long long tree_size = readLongLongInt(input_file);
    
    if(original_size == 0)
        return;
    
    //Считывание списка кодов
    codes.clear();
    unsigned long long offset = 0;
    while(1) {
        if(offset > tree_size - 1) {
            break;
        }
        Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
        newSymbol->c = getc(input_file);
        newSymbol->code_length = getc(input_file);
        newSymbol->code = (int*)malloc(newSymbol->code_length*sizeof(int));
        offset += 2;
        for(int i = 0; i < newSymbol->code_length; i++) {
            newSymbol->code[i] = getc(input_file);
            offset++;
        }
        codes.push_back(newSymbol);
    }
    
    //Декодирование
    unsigned long long data_size = stream_size - tree_size - 8;
    unsigned char current_byte;
    int byte_offset = 8;
    unsigned long long stream_offset = 0;
    vector<int> current_code_bits;
    int bits[8];
    bool endFlag = false;
    unsigned long long byte_count = 0;
    while(1) {
        if(byte_offset >= 8) {
            if(stream_offset < data_size) {
                byte_offset = 0;
                current_byte = invertByte(getc(input_file));
                stream_offset++;
                for(int i = 0; i < 8; i++) {
                    unsigned char mask = pow(2, (8-i-1));
                    bits[i] = (current_byte & mask) > 0;
                }
            } else {
                break;
            }
        }
        current_code_bits.push_back(bits[byte_offset]);
        for(int j = 0; j < codes.size(); j++) {
            if(codes[j]->code_length != current_code_bits.size())
                continue;
            bool found = true;
            for(int k = 0; k < current_code_bits.size(); k++) {
                if(current_code_bits[k] != codes[j]->code[k]) {
                    found = false;
                    break;
                }
            }
            if(found) {
                fprintf(output_file, "%c", codes[j]->c);
                byte_count++;
                if(byte_count >= original_size)
                    endFlag = true;
                current_code_bits.clear();
                break;
            }
        }
        if(endFlag) {
            printf("%llu bytes unpacked\n", byte_count);
            break;
        }
        byte_offset++;
    }
}