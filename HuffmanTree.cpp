
/**
 * S19 CSCI 332 Design and Analysis of Algorithms
 * 
 * Huffman Image Compression
 * (and Hashing)
 * 
 * Phillip J. Curtiss, Assistant Professor
 * pcurtiss@mtech.edu, 406-496-4807
 * Department of Computer Science, Montana Tech
 */

#include <cstdlib> // standard c++ routines
#include <memory> // shared pointers
#include <vector> // underlying storage for priority queue
#include <queue> // priority queue routines
#include <string> // string object and methods
#include <map> // associative container

#include "GenHuffmanCode.h" // table entry for SymbolMap
#include "HuffmanTree.h" // Tree structures

using std::map;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::priority_queue;
using std::vector;

#define SENTINEL '$' // symbol not in input stream - choose carefully

/**
 * Recursively map Huffman tree to binary representation for all
 * leaf nodes in the tree - i.e. symbols processed
 * store the binary representation in the SymbolMap for the corresponding Symbol
 * @param sm - SymbolMap
 * @param root - root of Huffman Tree
 * @ cw - binary code word, constructed recursively
 */
void CodeWord(map<char, TableEntry> &sm, shared_ptr<MinHeapNode> node, string cw) 
{
    // return if node is empty
    if (node == nullptr)
        return;

    // if symbol at tree node is not the sentinel, then must be a symbol
    // node, therefore cw should represent the binary Huffman Code Word
    if (node->symbol != SENTINEL)
    {
        sm[node->symbol].code = cw;
        return;
    }

    // recurse down the left child node updating the code word
    CodeWord(sm, node->left, cw + "0" );

    // recurse down the right child node updating the code word
    CodeWord(sm, node->right, cw + "1" );

} // end CodeWord

/**
 * Generate the Huffman Code for each symbol in the SymbolMap
 * by creating a Huffman Tree - a MinHeap
 */
void HuffmanCode(map<char, TableEntry> &sm)
{
    // Pointers to MinHeapNodes
    shared_ptr<MinHeapNode> top, left, right;

    // Priority Queue representing a MinHeap - the symbol frequency determines priority order
    priority_queue<shared_ptr<MinHeapNode>, 
                   vector<shared_ptr<MinHeapNode> >, 
                   compare > MinHeap;

    // add to the MinHeap a MinHeapNode for each symbol in the SymbolMap
    for (auto element : sm)
        MinHeap.push(make_shared<MinHeapNode>(element.first, element.second.freq));

    // until the MinHeap only consists of a single node (the root of the Huffman tree)
    while (MinHeap.size() > 1) {
        // obtain the node with the smallest frequency
        left = MinHeap.top();
        MinHeap.pop();

        // obtain the node with the next smallest frequency
        right = MinHeap.top();
        MinHeap.pop();

        // create a new node from each of the previous node
        // whose symbol is the sentinel, and whose freq is 
        // the sum of the two nodes' frequencies
        // link the original nodes to form a tree
        top = make_shared<MinHeapNode>(SENTINEL, left->freq + right->freq);
        top->left = left;
        top->right = right;

        // add the newly created node to the MinHeap
        MinHeap.push(top);
    
    } // end MinHeap

    // for the Huffman tree in the MinHeap, create the 
    // binary code words and add these to the symbols in the SymbolMap
    CodeWord(sm, MinHeap.top(), "");

} // end HuffmanCode
