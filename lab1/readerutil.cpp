Token<char*> getNextToken(){
      char ch;
      int length = 0;
      int index = 0, tokenStartLine = AbstractReader::lineNumber, tokenStartColumn = AbstractReader::columnNumber;
      char* token = new char[512];
      bool tokenStart = false, isWhiteSpace = false, emptyLine = true;

      while(*(this->fin) >> ch){
        if(ch == '\n'){
          if(this->fin->tellg() != this->fileSize){
            AbstractReader::lineNumber++;
            AbstractReader::columnNumber = 1;
          }
          emptyLine = true;
        }else{
          emptyLine = false;
          AbstractReader::columnNumber++;
        }

        isWhiteSpace = (ch == ' ' || ch == '\n' || ch == '\t');

        if(tokenStart && isWhiteSpace){
          token[index] = '\0';
          break;
        }else if(isWhiteSpace){
          tokenStartLine = AbstractReader::lineNumber;
          tokenStartColumn = AbstractReader::columnNumber;
          continue;
        }else if(!tokenStart){ // token is beginning here
          tokenStartLine = AbstractReader::lineNumber;
          tokenStartColumn = AbstractReader::columnNumber -1;
          tokenStart = true;
          length = 0;
        }

        length++;
        token[index++] = ch;
      }

      Token<char*> newToken(tokenStartLine, tokenStartColumn, token, length);
      return newToken;
    }

    Token<int> getNextTokenAsInteger(bool allowZeroLength){
      Token<char*> nextToken = getNextToken();

      if(!allowZeroLength){
        if(nextToken.getLength() == 0){
          cout << "Parse Error line "<< nextToken.getLineNumber() << " offset "<< nextToken.getColumnNumber() << ": NUM_EXPECTED\n";
          exit(99);
        }
      }

      char *last = 0;
      int intVal = (int) strtol(nextToken.getValue(), &last, 10);
      if(*last != 0 || intVal < 0 ){
        cout << "Parse Error line "<< nextToken.getLineNumber() << " offset "<< nextToken.getColumnNumber() << ": NUM_EXPECTED\n";
        exit(99);
      }
      return Token<int>(nextToken.getLineNumber(), nextToken.getColumnNumber(), intVal, nextToken.getLength());
    }

    Symbol getSymbol(){
      Token<char*> symbol = getNextToken();
