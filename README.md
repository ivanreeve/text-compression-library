# Huffman Text Compression Library

This is a C++ library for Huffman text compression and decompression algorithm.

## Features

### Compression Ratio

The Huffman text compression algorithm employed by this library aims to achieve significant reduction in file size while maintaining lossless data compression. While the actual compression ratio may vary depending on factors such as the content and structure of the input text files, tests have shown that the compression ratio can reach up to 50%. 

## Getting Started

To use this library in your C++ project, follow these steps:

1. Clone the repository or download the source code.
2. Include the necessary files in your project. Make sure that the header file is in the appropriate folder location.
3. Use the provided functions for compression and decompression.

## Functions

```cpp
huffmanlib::compress(string& filename): bool : <filename>.compressed.huffman
```

Takes a string ```filename``` as a parameter. Returns true if the compression is successful. Assumes that the filename has ```".txt"``` file extension omitted. For example, if the file name is ```"sample.txt"```, the input should be "sample". Appends ```".compressed.huffman"``` file extension to the compressed file.

```cpp
huffmanlib::decompress(string& filename): bool : <filename>.decompressed.txt
```

Takes a string ```filename``` as a parameter. Returns true if the decompression is successful. Assumes that the filename has ```".huffman"``` file extension omitted. For example, if the file name is "sample.txt.compressed.huffman", the input should be "sample.txt.compressed". Appends ```".decompressed.txt"``` file extension to the decompressed file.

## Usage

```cpp
#include <iostream>
#include "huffman.h"

using namespace std;
using namespace huffmanlib;

int main() {
    // Compress a text file
    compress("input");

    // Decompress a compressed file
    decompress("compressed");

    return 0;
}
```

For more information, see the [official documentation](https://example.com/docs).

## License
This project is licensed under the [MIT License](LICENSE).
