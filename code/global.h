
#ifndef GLOBAL_H
#define GLOBAL_H
//设计细节：没有全局变量
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <iomanip>
#include <deque>

#define IF 0
#define ELSE 1
#define WHILE 2
#define FOR 3
#define INT 4
#define FLOAT 5
#define VOID 6
#define WRITE 7
#define READ 8
#define RETURN 9
#define ASSIGN 10
#define SMINUS 11
#define PLUS 12
#define MINUS 13
#define MULTI 14
#define DIV 15
#define LESS 16
#define LESSE 17
#define LARGER 18
#define LARGERE 19
#define EQUAL 20
#define NEQUAL 21
#define LBRACE 22
#define RBRACE 23
#define LSQRBRA 24
#define RSQRBRA 25
#define LCLYBRA 26
#define RCLYBRA 27
#define COMMA 28
#define SEMICOLON 29
#define ID 30
#define INTEGER 31
#define DECIMAL 32
#define _EOF 33

using namespace std;


struct Token
{
	string m_src;//原始字符串
	int m_type;//类型
	int m_lineNum;
	//int priorityOfFather;//父亲在他的兄弟中的次序
	//int priorityOfBro;//在兄弟层次中的次序
	//int iLayer;//自己的层次
	//bool assigned;//是否被赋值
	//int i_value;//整形值
	//double f_value;//浮点值
	Token() : m_src(), m_type(-1), m_lineNum(-1){}
	Token(string src, int type, int lineNum) : m_src(src), m_type(type), m_lineNum(lineNum){}
};

struct Id
{
	string m_src;
	int m_index;
	int m_type;
	int m_assLineNum;
	int m_numOfVar;
	int m_lenght;//申请的空间长度
	static int _count;
	Id() : m_src(), m_index(-1), m_type(-1), m_assLineNum(-1), m_numOfVar(-1), m_lenght(-1){}
	Id(string src, int index, int type, int length = 1, int assLineNum = -1, int numOfVar = -1) : m_src(src), m_index(index), m_type(type), m_lenght(length), m_assLineNum(assLineNum), m_numOfVar(numOfVar){}//type 0 1 2 int float void

	static int Get_count(){return _count;}
	static void Set_count(int num){_count += num;}
};


enum INSTRU{ LIT, CAL, RET, ALC, JMP, JPC, OPR, WRT, IPT, FTI, ITF };
struct Code
{
	INSTRU m_instru;
	int m_a;
	float m_fvalue;
	Code() : m_instru(LIT), m_a(-1), m_fvalue(0.0){}
	Code(INSTRU instru, float fvalue = 0.0) : m_instru(instru), m_fvalue(fvalue), m_a(0){}
	Code(INSTRU instru, int a = 0) : m_instru(instru), m_fvalue(0.0), m_a(a){}
	Code(INSTRU instru, int a, float fvalue) : m_instru(instru), m_fvalue(fvalue), m_a(a){}
};


#endif

