#include <unordered_map>
#include <string>
#include <fstream>
#include <bitset>
#include <memory>

#define BYTE_LENGTH 8
#define EMPTY '\0'
#define INPUT_FILENAME_EXTENSION ".txt"
#define COMPRESSED_OUTPUT_FILENAME_EXTENSION ".compressed.huffman"
#define COMPRESSED_INPUT_FILENAME_EXTENSION ".huffman"
#define DECOMPRESSED_OUTPUT_FILENAME_EXTENSION ".decompressed.txt"

using namespace std;

string FILENAME;

typedef struct huffmanNode {

    char character;
    size_t frequency;
    huffmanNode* right;
    huffmanNode* left;

    huffmanNode(char ch) : character(ch), frequency(1), right(nullptr), left(nullptr) {}

} HUFFMAN_NODE;

typedef struct priorityQueueNode {

    HUFFMAN_NODE* node;
    priorityQueueNode* next;

    priorityQueueNode(HUFFMAN_NODE* n) : node(n), next(nullptr){}

} PRIORITY_QUEUE_NODE;

class PriorityQueue {

    private:
        PRIORITY_QUEUE_NODE* frontNode = nullptr;
        bool isEmpty();
        void registerCharacter(char c);

    public:
        PriorityQueue(const string& filename){
            char c;
            ifstream inputFileStream(filename);
            while(inputFileStream.get(c)){
                registerCharacter(c);
            }
            inputFileStream.close();
        }

        void enqueue(PRIORITY_QUEUE_NODE* newNode);
        PRIORITY_QUEUE_NODE* dequeue(PRIORITY_QUEUE_NODE* lastNode, PRIORITY_QUEUE_NODE* currentNode);
        PRIORITY_QUEUE_NODE* getFrontNode();
};

bool PriorityQueue::isEmpty(){
    return (frontNode == nullptr);
}

void PriorityQueue::enqueue(PRIORITY_QUEUE_NODE* newNode){
    if (isEmpty()){
        frontNode = newNode;
        return;
    }

    PRIORITY_QUEUE_NODE* currentNode = frontNode;
    PRIORITY_QUEUE_NODE* lastNode = frontNode;

    while (currentNode != nullptr && newNode->node->frequency >= currentNode->node->frequency) {
        // Moves the pointer to a position where: frequency of newNode > lastNode but less than the next node (if there's any).
        lastNode = currentNode;
        currentNode = currentNode->next;
    }

    newNode->next = currentNode;

    if (currentNode == frontNode && lastNode == frontNode) frontNode = newNode;// if currentNode and lastNode point to the same object in memory, we know that we are dealing with the front node.
    else lastNode->next = newNode;

    return;
}

PRIORITY_QUEUE_NODE* PriorityQueue::dequeue(PRIORITY_QUEUE_NODE* lastNode, PRIORITY_QUEUE_NODE* currentNode){
    PRIORITY_QUEUE_NODE* tempNode = currentNode;

    if (currentNode == frontNode && lastNode == frontNode) frontNode = currentNode->next;
    else lastNode->next = currentNode->next;

    tempNode->next = nullptr;
    return tempNode;
}

void PriorityQueue::registerCharacter(char c){
    PRIORITY_QUEUE_NODE* currentNode = frontNode;
    PRIORITY_QUEUE_NODE* lastNode = frontNode;
    while(currentNode != nullptr){
        if (currentNode->node->character == c){
            /*
                Does the character already exist in the queue?
                Then increase its frequency and recalculate its position.
                NOTE: recalculation happens in enqueue()
            */
            PRIORITY_QUEUE_NODE* d = dequeue(lastNode, currentNode);
            d->node->frequency++;
            enqueue(d);
            return;
        }

        lastNode = currentNode;
        currentNode = currentNode->next;
    }
    HUFFMAN_NODE* h = new HUFFMAN_NODE(c);
    PRIORITY_QUEUE_NODE* p = new PRIORITY_QUEUE_NODE(h);
    enqueue(p);
    return;
}

PRIORITY_QUEUE_NODE* PriorityQueue::getFrontNode(){
    return frontNode;
}

class HuffmanTree {

