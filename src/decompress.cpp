#include "huffmanlib.h"
#include <iostream>

#define HEADER "HUFFMAN DECOMPRESSION | @ivanreeve\nNote: decompressed files are saved with '.txt' extension.\n\n"
#define PROMPT "Enter file name (without -> .huffman): "
#define SUCCESS_MESSAGE "\n[SYSTEM] Decompression successful!"
#define FAILED_MESSAGE "\n[SYSTEM] Decompression failed. please check if the file name exists."
#define WAIT_MESSAGE "\n[SYSTEM] Please wait. Decompression may take a while with larger files.\n"

using namespace std;

int main(){
    string filename;

    while(true){
        system("cls");
        cout << HEADER;
        cout << PROMPT;

        cin >> filename;
        cout << WAIT_MESSAGE;

        cout << (huffmanlib::decompress(filename) ? SUCCESS_MESSAGE : FAILED_MESSAGE) << endl;
        system("pause");
    }
}
