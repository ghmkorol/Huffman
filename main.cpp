/**
 * S19 CSCI 332 Design and Analysis of Algorithms
 * 
 * Project-2 Efficient Elevator Control
 * 
 * Simulation Driver
 * 
 * Phillip J. Curtiss, Assistant Professor
 * pcurtiss@mtech.edu, 406-496-4807
 * Department of Computer Science, Montana Tech
 */

#include <cstdlib>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <map> 
#include <algorithm> // stl algorithms; count, remove, etc.
#include <unordered_map>
#include <regex> //for replacement in string


#include "HuffmanTree.h"


#include <bitset> // for representing numeric values as binary
#include <array> // generic array object and methods


using std::cout;
using std::cin; 
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::ifstream;
using std::vector;
using std::queue;
using std::string;
using std::map;
using std::count;
using std::array;
using std::ostringstream;
using std::unordered_map;

#include <bitset>         
using std::bitset;


using TableEntry = struct _TableEntry {
    uint count; // the instances of a alphabet symbol
    double freq; // the count/total_of_all_symbol_instances
    string code; // the huffman codeword 
};

// Forward Declaration for creating huffman codes
void HuffmanCode(map<char, TableEntry>&);
void PrintCodeTable(const map<char, TableEntry> &sm);
map<char, TableEntry> GenHuffmanCode(string fileName);

//https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
short GenKey(short A, short B)
{
    return A<<8 | B;
}
short getA(short C)
{
    return C>>8;
}
short getB(short C)
{
    //return C & 0xFFFF;
    return C & 255;
}

char findCharByCode(map<char, TableEntry> table,string code){
  for (auto x: table) {
    if(x.second.code == code){
      return x.first;
    }
  }
  return '?';
}

void writeHashMapToBinFile(unordered_map<short, string> umap, string filename){
  
  //from map to string
  string fullString="";
  for (auto x: umap) {
      string binary = bitset<16>(x.first).to_string();
      fullString += binary;
      fullString += x.second;
      
    }
  //cout << "full string: " <<fullString << endl;  
  
  
  //seve string bits to real binary file
  int NbytesToBeWrite = (int)fullString.length()/8+1;
  std::ofstream output(filename,std::ios::binary);
  for(int i = 0; i < NbytesToBeWrite; i++){
    string tempStr =  fullString.substr(i*8,8);
    bitset<8> bits(tempStr);
    output.write( reinterpret_cast<char *>(&bits), 1 );
  }
  output.close();
  
  
}


unordered_map<short, string> readHashMapFromBinFile(string fileNameBin, map<char, TableEntry> hCode){
  
  unordered_map<short, string> uMapFromBinFile;
  
  //read from file to fullString
  string fullString="";
  std::ifstream is(fileNameBin, std::ios::binary);
  bitset<8> bits;
  while(is.read(reinterpret_cast<char *>(&bits), 1)){
    fullString+=bits.to_string();
  }
  
  //from fullString to hash map
  for(int i=0;i<(int)fullString.length();i+=0 ){
    string strC = fullString.substr(i,16);
    if(strC.length()<16)break;
    bitset<16> bitsetC(strC);
    i+=16;
    short C = (short)(bitsetC.to_ulong());
    
    string code = "";
    int counter = 1;
    while(1){
      for (auto x: hCode){
	if(fullString.substr(i,counter)==x.second.code){
	  code=fullString.substr(i,counter);
	  break;
	}
      }
      if(code.length()>0)break;
      else counter++;
      if(counter>100)break;
    }
    if(code.length()<100){
      i+=code.length();
      uMapFromBinFile[C]=code;
//       cout << code << " " ;
    }
  }  
  
  return uMapFromBinFile;
}


void printImage(unordered_map<short, string> uMap,int maxNX,int B,map<char, TableEntry> hCode){
  
  vector< vector<char> > text2DvectorChar(B,vector<char>(maxNX,'\n'));
  //from unordered hash map to 2d vector of chars
      for (auto x: uMap) {
      short A=getA(x.first);
      short B=getB(x.first);
      text2DvectorChar[B][A]=findCharByCode(hCode,x.second);
    }
  //print from 2d vectro of chars
    for(auto& row:text2DvectorChar){
      for(auto& col:row){
	if(col=='\n')break;
	cout << col;
      }
      cout << endl;
    }
  
}

int main(int argc, char *argv[])
{
  string fileName = "./images/stegosaurus.in";
  if(argc==2){
    fileName = argv[1];
  }
  std::cout << "fileName: " << fileName <<std::endl;
  string fileNameBin = std::regex_replace(fileName, std::regex(".in"), ".bin");
  fileNameBin = std::regex_replace(fileNameBin, std::regex("/images"), "");
  std::cout << "fileNameBin: " << fileNameBin <<std::endl;
  
  map<char, TableEntry> hCode = GenHuffmanCode(fileName);  
  unordered_map<short, string> uMap;
  
  int nX=0;
  int maxNX=0;
  
  short A=0;
  short B=0;
  
  std::ifstream infile(fileName); 
  std::string line;
  while (std::getline(infile, line))
  {
    
    for(short i = 0; i < (int) line.length(); ++i)
    {
      A=i;
      char ch = line[i];
      short key = GenKey(A,B);
      //fill hash map
      uMap[key] = hCode.at(ch).code;
//       cout << hCode.at(ch).code << " " ;
    }
    B++;
    
    nX=line.length();
    if(nX>maxNX)maxNX=nX;
  }
  nX=maxNX;
  
  
  //write hash map to binary file
  writeHashMapToBinFile(uMap,fileNameBin);
  
//   cout << "code after: " << endl;
  //read hash map from binary file and store data to 2d vector of chars
  unordered_map<short, string> uMapFromBinFile = readHashMapFromBinFile(fileNameBin,hCode);  
  
  cout << "Print image ***Before*** writing to binary file: "<< endl;;
  printImage(uMap,maxNX,B,hCode);
  cout << "Print image ***After*** reading from binary file: " << endl;
  printImage(uMapFromBinFile,maxNX,B,hCode);
  
  

  
  
  
  // save unorederd map to the binary file file
  //https://stackoverflow.com/questions/47929150/writing-bits-to-file
  
  
  

  
    return EXIT_SUCCESS;
}



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


map<char, TableEntry> GenHuffmanCode(string fileName)
{
    // associative container representing the Symbol map
    map<char, TableEntry> SymbolMap;

    // holds a line from the input
    string buffer;

    // counter for the total number of symbols encountered on 
    // the input; used to compute individual symbol frequency
    int total = 0;
    
    std::ifstream infile(fileName);
    
    // while there is input, store it a line at a time in buffer
    while (getline(infile, buffer)){
      
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
    return SymbolMap;
} // end main routine

