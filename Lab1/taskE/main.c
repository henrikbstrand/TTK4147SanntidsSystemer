#include "array.h"

int main(){
    Array array = array_new(10);
    for(int i = 0; i< 15; i++){
        array_insertBack(&array, i);
        array_print(array);
    }
    return 1;
}