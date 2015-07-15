#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

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
//        printf("Cимвол: %c, код: ", newSymbol->c);
//        for(int i = 0; i < newSymbol->code_length; i++)
//            printf("%d", newSymbol->code[i]);
//        printf("\n");
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

void huff_compress(FILE* input_file, FILE* output_file) {
    
    //Построение таблицы частот
    int frequencyTable[256];
    for(int i = 0; i < 256; i++) {
        frequencyTable[i] = 0;
    }
    while(1) {
        char c = getc(input_file);
        if(c >= 0)
            frequencyTable[c]++;
        else
            break;
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
        
        //Склеиваем минимальные элементы в один
        Element* minElement1 = findAndDeleteMin();
        Element* minElement2 = findAndDeleteMin();
        Element* newParentElement = (Element*)malloc(sizeof(Element));
        newParentElement->frequency = minElement1->frequency + minElement2->frequency;
        newParentElement->left = minElement1;
        newParentElement->right = minElement2;
        leafs.push_back(newParentElement);
        
    }
    
    codes.clear();
    leafs.clear();
    
//    printf("\nНачинаем генерировать дерево\n");
    Element* root;
    root = leafs[0];
    vector<int> vec;
    code cur_code = &vec;
    generateTree(root, cur_code);
    
//    printf("\nВыводим\n");
//    for(int i = 0; i < codes.size(); i++) {
//        printf("Код %c(%d): ", codes[i]->c, codes[i]->code_length);
//        for(int j = 0; j < codes[i]->code_length; j++)
//            printf("%d", codes[i]->code[j]);
//        printf("\n");
//    }
    
    //Запись потока
    while(1) {
        unsigned char input_byte = getc(input_file);
        unsigned char output_byte;
        int used_bits = 0;
        if(input_byte >= 0) {
            for(int i = 0; i < codes.size(); i++) {
                if(input_byte == codes[i]->c) {
                    while(1) {
                        
                    }
                }
            }
        } else {
            
        }
    }
    
}