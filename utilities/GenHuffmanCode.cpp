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

#include <cstdlib> // std c/c++ library routines
#include <iostream> // i/o routines (cout, etc.)
#include <algorithm> // stl algorithms; count, remove, etc.
#include <sstream> // string stream; for formatted output
#include <string> // string methods and iterators
#include <bitset> // for representing numeric values as binary
#include <array> // generic array object and methods
#include <map> // associative containers; <k,v> store

#include "GenHuffmanCode.h" // for TableEntry stucture

// explicit namespace imports
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::count;
using std::string;
using std::array;
using std::ostringstream;

// Forward Declaration for creating huffman codes
void HuffmanCode(map<char, TableEntry>&);

/**
 * Output the Symbol Table Consisting of
 * for each Symbol encountered on the input:
 * count - number of instances of the symbol on the input
 * freq  - the percenter of the symbol compared to all symbols
 * code  - binary Huffman code from Huffman tree traversal
 * @param - a reference SymbolMap associative array
 */
void PrintCodeTable(const map<char, TableEntry> &sm)
{
    #define SYMBOL 0 // manifest constant for symbol string stream
    #define COUNT  1 // ... for count string stream
    #define FREQ   2 // ... for frequency string stream
    #define CODE   3 // ... for code string stream

    // stringstream arrays for formatting table output 
    // in O(n) where n is the number of symbols encountered
    // on the input
    array<ostringstream, 4> codeTable;

    // index for iterating over labels and label width
    const array<string, 4> labels({"Symbol: ", "Count: ", "Frequency: ", "Code Word: "});
    int ndx = 0, 
        label_width = 11, // width of table labels
        data_width = 10, // width of table data fields
        precision = 2; // precision of decimal data fields

    // Store in the corresponding table lines the left labels
    // and set default formatting for the rest of the entries 
    // written to the string stream
    for (auto label : labels) {
        // set the width of the table line label
        // and write out to the string stream the label
        codeTable[ndx].width(label_width);
        codeTable[ndx] << label;

        // if the table line requires decimal format, set it
        if (ndx == COUNT || ndx == FREQ)
            codeTable[ndx].setf(std::ios::dec);

        // if the table line requires decimal precision, set it
        if (ndx == FREQ)
            codeTable[ndx].precision(precision);

        // go to next table line
        ndx++;

    } // end table labels

    // for every element in the Symbol Table
    for (auto element : sm) {
        // set bcode to be a binary representation of Huffman code
        //bitset<8> bcode(element.second.code);

        // add to the symbol line of the table the symbol
        codeTable[SYMBOL].width(data_width);
        codeTable[SYMBOL] << element.first;

        // add to the count line of the table the symbol count
        codeTable[COUNT].width(10);
        codeTable[COUNT] << element.second.count;

        // add to the frequency line of the table the symbol freq
        codeTable[FREQ].width(10);
        codeTable[FREQ] << element.second.freq;

        // add to the code line of the table the symbol Huffman code
        codeTable[CODE].width(10);
        codeTable[CODE] << element.second.code; 

    } // end iterate over huffman code associative array

    // send to the output the symbol line of the table
    cout << endl << codeTable[SYMBOL].str() << endl;

    // send to the output a separator line
    cout.width(codeTable[0].str().length()+1); 
    cout.fill('-');
    cout << " " << endl; // output a single space
    cout.width(); // unset width 

    // send to the output the count line of the table
    cout << codeTable[COUNT].str() << endl << endl;

    // send to the output the frequency line of the table
    cout << codeTable[FREQ].str() << endl << endl;

    // send to the output the Huffman code line fo the table
    cout << codeTable[CODE].str() << endl << endl;

} // end PrintCodeTable

/**
 * read standard input and for each symbol encountered
 * compute a huffman codeword; present results for all
 * symbols on the input in a table format
 */
int main()
{
    // associative container representing the Symbol map
    map<char, TableEntry> SymbolMap;

    // holds a line from the input
    string buffer;

    // counter for the total number of symbols encountered on 
    // the input; used to compute individual symbol frequency
    int total = 0;

    // while there is input, store it a line at a time in buffer
    while (getline(cin, buffer)){
        // so long as there are still characters in the buffer...
        while (buffer.length() > 0) {
            // store the first character in the buffer
            char c = buffer[0];

            // obtain number of instances of the character c
            // contained in the buffer string using iterators
            // and store this count for later processing
            uint c_count = count(buffer.begin(), buffer.end(), c);

            // remove all instances of the character c from the buffer
            // using the string iterators and the erase method on strings
            buffer.erase(remove(buffer.begin(), buffer.end(), c), buffer.end());

            // update/insert the symbol c in the SymbolMap by
            // incrementing the number of symbols found on the line
            // read from the input
            SymbolMap[c].count += c_count;

            // accumulate the total symbols read by the size of the buffer
            total += c_count;

        } // end processing line of input

    } // end reading a line from input

    // for every element (symbol) in the SymbolMap
    // compute its frequency from the count and total symbols
    // encountered from processing the input
    for (auto element : SymbolMap)
        // note: element is _N_O_T_ a reference, rather a copy of the map element value
        // therefore, element.second.freq = element.second.count / total will set
        // the freq member element (copy), but not change the value in the SymbolMap
        // need to use the code below to avoid this situation of not having a reference
        // and actually update the entry in the associative array (SymbolMap)
        // oh the joys of passing structs in C/C++ ;-/
        SymbolMap[element.first].freq = ((double) element.second.count) / total;

    // Generate the Huffman code for each symbol in the SymbolMap
    HuffmanCode(SymbolMap);

    // send to the output the Huffman Code Table
    PrintCodeTable(SymbolMap);

    // exit signalling success to the shell
    return EXIT_SUCCESS;
} // end main routine
