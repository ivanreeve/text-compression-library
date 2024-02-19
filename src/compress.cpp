#include "huffmanlib.h"
#include <iostream>

#define HEADER "HUFFMAN COMPRESSION | @ivanreeve\nNote: compressed files are saved with '.huffman' extension.\n\n"
#define PROMPT "Enter file name (without -> .txt): "
#define SUCCESS_MESSAGE "\n[SYSTEM] Compression successful!"
#define FAILED_MESSAGE "\n[SYSTEM] Compression failed. please check if the file name exists."
#define WAIT_MESSAGE "\n[SYSTEM] Please wait. Compression may take a while with larger files.\n"

using namespace std;

int main(){
    string filename;

    while(true){
        system("cls");
        cout << HEADER;
        cout << PROMPT;

        cin >> filename;
        cout << WAIT_MESSAGE;

        cout << (huffmanlib::compress(filename) ? SUCCESS_MESSAGE : FAILED_MESSAGE) << endl;
        system("pause");
    }
}
