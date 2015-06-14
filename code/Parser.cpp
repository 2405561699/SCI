#include "Parser.h"

//static int assLineNum = 1;
//int numOfTmp = 0;
int eax, ebx, ecx, edx;
bool hasError = false;
int p = 0;//指针
vector<Token> expStk;//表达式栈
extern map<string, Id> IdTable;
extern char * symbTable[];

vector<Code> arrInstru;//存放执行指令
extern fstream micode;



void Match(int i)
{
	if(arrToken[p].m_type == i)
	{
		++p;
	}
	else
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has error, "<<symbTable[i]<<" is expected,"<<" but get "<<symbTable[arrToken[p].m_type]<<endl;
	}

}

bool Parser()
{
	arrInstru.push_back(Code(ALC, -1));

	Program();
	Match(_EOF);

	arrInstru[0].m_a = Id::Get_count();
	return hasError;

}


void Program()
{
	if(arrToken[p].m_type == _EOF)
	{
		return ;
	}
	else if(arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)//Match(VOID) || Match(FLOAT) || Match(INT)函数定义
	{
		if(arrToken[p + 2].m_type == LBRACE)
		{
			DclrFun();
			Program();
			return ;
		}
	}
	if(arrToken[p].m_type == LCLYBRA || arrToken[p].m_type == READ || arrToken[p].m_type == WRITE || arrToken[p].m_type == SEMICOLON || arrToken[p].m_type == WHILE || arrToken[p].m_type == FOR || arrToken[p].m_type == IF || arrToken[p].m_type == ID || arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)
	{
		Block();
		Program();
	}
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}

}

void HandleOffset(int num)
{
	for(map<string, Id>::iterator it = IdTable.begin(); it != IdTable.end(); ++it)
		it->second.m_index -= num;
}

void ClearCase()
{
	for(map<string, Id>::iterator it = IdTable.begin(); it != IdTable.end(); )
	{
		if(!(it->second.m_src[0] == '@' || it->second.m_assLineNum != -1))
			IdTable.erase(it++);
		else
			++it;
	}

}


void DclrFun()
{
	Type();
	int beg = arrInstru.size();
	arrInstru.push_back(Code(JMP, 0));
	int record = p;
	if(!HandleIdTable(arrToken[p], arrToken[p - 1].m_type, 1, arrInstru.size() + 1))
		hasError = true;
	//数据中转（或者说：清理现场）
	map<string, Id> tmpIdTable(IdTable);
	ClearCase();
	int tmp_count = Id::Get_count();
	Id::Set_count(-tmp_count);

	Match(ID);
	
	Match(LBRACE);
	DclrVarList();
	int numOfVarList = Id::Get_count();
	
	
	HandleOffset(numOfVarList);
	Id::Set_count(-numOfVarList);
	IdTable[arrToken[record].m_src].m_numOfVar = numOfVarList;
	Match(RBRACE);

	int now = arrInstru.size();
	arrInstru.push_back(Code(ALC, 0));

	FunBody();

	arrInstru.push_back(Code(RET, numOfVarList));
	arrInstru[now].m_a = Id::Get_count();
	
	arrInstru[beg].m_a = arrInstru.size() + 1;
	//数据中转恢复，或者说：恢复现场
	Id::Set_count(-Id::Get_count());
	IdTable = tmpIdTable;
	IdTable[arrToken[record].m_src].m_numOfVar = numOfVarList;
	Id::Set_count(tmp_count);
}

void FunBody()
{
	Match(LCLYBRA);
	SubBlock();
	Match(RETURN);
	if(arrToken[p].m_type == SEMICOLON)
		Empty();
	else
	{
		Expression();
		HandleExp();
		
		Match(SEMICOLON);
	}
	Match(RCLYBRA);
}


void Block()
{
	if(arrToken[p].m_type == LCLYBRA)
	{
		Match(LCLYBRA);
		SubBlock();
		Match(RCLYBRA);
	}
	else if(arrToken[p].m_type == READ || arrToken[p].m_type == WRITE || arrToken[p].m_type == SEMICOLON || arrToken[p].m_type == WHILE || arrToken[p].m_type == FOR || arrToken[p].m_type == IF || arrToken[p].m_type == ID || arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)
		Stat();
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}

}

