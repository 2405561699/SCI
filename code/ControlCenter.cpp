#include "LexAnlyz.h"
#include "global.h"
#include "Parser.h"
#include "Assist.h"


#include <fstream>
#include <iostream>
using namespace std;

void Initial(string fileName);
int CountLineNum(char *buffer, int length);
extern char *buffer;
extern char *cur;
extern int lineNum;
extern map<string, int> keyTable;
extern char *symbTable[];
extern map<string, Id> IdTable;

//语义分析   简单分析
//中间代码   模拟函数栈式结构
//代码执行


int main(int argc, char** argv)//argv用于以后传递文件名
{
//	freopen("in.txt","r", stdin);//argv用于以后传递文件名
	//get original file
	char *name;
	if(argc != 1)
		name = strtok(argv[1], ".");
	else
		name = "in";
	
	Initial(name);
	if(LexAnlyz())
	{
		system("pause");
		return 0;
	}
	
	if(Parser())
	{
		system("pause");
		return 0;
	}


	Assist(name);
}



void Initial(string fileName)
{
	
	ifstream is(fileName + ".txt", ifstream::in);
	
	// get length of file:
	is.seekg (0, ios::end);
	int length = is.tellg();
	is.seekg (0, ios::beg);

	// allocate memory:	
	buffer = new char [length];

	// read data as a block:
	is.read (buffer,length);
	is.close();

	length -= CountLineNum(buffer, length);

	buffer[length] = '\0';

	cur = buffer;

	//initial 
	keyTable["if"] = 0;
    keyTable["else"] = 1;
    keyTable["while"] = 2;
    keyTable["for"] = 3;
    keyTable["int"] = 4;
    keyTable["float"] = 5;
    keyTable["void"] = 6;
    keyTable["write"] = 7;
    keyTable["read"] = 8;
    keyTable["return"] = 9;
    keyTable["="] = 10;

    keyTable["+"] = 12;
    keyTable["-"] = 13;
    keyTable["*"] = 14;
	keyTable["/"] = 15;
	keyTable["<"] = 16;
	keyTable["<="] = 17;
	keyTable[">"] = 18;
	keyTable[">="] = 19;
	keyTable["=="] = 20;
	keyTable["!="] = 21;
	keyTable["("] = 22;
	keyTable[")"] = 23;
	keyTable["["] = 24;
	keyTable["]"] = 25;
	keyTable["{"] = 26;
	keyTable["}"] = 27;
	keyTable[","] = 28;
	keyTable[";"] = 29;
	keyTable["id"] = 30;
	keyTable["integer"] = 31;
	keyTable["decimal"] = 32;
	keyTable["_EOF"] = 33;


	//Id::_count = 0;
	IdTable["@0"] = Id("@0", -1, 0);
	IdTable["@1"] = Id("@1", -1, 1);
}

int CountLineNum(char *buffer, int length)
{
	int _count = 0;
	for(int i = 0; i < length; ++i)
		if(buffer[i] == '\n')
			++_count;
	return _count;
}

