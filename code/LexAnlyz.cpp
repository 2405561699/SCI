#include "LexAnlyz.h"
#include "global.h"



bool isLexError = false;

bool IsValidNumber(string word)
{
	regex floatPattern("\\.[0-9]+|[0-9]+\\.[0-9]+|[0-9]+\\.");
	regex intPattern("0|[1-9][0-9]*");
	return regex_match(word, floatPattern) || regex_match(word, intPattern);
}

bool IsInteger(string word)
{
	regex intPattern("0|[1-9][0-9]*");
	return regex_match(word, intPattern);
}

bool IsDecimal(string word)
{
	regex floatPattern("\\.[0-9]+|[0-9]+\\.[0-9]+|[0-9]+\\.");
	return regex_match(word, floatPattern);
}

bool IsValidId(string word)
{
	regex pat("[a-zA-Z_][a-zA-Z0-9_]*");
	return regex_match(word, pat);
}

inline bool IsSign(char a)
{
	return a == '+' || a == '-' || a == '*' || a == '/' || a == '<' || a == '>' || a == '!' || a == '=' || a == '(' || a == ')' || a == '[' || a == ']' || a == '{' || a == '}' || a == ';' || a == ',';
}

bool SkipComment()
{
	if(*cur == '/')
	{
		if(*(cur + 1) == '/')
		{
			while(*cur++ != '\n');//读字符直到换行
			lineNum++;
			return true;
		}
		else if(*(cur + 1) == '*')
		{
			cur += 2;
			char _old = *cur, _new = *(++cur);
			for(; _old != '*' && _new != '/';)
			{
				if(_old == '\n')
					++lineNum;
				_old = _new;
				_new = *(++cur);
			}
			++cur;
			return true;
		}
		
	}
	return false;
}


inline bool IsSpace(char a)
{
	if(a == '\n')
		++lineNum;
	return a == ' ' || a == '\t' || a == '\n' || a == '\r';
}

inline bool IsCutPoint(char a)
{
	return IsSign(a) || IsSpace(a);
}

bool IsKeyword(string word)
{
	return word == "int" || word == "float" || word == "if" || word == "else" || word == "while" || word == "read" || word == "write" || word == "for";
}

string GetOper()
{
	string result;
	if(IsCompoundOper())
	{
		result.append(cur, 2);
		cur += 2;
	}
	else
		result.append(1, *cur++);
	
	return result;
}


bool IsCompoundOper()
{
	if(*cur == '=' && *(cur + 1) == '=' || *cur == '<' && *(cur + 1) == '=' || *cur == '>' && *(cur + 1) == '=' || *cur == '!' && *(cur + 1) == '=')
		return true;
	return false;
}


string GetWord()
{
	string result;
	while(*cur != '\0' && !IsSign(*cur) && !IsSpace(*cur))//数字(小数点) 下划线 字母 特殊字符
		result.append(1, *cur++);

	return result;
}

bool LexAnlyz()
{
//	char a, type;//type e 空格， s 符号, i 标识符id 数字 
	string word;
	for(;*cur != '\0';)
	{
		if(IsSpace(*cur))
			++cur;
		else if(IsSign(*cur))
		{
			if(SkipComment())//处理注释语句
				continue;
			/*else
				--cur;*/
			word = GetOper();
			AddToArrToken(word);
			word.clear();
		}
		else
		{
			word = GetWord();
			/*if(IsKeyword(word))
				AddToArrToken(word);
			else if(IsValidId(word))
				AddToArrToken(word);
			else if(IsValidNumber(word))
				AddToArrToken(word);
			else
			{
				cout<<word<<" is invalid"<<endl;
				cout<<"error in line "<<lineNum<<endl;
			}	*/
			if(IsKeyword(word) || IsValidId(word) || IsValidNumber(word))
				AddToArrToken(word);
			else
			{
				isLexError = true;
				cout<<word<<" is invalid"<<endl;
				cout<<"error in line "<<lineNum<<endl;
			}
			word.clear();
		}
	}
	//尾部增加人为规定的eof
	Token *tmp = new Token("_EOF", _EOF, lineNum);
	arrToken.push_back(*tmp);
	Unknown();
	return isLexError;
}

void AddToArrToken(string word)
{
	Token *tmp = new Token();
	tmp->m_src = word;
	map<string, int>::iterator it;
	it = keyTable.find(word);
	if(it != keyTable.end())
		tmp->m_type = keyTable[word];
	else if(IsValidId(word))
		tmp->m_type = ID;
	else if(IsDecimal(word))
		tmp->m_type = DECIMAL;
	else if(IsInteger(word))
		tmp->m_type = INTEGER;

	tmp->m_lineNum = lineNum;
	arrToken.push_back(*tmp);
}

bool IsOper(int a)
{
	return a == MINUS || a == PLUS;// || a == MULTI || a == DIV;
}
void Handle(Token &a, Token &b)
{
	if(b.m_type == MINUS)
	{
		if(a.m_type == PLUS)
			a.m_type = MINUS;
		else
			a.m_type = PLUS;
	}

}

bool IsSinglePM(int i)
{
	return arrToken[i - 1].m_type == LBRACE || arrToken[i - 1].m_type == ASSIGN || arrToken[i - 1].m_type == MULTI || arrToken[i - 1].m_type == DIV || arrToken[i - 1].m_type == RETURN || arrToken[i - 1].m_type == EQUAL || arrToken[i - 1].m_type == NEQUAL || arrToken[i - 1].m_type == LESS || arrToken[i - 1].m_type == LESSE || arrToken[i - 1].m_type == LARGER || arrToken[i - 1].m_type == LARGERE;
}

void Unknown()//这个函数的作用是：化简多重连续加减号，删除单目加号，保留单目减号。我懒得想名字了，起名unknown
{
	for(int i = 0; i < arrToken.size() - 1; )
	{
		if(IsOper(arrToken[i].m_type))
		{
			if(IsOper(arrToken[i + 1].m_type))//2个或多个
			{
				Handle(arrToken[i], arrToken[i + 1]);
				arrToken.erase(arrToken.begin() + i + 1);
			}
			else//1个
			{
				if(arrToken[i].m_type == PLUS)
				{
					if(IsSinglePM(i))//丑，不敢说这代码是我写的
					{
						arrToken.erase(arrToken.begin() + i);//删掉单目加法
					}
					arrToken[i].m_src = "+";
				}
				else
				{
					if(IsSinglePM(i))
					{
						arrToken[i].m_type = SMINUS;//保留单目减法
						
					}
	     			arrToken[i].m_src = "-";
				}
				++i;
			}
		}
		else
			++i;
	}

}



