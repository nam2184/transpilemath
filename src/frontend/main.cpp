#include "lexical.cpp"
#include <iostream>
int main(){
  mathnotation::LexicalAnalyser lexicalObj;
 
  lexicalObj.scan("input.txt");
  return 0;
    
}