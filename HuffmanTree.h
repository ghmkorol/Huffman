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
#include <memory> // shared pointers

using std::shared_ptr;

/**
 * MinHeapNode type
 */
using MinHeapNode = struct _MinHeapNode 
{
    char symbol; // symbol from SymbolMap or sentinel
    double freq; // priority value for MinHeap

    shared_ptr<_MinHeapNode> left; // left child tree node
    shared_ptr<_MinHeapNode> right; // right child tree node

    // constructor for type
    _MinHeapNode(char symbol, double freq) : symbol(symbol),
                                           freq(freq),
                                           left(nullptr),
                                           right(nullptr)
    {
    }
}; // end MinHeapNode

/**
 * Comparator for Priority Queue
 * generate priority based on MinHeapNode frequency
 */
using compare = struct {
    bool operator() (shared_ptr<MinHeapNode> lhs, 
                     shared_ptr<MinHeapNode> rhs)
    {
        // compare frequency values for priority order
        return (lhs->freq > rhs->freq);
    } // end invocation operator
}; // end compare