void SubBlock()
{
	if(arrToken[p].m_type == READ || arrToken[p].m_type == WRITE || arrToken[p].m_type == SEMICOLON || arrToken[p].m_type == WHILE || arrToken[p].m_type == FOR || arrToken[p].m_type == IF || arrToken[p].m_type == ID || arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)//
	{
		Stat();
		SubBlock();
	}
}

void Stat()
{
	if(arrToken[p].m_type == READ)
		Read();
	else if(arrToken[p].m_type == WRITE)
		Write();
	else if(arrToken[p].m_type == SEMICOLON)
		Empty();
	else if(arrToken[p].m_type == WHILE)
		While();
	else if(arrToken[p].m_type == FOR)
		For();
	else if(arrToken[p].m_type == IF)
		If();
	else if( arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)
		DclrVar();
	else if(arrToken[p].m_type == ID)
	{
		if(arrToken[p + 1].m_type == LBRACE)
			FunCall();
		else
		{
			Assign();
			Match(SEMICOLON);
		}	
	}
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void FunCall()
{
	int record = p;
	Match(ID);
	Match(LBRACE);
	VarList();
	arrInstru.push_back(Code(CAL, IdTable[arrToken[record].m_src].m_assLineNum));
	//arrInstru.push_back(Code(STO, IdTable[arrToken[record].m_src].m_index));
	Match(RBRACE);
	Match(SEMICOLON);
	expStk.push_back(arrToken[record]);
	printMidCodeFunCall(expStk.size() - 1);
	expStk.clear();
}

void DclrVar()
{
	if(arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)
	{
		Type();

		expStk.push_back(arrToken[p]);
		if(!HandleIdTable(arrToken[p], arrToken[p - 1].m_type))
			hasError = true;
		Match(ID);
		
		DclrVar2();
		expStk.clear();
	}
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}

}

