#ifndef LEXANLYZ_H
#define LEXANLYZ_H

#include "global.h"


//----------------------------------------------
//����������
bool IsValidNumber(string key);
bool IsValidId(string key);
bool IsValid(string word);
bool LexAnlyz();
bool IsCompoundOper();
void AddToArrToken(string word);

void Unknown();
//----------------------------------------------

//map<string, string> IdTable;
extern int lineNum ;
extern char *cur;
extern vector<Token> arrToken;
extern char *buffer;
extern char *cur;//��ǰָ����ָλ��
extern map<string, int> keyTable;
#endif
