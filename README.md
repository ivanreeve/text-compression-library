# Huffman Text Compression Library

This is a C++ library for Huffman text compression and decompression algorithm.

## Features

### Compression Ratio

The Huffman text compression algorithm employed by this library aims to achieve significant reduction in file size while maintaining lossless data compression. While the actual compression ratio may vary depending on factors such as the content and structure of the input text files, tests have shown that the compression ratio can reach up to 50%. 

## Getting Started

To use this library in your C++ project, follow these steps:

1. Clone the repository or download the source code.
2. Include the necessary files in your project.
3. Use the provided functions for compression and decompression.

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

## License
This project is licensed under the [MIT License](LICENSE).
