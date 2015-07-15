#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef struct _Element {
    char c;
    int frequency;
    struct _Element *left, *right;
} Element;

//const unsigned int MAX_INT = 4294967295;

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
    printf("===Frequencies:===\n");
    int lettersCount = 0;
    for(int i = 0; i < 256; i++) {
        if(frequencyTable[i] > 0) {
            printf("%c: %d\n", i, frequencyTable[i]);
            lettersCount++;
        }
    }
    
    //Дерево
    vector<Element*> leafs;
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
    
    printf("Forest:\n");
    for(int i = 0; i < leafs.size(); i++) {
        printf("%c: %d\n", leafs[i]->c, leafs[i]->frequency);
    }
    
/*
    //Поиск минимальных частот
    unsigned int minFreq1 = MAX_INT;
    int minFreq1index = 0;
    unsigned int minFreq2 = MAX_INT;
    int minFreq2index = 0;
    int prevMinFreq = 0;
    for(int i = 0; i < 256; i++) {
        if(frequencyTable[i] > 0)
            if(frequencyTable[i] > 0 && frequencyTable[i] <= minFreq1) {
                minFreq2 = minFreq1;
                minFreq2index = minFreq1index;
                minFreq1 = frequencyTable[i];
                minFreq1index = i;
            }
    }
*/
/*
    printf("minFreq1: %d\n", minFreq1);
    printf("minFreq2: %d\n", minFreq2);
    printf("minFreq1index: %d\n", minFreq1index);
    printf("minFreq2index: %d\n", minFreq2index);
*/
}
