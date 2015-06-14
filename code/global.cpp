#include "global.h"

//----------------------------------------------
//全局变量声明区
map<string, int> keyTable;
vector<Token> arrToken;
map<string, Id> IdTable;
char *symbTable[] = {"if", "else", "while", "for", "int", "float", "void", "write", "read", "return", //10 keyword
					 "=", "-", "+", "-", "*", "/", //5 oper
					 "<", "<=", ">", ">=", "==", "!=",//6 relation oper
					 "(", ")", "[", "]", "{", "}",  ",",";",//8 border oper};
					 "id", "integer", "decimal",
					 "_EOF",};
int lineNum = 1;
char *buffer;
char *cur;//当前指针所指位置
int layer;//当前token所处层次

//----------------------------------------------

int Id::_count = 0;