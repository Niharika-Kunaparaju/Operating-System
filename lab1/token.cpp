#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

using namespace std;

template <class T> 
class Token{
  private:
    int lineNumber;
    int columnNumber;
    T token;
    int length;

  public:
    Token(int lineNumber, int columnNumber, T token, int length){
      this->lineNumber = lineNumber;
      this->columnNumber = columnNumber;
      this->token = token;
      this->length = length;
    }

    int getLineNumber(){
      return this->lineNumber;
    }

    int getColumnNumber(){
      return this->columnNumber;
    }

    T getValue(){
      return this->token;
    }

    int getLength(){
      return  this->length;
    };
};

#endif