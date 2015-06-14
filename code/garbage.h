//string tmpVarName ("@");
//tmpVarName += itostring(numOfTmp++);
//micode.width(9);micode.setf(ios::right);
//micode<<"MINUS";
//micode<<'0';
//micode<<expStk[point - 1].m_src;
//micode<<tmpVarName<<endl;
//++assLineNum;
//expStk[point - 1].m_src = tmpVarName;

//micode.width(9);micode.setf(ios::right);
//micode<<toUpper(expStk[point].m_src);//instru
//micode<<'~';
//micode<<'~';
//micode<<expStk[point - 1].m_src<<endl;
//++assLineNum;

//
//void PrintJump()
//{
//	micode.width(9);micode.setf(ios::right);
//	micode<<"JUMP";
//	micode.setf(ios::right);micode.width(8);micode<<'~';
//	micode.setf(ios::right);micode.width(8);micode<<'~';
//	micode.setf(ios::right);micode.width(8);micode<<"from"<<endl;
//	
//	++assLineNum;
//}
//
//void PrintJumpTo()
//{
//	micode.width(9);micode.setf(ios::right);
//	micode<<"to "<<endl;
//	
//	++assLineNum;
//}

	/*switch (expStk[point].m_type)
	{
		case PLUS : p.m_a = expStk[point].m_type - PLUS; break;
		case MINUS : p.m_a = expStk[point].m_type - PLUS;break;
		case MULTI  : p.m_a = expStk[point].m_type - PLUS;break;
		case DIV : p.m_a = expStk[point].m_type - PLUS;break;
		case LESS  : p.m_a = expStk[point].m_type - PLUS;break;
		case LESSE  : p.m_a = expStk[point].m_type - PLUS;break;
		case LARGER  : p.m_a = expStk[point].m_type - PLUS;break;
		case LARGERE : p.m_a = expStk[point].m_type - PLUS;break;
		case EQUAL  : p.m_a = expStk[point].m_type - PLUS;break;
		case NEQUAL  : p.m_a = expStk[point].m_type - PLUS;break;
		default:
			break;
	}*/
	//if(expStk[point].m_type == ASSIGN)
	//{
	//	micode.width(9);micode.setf(ios::right);
	//	micode<<"ASSIGN";
	//	micode.setf(ios::right);micode.width(8);micode<<expStk[point - 1].m_src;
	//	micode.setf(ios::right);micode.width(8);micode<<'~';
	//	micode.setf(ios::right);micode.width(8);micode<<expStk[point - 2].m_src<<endl;
	//	++assLineNum;
	//	expStk.erase(expStk.begin() + point);
	//	expStk.erase(expStk.begin() + point - 1);
	//}
	//else
	//{
		//string tmpVarName = "@";
		//tmpVarName += itostring(numOfTmp++);
		//micode.width(9);micode.setf(ios::right);

//bool IsValid(string word)
//{
//	if(word == "int" || word == "float" || word == "if" || word == "else" || word == "while" || word == "read" || word == "write" || word == "for")
//		return true;
//	else if(IsValidNumber(word))
//		return true;
//	else if(IsValidId(word))
//		return true;
//	else
//		return false;	
//}

//void VarList()
//{
//	Match(ID);
//	VarList_();
//}
//
//void VarList_()
//{
//	if(Match(COMMA))
//	{
//		Match(ID);
//		VarList_();
//	}
//	else
//		return ;
//}

//void Special2()
//{
//	if(Match(LBRACE))
//	{
//		VarList();
//		Match(RBRACE);
//		Match(SEMICOLON);
//	}
//	else if(Match(ASSIGN))
//	{
//		Expression();
//		Match(SEMICOLON);
//	}
//	else if(Match(LSQRBRA))
//	{
//		Match(INTEGER);
//		Match(ID);
//		Match(RSQRBRA);
//		Dclr3();
//	}
//
//}

//
//void Special1()
//{
//	if(Match(LBRACE))
//	{
//		DclrVarList();
//		Match(RBRACE);
//		FunBody();
//	}
//	else if(arrToken[p].type == COMMA || arrToken[p].type == LSQRBRA)
//		Dclr2();
//}


//

//
//
//while(a = getchar() != EOF)
//	{
//		if(IsSpace(a) && word.length())
//		{
//			if(IsKeyword(word))
//			{
//				cout<<word<<" is keyword"<<endl;
//			}
//			else if(IsValidId(word))
//				cout<<word<<" is id"<<endl;
//			else if(IsValidNumber(word))
//				cout<<word<<"is number"<<endl;
//			word.clear();
//			continue;
//		}
//		else if(IsSign(a) && word.length())
//		{
//			continue;
//		}
//		else//是字母或者下划线或者数字
//		{
//			word.append(1, a);
//		}


//bool resultIsFloat(Token a, Token b)
//{
//	bool left = false, right = false;
//	if(a.m_type == ID)
//		left = IdTable[a.m_src].m_type == 1;//为1说明是浮点类型
//	else
//		left = a.m_type == DECIMAL;
//
//	if(b.m_type == ID)
//		right = IdTable[b.m_src].m_type == 1;//为1说明是浮点类型
//	else
//		right = b.m_type == DECIMAL;
//
//	return left || right;
//}

//bool MismatchOfIntFloat(Token a, Token b)
//{
//	bool left = false, right = false;
//	if(a.m_type == ID)
//		left = IdTable[a.m_src].m_type == 1;//为1说明是浮点类型
//	else
//		left = a.m_type == DECIMAL;
//
//	if(b.m_type == ID)
//		right = IdTable[b.m_src].m_type == 1;//为1说明是浮点类型
//	else
//		right = b.m_type == DECIMAL;
//
//	return left ^ right;
//}


//else if(expStk[point - 1].m_type == INTEGER)
//	arrInstru.push_back(Code(LIT, atoi(expStk[point - 1].m_src.c_str())));
//else if(expStk[point - 1].m_type == DECIMAL)
//	arrInstru.push_back(Code(LIT, (float)atof(expStk[point - 1].m_src.c_str())));
//else if(expStk[point - 1].m_type == ID && IsArrId(expStk[point - 1].m_src))//数组变量
//{
//	printMidCodeArr(point - 1);
//	arrInstru.push_back(Code(OPR, 22));
//}
//else
//{
//	arrInstru.push_back(Code(LIT, IdTable[expStk[point - 1].m_src].m_index));
//	arrInstru.push_back(Code(OPR, 22));
//}


//
//struct Id : public Token
//{
//    bool Dclred;
//	virtual void tmp(){}
//};
//
//struct IntId : public Id
//{
//    int value;
//    int num;
//};
//
//struct FloatId : public Id
//{
//    float value;
//    int num;
//};
//
//struct FuncId : public Id
//{
//    char **parameters;
//    int returnType;
//};
//struct node 
//{
//	int beg, end;
//	node() : beg(), end(){}
//	node(int _beg, int _end) : beg(_beg), end(_end) {}
//};//专门用于处理表达式中的函数调用
//deque<node> dequeFun();

//	bool flag = Parser();
	//if(flag)
	//	return 0;
	/*bool hasE = false;
	if(LexAnlyz() && Parser())
	{
		hasE = true;
		system("pause");
	}
	if(!hasE)*/
