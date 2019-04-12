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

#include <string> // for Huffman Code

using std::string;

// value structure for the huffman code table
// C++ compiler complains about anonymous structs, so we name it
using TableEntry = struct _TableEntry {
    uint count; // the instances of a alphabet symbol
    double freq; // the count/total_of_all_symbol_instances
    string code; // the huffman codeword 
}; 