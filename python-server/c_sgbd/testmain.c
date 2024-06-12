#include"interface.h"
#include"index.h"

int main(){
    // FILE *my = fopen("./env/binario7.bin", "rb"), *std= fopen("./Saída/binario7.bin", "rb");
    // logList(my, extraiHeader(my), "my.txt");
    // logList(std, extraiHeader(std), "std.txt");
    //FILE *my = fopen("./env/indice7.bin", "rb"), *std= fopen("./Saída/indice7.bin", "rb");
    INDEX* mi = loadIndex("./env/indice4.bin");
    INDEX *std = loadIndex("./Saída/indice4.bin");
    printArr(mi);
    printArr(std);
}