    private:
        HUFFMAN_NODE* root;
        size_t numLeaf, numChar; //Metadata
        size_t charCount, bitstreamIndex;
        string serializedHuffmanTree, bitstream;
        unordered_map<char, string> huffmanData;

        bool isEmpty();
        void generateHuffmanData(HUFFMAN_NODE* currentNode, string binaryString);
        void serializeHuffmanTree(HUFFMAN_NODE* currentNode);
        HUFFMAN_NODE* deserializeHuffmanTree(size_t& i);
        void generateBitstream(const string& filename);
        void decodeBitstream(ofstream& outputFileStream);

    public:
        HuffmanTree(): serializedHuffmanTree(""), bitstream(""), numLeaf(0), numChar(0), charCount(0), bitstreamIndex(0) {}

        void buildHuffmanTree(PriorityQueue& PQ, const string& filename);
        void buildHuffmanTree(const string& filename);
        void saveCompressedData(const string& filename);
        void saveDecompressedData(const string& filename);
};

bool HuffmanTree::isEmpty(){
    return (root == nullptr);
}

void HuffmanTree::generateHuffmanData(HUFFMAN_NODE* currentNode, string binaryString){
    if (currentNode != nullptr){
        if (currentNode->character == EMPTY){
            generateHuffmanData(currentNode->left, binaryString + "0");
            generateHuffmanData(currentNode->right, binaryString + "1");
        }
        else{
            huffmanData[currentNode->character] = binaryString;
        }
    }
    return;
}

void HuffmanTree::serializeHuffmanTree(HUFFMAN_NODE* currentNode){
    if (currentNode != nullptr) {
        if (currentNode->character == EMPTY) {
            serializedHuffmanTree += "0";
            serializeHuffmanTree(currentNode->left);
            serializeHuffmanTree(currentNode->right);
        }
        else {
            numLeaf++;
            serializedHuffmanTree += "1";
            serializedHuffmanTree += currentNode->character;
        }
    }
    return;
}

HUFFMAN_NODE* HuffmanTree::deserializeHuffmanTree(size_t& i){

    if (i >= serializedHuffmanTree.size()) {
        return nullptr;
    }
    if (serializedHuffmanTree[i] == '1') {// Leaf
        i++;
        return new HUFFMAN_NODE(serializedHuffmanTree[i]);
    } else {// Not Leaf
        HUFFMAN_NODE* newNode = new HUFFMAN_NODE(EMPTY);
        i++;
        newNode->left = deserializeHuffmanTree(i);
        i++;
        newNode->right = deserializeHuffmanTree(i);

        return newNode;
    }
}


void HuffmanTree::generateBitstream(const string& filename){
    char c;
    ifstream inputFileStream(filename);

    while(inputFileStream.get(c)){
        bitstream += huffmanData[c];
        numChar++;
    }

    size_t lastByteLength = bitstream.size() % BYTE_LENGTH;
    size_t numZeroes = BYTE_LENGTH > lastByteLength ? (BYTE_LENGTH - lastByteLength) : 0;

    bitstream += string(numZeroes,'0'); // Padding for the last byte with incomplete bits

    inputFileStream.close();
    return;
}

void HuffmanTree::decodeBitstream(ofstream& outputFileStream) {
    HUFFMAN_NODE* node = root;
    while (charCount < numChar) {
        if (node->character == EMPTY) {
            if (bitstream[bitstreamIndex] == '1') node = node->right;
            else node = node->left;
            bitstreamIndex++;
        } else {
            outputFileStream << node->character;
            charCount++;
            node = root;
        }
    }
}

