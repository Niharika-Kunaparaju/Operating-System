#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include "token.cpp"

using namespace std;
int g_line_num = 1, g_column_num = 1, fileSize = 0;int totallength = 0;
ifstream myFile;

Token<char*> getNextToken(){
      char ch;
      int length = 0;
      int index = 0, tokenStartLine = g_line_num, tokenStartColumn = g_column_num;
      char* token = new char[128];
      bool tokenStart = false, isSpace = false;
      while (myFile >> ch){
        if(ch == '\n'){
          if(myFile.tellg() != fileSize){
            g_line_num++;
            g_column_num = 1;
          }
	  ////cout << "read new line " << endl;
        }else{
	  //cout << "reading non new line :" << ch << ":" << endl;
          g_column_num++;
        }

        isSpace = (ch == ' ' || ch == '\n' || ch == '\t');
		
        if(tokenStart && isSpace){
	  //cout << "in first condition " << endl;
          token[index] = '\0';
          break;
        }else if(isSpace){
	  //cout << "in second condition " << endl;
          tokenStartLine = g_line_num;
          tokenStartColumn = g_column_num;
          continue;
        }else if(!tokenStart){ 
	  //cout << "in third condition " << endl;
          tokenStartLine = g_line_num;
          tokenStartColumn = g_column_num -1;
          tokenStart = true;
          length = 0;
        }
		
        length++;
        token[index++] = ch;
      }

	//cout << "token is at line "<< g_line_num << " column " << g_column_num << " token is " << tokenStartLine << " " << tokenStartColumn << endl;
      Token<char*> t(tokenStartLine, tokenStartColumn, token, length);
	  return t;
    }
Token<int> getCount(bool isDefinitionList){
	Token<char*> t = getNextToken();
	int length = t.getLength();
	string symbol = t.getValue();
	if (!isDefinitionList){
	if (length < 1){
		cout << "Parse Error line" <<" "<< t.getLineNumber() <<" "<< "offset" <<" "<< t.getColumnNumber() <<": "<<"NUM_EXPECTED"<< endl;
		exit(99);
	}
	}
	char *temp;
	int count = strtol(symbol.c_str(), &temp, 10);
	if (*temp != '\0'){
		cout << "Parse Error line" <<" "<< t.getLineNumber() <<" "<< "offset" <<" "<< t.getColumnNumber() <<": "<<"NUM_EXPECTED"<< endl;
		exit(99);
	}
	return Token<int>(t.getLineNumber(), t.getColumnNumber(), count, t.getLength());
}
Token<char*> getSymbol() {
	Token<char*> t = getNextToken();
	string symbol = t.getValue();
	int length = t.getLength();
	
	if (length == 0 ){
		cout << "Parse Error line" <<" "<< t.getLineNumber() <<" "<< "offset" <<" "<< t.getColumnNumber() <<": "<< "SYM_EXPECTED"<< endl;
		exit(99);
	}
	if (length > 16){
		cout << "Parse Error line" <<" "<< t.getLineNumber() <<" "<< "offset" <<" "<< t.getColumnNumber() <<": "<<"SYM_TOLONG"<< endl;
		exit(99);
	}
	char ch = symbol.at(0);
	if( !((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) ){
		cout << "Parse Error line" <<" "<< t.getLineNumber() <<" "<< "offset" <<" "<< t.getColumnNumber() <<": "<< "SYM_EXPECTED"<< endl;
		exit(99);
	}
}

Token<char*> getAddressing (){
	Token<char*> t = getNextToken();
	char* addressing = t.getValue();
	if (!(addressing[0] == 'A' || addressing[0] == 'E' || addressing[0] == 'I' || addressing[0] == 'R')){
		cout << "Parse Error line" <<" "<< t.getLineNumber() <<" "<< "offset" <<" "<< t.getColumnNumber() <<": "<< "ADDR_EXPECTED"<< endl;
		exit(99);
	}
}
	
Token<int> getInst (){
	Token<char*> t = getNextToken();
	string inst = t.getValue();
	int length = t.getLength();
	if (length==3){
		inst.append("0");
	}
	else if (length==2){
		inst.append("00");
	}
	else if (length==1){
		inst.append("000");
	}	
	
}
int getOpcode(int inst){
	return inst/1000;	
	
}
int getOperand(int inst){
	return inst%1000;	
}
	
void readDefList() {
	Token<int> count = getCount(true);//TODO getCount for deflist count will pass true.  Other places in getCount will pass false
	int value = count.getValue();
	if (value>16){
		cout << "Parse Error line" <<" "<< count.getLineNumber() <<" "<< "offset" <<" "<< count.getColumnNumber() <<": "<< "TO_MANY_DEF_IN_MODULE" << endl;
		exit(99);
	}
	for(int i = 0; i < value; i++){
		Token<char*> symbol = getSymbol();
		Token<int> count_add = getCount(false);
		//push_to_symbol_table(symbol, address);
		}
}
	
		
void readUseList() {
	Token<int> count = getCount(false);
	int value = count.getValue();
	
	if (value>16){
		cout << "Parse Error line" <<" "<< count.getLineNumber() <<" "<< "offset" <<" "<< count.getColumnNumber()<<": "<< "TO_MANY_USE_IN_MODULE" << endl;
		exit(99); 
	}
	for (int j = 0 ; j < value ; j++){
		Token<char*> symbol = getSymbol();
	}
}
	
	
Token<int> readInstList() {
	Token<int> count = getCount(false);
	int value = count.getValue();
	int modulelength = 0;
	for(int k =0; k < value; k++) {
		Token<char*> addressing = getAddressing();	
		Token<int> inst = getInst();
		int operand = getOperand(inst.getValue());
		int opcode = getOpcode(inst.getValue());		}
		
		return count;
	
	}
	
	int firstpass() {
		int count;
		int length ;
		while(!myFile.eof()) {
			readDefList();
			if(myFile.eof()){
			exit(99);
			}
			readUseList();
			Token<int> count = readInstList();
			length = count.getValue();
		totallength = totallength + length;
		if (totallength > 512){
		cout << "Parse Error line" <<" "<< count.getLineNumber() <<" "<< "offset" <<" "<< count.getColumnNumber()<<": "<< "TO_MANY_INSTR" << endl;
		exit(99);
		}
			
		//	modulenum++;
			
		//	map[modulenum]  =  modulelength;
						
		}	
	}
	
	

int main(int argc, char* argv[]) 
{
	if(argc !=2)
	{
		cout << "Number of arguments is not two" << endl;
		exit(99);
	}
	myFile.open(argv[1], fstream::in|ios::ate);
	fileSize = myFile.tellg();
	myFile.seekg(0);
	myFile.unsetf(ios_base::skipws);
	if(!myFile.good())
	{
		cout << "File is not good" <<endl;
		exit(99);
	}
	
	
	firstpass();		
		
		
		//Token t = getNextToken();
		//cout << "Token: " << t.value << " line number: " << t.linenumber << " lineoffset: " << t.columnnumber << endl;
}