void DclrVar2()
{
	int record = 0;
	if(arrToken[p].m_type == SEMICOLON)
	{
		Match(SEMICOLON);
	}
	else if(arrToken[p].m_type == LSQRBRA)
	{
		Match(LSQRBRA);
		if(arrToken[p].m_type == ID)
			Match(ID);
		else if(arrToken[p].m_type == INTEGER)
		{
			IdTable[arrToken[p - 2].m_src].m_lenght = atoi(arrToken[p].m_src.c_str());
			Id::Set_count(atoi(arrToken[p].m_src.c_str()) - 1);
			Match(INTEGER);
		}
		Match(RSQRBRA);
		DclrVar3();
	}
	else if(arrToken[p].m_type == ASSIGN)
	{
		record = p;
		Match(ASSIGN);
		Expression();
		expStk.push_back(arrToken[record]);
        HandleExp();
		Match(SEMICOLON);
	}
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void DclrVar3()
{
	if(arrToken[p].m_type == SEMICOLON)
		Match(SEMICOLON);
	else if(arrToken[p].m_type == ASSIGN)
	{
		Match(ASSIGN);
		Match(LCLYBRA);
		NumList();
		Match(RCLYBRA);
		Match(SEMICOLON);
	}
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void Read()
{
	int record = p;

	Match(READ);
	NormId();
	expStk.push_back(arrToken[record]);
	HandleExp();
	Match(SEMICOLON);
}

void Write()
{
	int record = p;
	Match(WRITE);
	Expression();
	expStk.push_back(arrToken[record]);
	HandleExp();
	Match(SEMICOLON);
}

void Empty()
{
	Match(SEMICOLON);
}



void Expression()
{
	Item();
	Expression_();
}

void Expression_()
{
	if(arrToken[p].m_type == PLUS || arrToken[p].m_type == MINUS)
	{
		int record = p;

		if(arrToken[p].m_type == PLUS)
			Match(PLUS);
		else if(arrToken[p].m_type == MINUS)
			Match(MINUS);
		Item();

		expStk.push_back(arrToken[record]);
        
		Expression_();
	}
	
}

void Item()
{
	Factor();
	Item_();
}

void Item_()
{
	if(arrToken[p].m_type == MULTI || arrToken[p].m_type == DIV)
	{
		int record = p;
		if(arrToken[p].m_type == MULTI)
			Match(MULTI);
		else if(arrToken[p].m_type == DIV)
			Match(DIV);
		Factor();
		
		expStk.push_back(arrToken[record]);
		Item_();
	}
	
}

void Factor()
{
	if(arrToken[p].m_type == DECIMAL || arrToken[p].m_type == INTEGER)
		Num();
	else if(arrToken[p].m_type == ID)
		SupId();
	else if(arrToken[p].m_type == LBRACE)
	{
		Match(LBRACE);
		Expression();
		Match(RBRACE);
	}
	else if(arrToken[p].m_type == SMINUS)
	{
		int record = p;
		Match(SMINUS);
		Factor_();
		expStk.push_back(arrToken[record]);
	}
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void Factor_()
{
	if(arrToken[p].m_type == DECIMAL || arrToken[p].m_type == INTEGER)
		Num();
	else if(arrToken[p].m_type == ID)
		SupId();
	else if(arrToken[p].m_type == LBRACE)
	{
		Match(LBRACE);
		Expression();
		Match(RBRACE);
	}
}

void While()
{
	Match(WHILE);
	Match(LBRACE);
	int startOfLoop = arrInstru.size() + 1;
	BooleanExp();
	int endOfLoop = arrInstru.size();
	arrInstru.push_back(Code(JPC, 0));
	Match(RBRACE);
	Block();
	arrInstru.push_back(Code(JMP, startOfLoop));
	arrInstru[endOfLoop].m_a = arrInstru.size() + 1;
}

void For()
{
	Match(FOR);
	Match(LBRACE);
	if(arrToken[p].m_type != SEMICOLON)
		Assign();
	Match(SEMICOLON);
	int startOfLoop = arrInstru.size() + 1;
	BooleanExp();
	int endOfLoop = arrInstru.size();
	arrInstru.push_back(Code(JPC, 0));
	
	Match(SEMICOLON);

	//第三个表达式需要特殊处理，平行向后移动
	bool flag = false;
	int beg, end;
	vector<Token> tmpCon;
	if(arrToken[p].m_type != RBRACE)
	{
		for(end = p; end < arrToken.size() && arrToken[end].m_type != RBRACE; ++end);
		flag = true;
		beg = p;
		tmpCon.assign(arrToken.begin() + beg, arrToken.begin() + end);
		arrToken.erase(arrToken.begin() + beg, arrToken.begin() + end);
	}
	Match(RBRACE);
	Block();
	if(flag)
	{
		arrToken.insert(arrToken.begin() + p, tmpCon.begin(), tmpCon.end());
		Assign();
	}
	arrInstru.push_back(Code(JMP, startOfLoop));
	arrInstru[endOfLoop].m_a = arrInstru.size() + 1;
}

void Assign()
{
	NormId();
	int record = p;
	Match(ASSIGN);
	Expression();
	expStk.push_back(arrToken[record]);
	HandleExp();
}


void If()
{
	Match(IF);
	Match(LBRACE);
	BooleanExp();
	Match(RBRACE);
//	PrintJump();
	int start = arrInstru.size();
	arrInstru.push_back(Code(JPC, 0));
	
	Block();
//	PrintJumpTo();
	If_(start);
}

void If_(int start)
{
	if(arrToken[p].m_type == ELSE)
	{
		arrInstru[start].m_a = arrInstru.size() + 2;
		Match(ELSE);
		int now = arrInstru.size();
		arrInstru.push_back(Code(JMP, 0));
		
		
		Block();
		arrInstru[now].m_a = arrInstru.size() + 1;
	}
	else
		arrInstru[start].m_a = arrInstru.size() + 1;

}

void SupId()
{
	int record = p;
	Match(ID);
	SupId1();
	expStk.push_back(arrToken[record]);
}

void SupId1()
{
	int record = p;
	if(arrToken[p].m_type == LSQRBRA)
	{
		Match(LSQRBRA);
		expStk.push_back(arrToken[p]);
		if(arrToken[p].m_type == INTEGER)
			Match(INTEGER);
		else if(arrToken[p].m_type == ID)
			Match(ID);
		Match(RSQRBRA);
	}
	else if(arrToken[p].m_type == LBRACE)
	{
		/*if(IdTable[arrToken[record - 1].m_src].m_type == 0)
		{
			expStk.rbegin()->m_src = "@0";
		}
		else if(IdTable[arrToken[record - 1].m_src].m_type == 1)
		{
			expStk.rbegin()->m_src = "@1";
		}*/

		Match(LBRACE);
		VarList();
		//arrInstru.push_back(Code(CAL, IdTable[arrToken[record - 1].m_src].m_assLineNum));
		Match(RBRACE);
	}
}

void NormId()
{
	int record = p;
	Match(ID);
	NormId1();
	expStk.push_back(arrToken[record]);
}

void NormId1()
{
	if(arrToken[p].m_type == LSQRBRA)
	{
		Match(LSQRBRA);
		expStk.push_back(arrToken[p]);
		if(arrToken[p].m_type == INTEGER)
			Match(INTEGER);
		else if(arrToken[p].m_type == ID)
			Match(ID);
		Match(RSQRBRA);
	}
}

void BooleanExp()
{
	Expression();
	int record = p;
	RltOp();
	Expression();
	expStk.push_back(arrToken[record]);
	HandleExp();
}

void RltOp()
{
	if(arrToken[p].m_type == LESS)
		Match(LESS);
	else if(arrToken[p].m_type == LESSE)
		Match(LESSE);
	else if(arrToken[p].m_type == EQUAL)
		Match(EQUAL);
	else if(arrToken[p].m_type == LARGER)
		Match(LARGER);
	else if(arrToken[p].m_type == LARGERE)
		Match(LARGERE);
	else if(arrToken[p].m_type == NEQUAL)
		Match(NEQUAL);
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void Type()
{
	if(arrToken[p].m_type == INT)
		Match(INT);
	else if(arrToken[p].m_type == FLOAT)
		Match(FLOAT);
	else if(arrToken[p].m_type == VOID)
		Match(VOID);
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void Num()
{
	if(arrToken[p].m_type == INTEGER)
	{
		expStk.push_back(arrToken[p]);
		Match(INTEGER);

	}
	else if(arrToken[p].m_type == DECIMAL)
	{
			expStk.push_back(arrToken[p]);
			Match(DECIMAL);
	}

		
	else //error
	{
		hasError = true;
		cout<<"line "<<arrToken[p].m_lineNum<<" has unknown error"<<endl;
	}
}

void DclrVarList()
{
	if(arrToken[p].m_type == VOID || arrToken[p].m_type == FLOAT || arrToken[p].m_type == INT)
	{
		Type();
		if(!HandleIdTable(arrToken[p], arrToken[p - 1].m_type))
			hasError = true;
		Match(ID);
		DclrVarList1(arrToken[p - 1].m_type);
	}
}

void DclrVarList1(int type)
{
	if(arrToken[p].m_type == COMMA)
		DclrVarList2(type);
}

void DclrVarList2(int type)
{
	if(arrToken[p].m_type == COMMA)
	{
		Match(COMMA);
		Type();
		if(!HandleIdTable(arrToken[p], type))
			hasError = true;
		Match(ID);
		DclrVarList2(type);
	}
}

void VarList()
{
	if(arrToken[p].m_type == ID)
	{
		//arrInstru.push_back(Code(LOD, IdTable[arrToken[p].m_src].m_index));
		expStk.push_back(arrToken[p]);
		Match(ID);
		VarList1();
	}
	else if(arrToken[p].m_type == INTEGER)
	{
		//arrInstru.push_back(Code(LIT, atoi(arrToken[p].m_src.c_str())));
		expStk.push_back(arrToken[p]);
		Match(INTEGER);
		VarList1();
	}
	else if(arrToken[p].m_type == DECIMAL)
	{
		//arrInstru.push_back(Code(LIT, (float)atof(arrToken[p].m_src.c_str())));
		expStk.push_back(arrToken[p]);
		Match(DECIMAL);
		VarList1();
	}
}

void VarList1()
{
	if(arrToken[p].m_type == COMMA)
	{
		VarList2();
	}
}
void VarList2()
{
	if(arrToken[p].m_type == COMMA)
	{
		Match(COMMA);

		if(arrToken[p].m_type == ID)
		{
			//arrInstru.push_back(Code(LOD, IdTable[arrToken[p].m_src].m_index));
			expStk.push_back(arrToken[p]);
			Match(ID);
		}
		else if(arrToken[p].m_type == INTEGER)
		{
			//arrInstru.push_back(Code(LIT, atoi(arrToken[p].m_src.c_str())));
			expStk.push_back(arrToken[p]);
			Match(INTEGER);
		}
		else if(arrToken[p].m_type == DECIMAL)
		{
			//arrInstru.push_back(Code(LIT, (float)atof(arrToken[p].m_src.c_str())));
			expStk.push_back(arrToken[p]);
			Match(DECIMAL);
		}

		VarList2();
	}
}

void NumList()
{
	if(arrToken[p].m_type == INTEGER || arrToken[p].m_type == DECIMAL)
	{
		Num();
		NumList1();
	}
}

void NumList1()
{
	if(arrToken[p].m_type == COMMA)
		NumList2();
}
void NumList2()
{
	if(arrToken[p].m_type == COMMA)
	{
		Match(COMMA);
		Num();
		NumList2();
	}
}
string toUpper(string src)
{
	string result(src);
	int len = result.length();
	for(int i = 0; i <len; ++i)
		if('a' <= result[i] && result[i] <= 'z')
			result[i] += 'A' - 'a';
	return result;
}

string itostring(int i)
{
	bool flag = i >= 0;
	i = flag ? i : -i;
	string result;
	do
	{
		char a = i % 10 + '0';
		result = a + result;
		i /= 10;
	}while(i);
	if(!flag)
		result = '-' + result;
	return result;
}

inline bool IsFunId(string id)
{
	return IdTable[id].m_assLineNum != -1;
}

inline bool IsArrId(string id)
{
	return IdTable[id].m_lenght != 1;
}

bool IsSingleOper(int type)
{
	return INT <= type && type <= RETURN || type == SMINUS;
}

void genInstru(int point)
{
	if(expStk[point].m_type == ID && IsArrId(expStk[point].m_src))
	{
		printMidCodeArr(point);
		arrInstru.push_back(Code(OPR, 22));
	}
	else if(expStk[point].m_type == ID && IsFunId(expStk[point].m_src))
	{
		printMidCodeFunCall(point);
	}
	else if(expStk[point].m_type == ID)
	{
		arrInstru.push_back(Code(LIT, IdTable[expStk[point].m_src].m_index));
		arrInstru.push_back(Code(OPR, 22));
	}
	else if(expStk[point].m_type == DECIMAL)
	{
		arrInstru.push_back(Code(LIT, (float)atof(expStk[point].m_src.c_str())));
	}
	else if(expStk[point].m_type == INTEGER)
	{
		arrInstru.push_back(Code(LIT, atoi(expStk[point].m_src.c_str())));
	}

}

bool IsFloat(Token a)
{
	bool result = false;
	if(a.m_type == ID)
		result = IdTable[a.m_src].m_type == 1;//为1说明是浮点类型
	else
		result = a.m_type == DECIMAL;
	return result;
}

void printMidCode(int point)
{
	int firstBackStep, secBackStep;
	int first = getFirstId(point - 1);
	firstBackStep = BackStep(first), secBackStep = BackStep(point - 1);
	bool left = IsFloat(expStk[first]), right = IsFloat(expStk[point - 1]);
	bool intOPfloat = left ^ right;
	if(PLUS <= expStk[point].m_type && expStk[point].m_type <= NEQUAL)
	{
		
		if(expStk[first].m_src[0] != '@')//第一个操作数
		{
			genInstru(first);
		}
		if(intOPfloat && !left)//化简之后是：!a && b
			arrInstru.push_back(Code(ITF, 0));
		
		if(expStk[point - 1 - firstBackStep].m_src[0] != '@')//第二个操作数
		{
			genInstru(point - 1 - firstBackStep);
		}
		if(intOPfloat && !right)//化简之后是：!a && b
			arrInstru.push_back(Code(ITF, 0));
			

		if(left || right)
		{
			expStk[first].m_src = "@1";
			expStk[first].m_type = ID;
			arrInstru.push_back(Code(OPR, expStk[point - secBackStep - firstBackStep].m_type));
		}
		else
		{
			expStk[first].m_src = "@0";
			expStk[first].m_type = ID;
			arrInstru.push_back(Code(OPR, expStk[point - secBackStep - firstBackStep].m_type - PLUS + 1));
		}
	}
	else if(expStk[point].m_type == ASSIGN)
	{
		if(expStk[point - 1].m_src[0] != '@')//临时变量
			if(expStk[point - 1].m_type == ID && IsArrId(expStk[point - 1].m_src))//五个判断用于 赋值的前导条件
			{
				printMidCodeArr(point - 1);
				arrInstru.push_back(Code(OPR, 22));
			}
			else if(expStk[point - 1].m_type == ID && IsFunId(expStk[point - 1].m_src))
			{
				printMidCodeFunCall(point - 1);
			}
			else if(expStk[point - 1].m_type == ID)
			{
				arrInstru.push_back(Code(LIT, IdTable[expStk[point - 1].m_src].m_index));
				arrInstru.push_back(Code(OPR, 22));
			}
			else if(expStk[point - 1].m_type == DECIMAL)
				arrInstru.push_back(Code(LIT, (float)atof(expStk[point - 1].m_src.c_str())));
			else if(expStk[point - 1].m_type == INTEGER)
				arrInstru.push_back(Code(LIT, atoi(expStk[point - 1].m_src.c_str())));

		if(intOPfloat)//不同类型之间的赋值操作， 必须先类型转换
			if(left)
				arrInstru.push_back(Code(ITF, 0));
			else
				arrInstru.push_back(Code(FTI, 0));

		if(expStk[first].m_type == ID && IsArrId(expStk[first].m_src))//三个判断用于 赋值的前导条件
		{
			printMidCodeArr(first);
			first -= 1;
		}
		else
			arrInstru.push_back(Code(LIT, IdTable[expStk[first].m_src].m_index));
		expStk[first].m_src = "@";
		
		arrInstru.push_back(Code(OPR, 23));
		
	}
	int tmp = point - firstBackStep - secBackStep;
	expStk.erase(expStk.begin() + tmp);
	expStk.erase(expStk.begin() + tmp - 1);
	
}


void printMidCode_SingleOper(int point)
{
	int backSteps = BackStep(point - 1);
	bool flag = IsFloat(expStk[point - 1]);
	if(expStk[point].m_type == SMINUS)
	{
		bool intORfloat = IsFloat(expStk[point - 1]);
		if(expStk[point - 1].m_src[0] == '@')
			;//do nothing
		else 
			genInstru(point - 1);
		arrInstru.push_back(Code(OPR, intORfloat ? 11 : 0));//指令0是栈顶整形取反、11浮点
		if(flag)
			expStk[point - 1].m_src = "@1";
		else
			expStk[point - 1].m_src = "@0";
		expStk.erase(expStk.begin() + point - backSteps);
	}
	else if(expStk[point].m_type == WRITE)
	{

		if(expStk[point - 1].m_src[0] == '@')//是表达式运算后的结果
			arrInstru.push_back(Code(WRT, expStk[point - 1].m_src[1] - '0'));//指令12是栈顶输出
		else//不是表达式运算后的结果，例如 ： write a； write 1.123;
		{
			if(expStk[point - 1].m_type == INTEGER)
			{
				arrInstru.push_back(Code(LIT, atoi(expStk[point - 1].m_src.c_str())));
				arrInstru.push_back(Code(WRT, expStk[point - 1].m_src[1] - '0'));
			}
			else if(expStk[point - 1].m_type == DECIMAL)
			{
				arrInstru.push_back(Code(LIT, (float)atof(expStk[point - 1].m_src.c_str())));
				arrInstru.push_back(Code(WRT, expStk[point - 1].m_src[1] - '0'));
			}
			else if(expStk[point - 1].m_type == ID && IsArrId(expStk[point - 1].m_src))//数组变量
			{
				printMidCodeArr(point - 1);
				arrInstru.push_back(Code(OPR, 22));
				arrInstru.push_back(Code(WRT, IdTable[expStk[point - backSteps - 1].m_src].m_type));
			}
			else if(expStk[point - 1].m_type == ID && IsFunId(expStk[point - 1].m_src))//函数变量
			{
				printMidCodeFunCall(point - 1);
				arrInstru.push_back(Code(WRT, IdTable[expStk[point - backSteps - 1].m_src].m_type));
			}
			else //普通变量
			{
				arrInstru.push_back(Code(LIT, IdTable[expStk[point - 1].m_src].m_index));
				arrInstru.push_back(Code(OPR, 22));
				arrInstru.push_back(Code(WRT, IdTable[expStk[point - 1].m_src].m_type));
			}


		}
		expStk.erase(expStk.begin() + point - backSteps - 1);
	}
	else if(expStk[point].m_type == READ)
	{
		bool intORfloat = IsFloat(expStk[point - 1]);
		//arrInstru.insert(arrInstru.end() - 3, Code(IPT, intORfloat));
		arrInstru.push_back(Code(IPT, intORfloat));
		if(expStk[point - 1].m_type == ID && IsArrId(expStk[point - 1].m_src))
			printMidCodeArr(point - 1);
		else if(expStk[point - 1].m_type == ID)
			arrInstru.push_back(Code(LIT, IdTable[expStk[point - 1].m_src].m_index));
		
		arrInstru.push_back(Code(OPR, 23));
		expStk.erase(expStk.begin() + point - backSteps );
		expStk[point - backSteps - 1].m_src = "@";
	}
	
	
}

void printMidCodeFunCall(int point)
{
	int num = IdTable[expStk[point].m_src].m_numOfVar;
	for(int i = num; i > 0; --i)
	{
		genInstru(point - num);
		expStk.erase(expStk.begin() + point - num);
	}
	arrInstru.push_back(Code(CAL, IdTable[expStk[point - num].m_src].m_assLineNum));
	if(IdTable[expStk[point - num].m_src].m_type == 0)
		expStk[point - num].m_src = "@0";
	else if(IdTable[expStk[point - num].m_src].m_type == 1)
		expStk[point - num].m_src = "@1";

}

void printMidCodeArr(int point)
{
	arrInstru.push_back(Code(LIT, IdTable[expStk[point].m_src].m_index));
	if(expStk[point - 1].m_type == ID)
	{
		arrInstru.push_back(Code(LIT, IdTable[expStk[point - 1].m_src].m_index));
		arrInstru.push_back(Code(OPR, 22));
	}
	else if(expStk[point - 1].m_type == INTEGER)
	{
		arrInstru.push_back(Code(LIT, atoi(expStk[point - 1].m_src.c_str())));
	}
	arrInstru.push_back(Code(OPR, 1));//1为整形加
	if(IdTable[expStk[point].m_src].m_type)
		expStk[point].m_src = "@1";
	else
		expStk[point].m_src = "@0";
	expStk.erase(expStk.begin() + point - 1);

}





void HandleExp()
{
	while(expStk.size() != 1)//只剩下一个数时，说明被处理得只剩一个最终结果了
	{
        int point = 0;
		for(; !(WRITE <= expStk[point].m_type  && expStk[point].m_type <= NEQUAL/* || expStk[point].m_type == ID && IsFunId(expStk[point].m_src)*//* || expStk[point].m_type == ID && IsArrId(expStk[point].m_src)*/); ++point);//取得栈中第一个符号位置, 或者函数调用位置

        
		
        if(IsSingleOper(expStk[point].m_type))//单目运算符
		{
			printMidCode_SingleOper(point);
		}
		else if(PLUS <= expStk[point].m_type && expStk[point].m_type <= NEQUAL || expStk[point].m_type == ASSIGN)//双目运算符
		{
			printMidCode(point);
		}
		//else if(IsFunId(expStk[point].m_src))//函数调用
		//{
		//	printMidCodeFunCall(point);
		//}
		//else//数组id处理
		//{
		//	printMidCodeArr(point);
		//}

	}
	if(expStk[0].m_src[0] != '@')
	{
		genInstru(0);
	}

	expStk.clear();
}

bool HandleIdTable(Token ref, int type, int num, int assLineNum)
{
	if(IdTable.find(ref.m_src) != IdTable.end())
	{
		cout<<"line "<<arrToken[p].m_lineNum<<" has error, "<<ref.m_src<<" duplicate defines!"<<endl;
		return false;
	}
	
	Id id(ref.m_src, Id::Get_count(), type - 4, num, assLineNum);
	IdTable[ref.m_src] = id;
	Id::Set_count(num);
	return true;

}


int getFirstId(int point)
{
	if(expStk[point].m_type == ID && IsArrId(expStk[point].m_src))
	{
		return point - 2;
	}
	else if(expStk[point].m_type == ID && IsFunId(expStk[point].m_src))
	{
		int tmp = IdTable[expStk[point].m_src].m_numOfVar;
		return point - tmp - 1;
	}
	else if(expStk[point].m_type == ID || expStk[point].m_type == DECIMAL || expStk[point].m_type == INTEGER)
	{
		return point - 1;
	}
}


int BackStep(int point)
{
	if(expStk[point].m_type == ID && IsArrId(expStk[point].m_src))
	{
		return 1;
	}
	else if(expStk[point].m_type == ID && IsFunId(expStk[point].m_src))
	{
		return IdTable[expStk[point].m_src].m_numOfVar;
	}
	else if(expStk[point].m_type == ID || expStk[point].m_type == DECIMAL || expStk[point].m_type == INTEGER)
	{
		return 0;
	}
}