void HuffmanTree::buildHuffmanTree(PriorityQueue& PQ, const string& filename){
    PRIORITY_QUEUE_NODE* frontNode;
    while(PQ.getFrontNode()->next != nullptr){
        frontNode = PQ.getFrontNode();
        // The parameters have to be the same to successfully detach frontNode from the queue:
        PRIORITY_QUEUE_NODE* firstNode = PQ.dequeue(frontNode, frontNode);
        frontNode = PQ.getFrontNode();
        PRIORITY_QUEUE_NODE* secondNode = PQ.dequeue(frontNode, frontNode);

        HUFFMAN_NODE* combinedNode = new HUFFMAN_NODE(EMPTY);
        combinedNode->frequency = firstNode->node->frequency + secondNode->node->frequency;
        combinedNode->left = firstNode->node;
        combinedNode->right = secondNode->node;

        PRIORITY_QUEUE_NODE* newNode = new PRIORITY_QUEUE_NODE(combinedNode);
        PQ.enqueue(newNode);

        delete firstNode;
        delete secondNode;
    }
    root = PQ.getFrontNode()->node;
    generateHuffmanData(root, "");
    serializeHuffmanTree(root);
    generateBitstream(filename);
    return;
}

void HuffmanTree::buildHuffmanTree(const string& filename){

    ifstream inputFileStream(filename, ios::binary);
    size_t i, n;
    char nodeBuffer, charBuffer;

    inputFileStream.read(reinterpret_cast<char*>(&numLeaf), sizeof(numLeaf));
    inputFileStream.read(reinterpret_cast<char*>(&numChar), sizeof(numChar));

    i = 0;
    n = 0;

    while (i < numLeaf){ // Since we stored the number of leaves, we can tell the pointer when to stop reading characters as part of the serializedHuffmanTree
        inputFileStream.get(nodeBuffer);
        serializedHuffmanTree += nodeBuffer;

        if (nodeBuffer == '1'){
            inputFileStream.get(nodeBuffer);
            serializedHuffmanTree += nodeBuffer;
            i++;
        }
    }

    // Reading binary stream from file
    while(inputFileStream.read(&charBuffer, sizeof(char))){
        bitset<BYTE_LENGTH> bits(charBuffer);
        bitstream += bits.to_string();
    }

    inputFileStream.close();
    root = deserializeHuffmanTree(n);
    return;
}

void HuffmanTree::saveCompressedData(const string& filename){
    ofstream outputFileStream(filename+COMPRESSED_OUTPUT_FILENAME_EXTENSION, ios::binary | ios::trunc);
    string byteString;
    size_t i;
    uint8_t byteValue;
    size_t bitstreamSize = bitstream.size();

    outputFileStream.write(reinterpret_cast<const char*>(&numLeaf), sizeof(numLeaf));
    outputFileStream.write(reinterpret_cast<const char*>(&numChar), sizeof(numChar));
    outputFileStream.write(serializedHuffmanTree.c_str(), serializedHuffmanTree.size());

    // Saving the bits as binary
    for (i = 0; i < bitstreamSize; i += BYTE_LENGTH) {
        byteString = bitstream.substr(i, BYTE_LENGTH);
        byteValue = static_cast<uint8_t>(bitset<BYTE_LENGTH>(byteString).to_ulong());
        outputFileStream.write(reinterpret_cast<const char*>(&byteValue), sizeof(uint8_t));
    }

    outputFileStream.close();
    return;
}

void HuffmanTree::saveDecompressedData(const string& filename){
    ofstream outputFileStream(filename+DECOMPRESSED_OUTPUT_FILENAME_EXTENSION);

    decodeBitstream(outputFileStream);
    outputFileStream.close();
    return;
}

bool isFileEmpty(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    return file.tellg() == 0;
}

namespace huffmanlib {
    bool compress(const string& fn){
        const string filename = fn + INPUT_FILENAME_EXTENSION;//.txt excluded in fn
        ifstream inputFileStream(filename);

        if (inputFileStream.is_open() && !isFileEmpty(filename)){
            inputFileStream.close();
            PriorityQueue PQ(filename);
            HuffmanTree HT;
            HT.buildHuffmanTree(PQ, filename);
            HT.saveCompressedData(filename);
            return true;
        }
        else return false;
    }

    bool decompress(const string& fn){ ;//.huffman excluded in fn
        const string filename = fn + COMPRESSED_INPUT_FILENAME_EXTENSION;
        ifstream inputFileStream(filename);

         if (inputFileStream.is_open() && !isFileEmpty(filename)){
            inputFileStream.close();
            HuffmanTree HT;
            HT.buildHuffmanTree(filename);
            HT.saveDecompressedData(filename);
            return true;
         }
         else return false;
    }
}
