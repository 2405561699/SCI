#include "global.h"
//中间代码生成
void HandleExp();
/*-----------------------------------------

Program        -> Block Program  | DclrFun Program | EOF                                              ***
DclrFun        -> Type ID '(' DclrVarList ')' 'FunBody
FunBody        -> '{' SubBlock 'return' (Expression ';' | Empty) '}'
DclrVarList    -> Type ID DclrVarList1 | @
DclrVarList1   -> DclrVarList2 | @
DclrVarList2   -> ',' Type ID DclrVarList2 | @ 



Block          -> '{' SubBlock '}' | Stat
SubBlock       -> Stat SubBlock | @



VarList        -> (ID | num) VarList1 | @
VarList1       -> VarList2  | @
VarList2       -> ',' (ID | num) VarList2 | @

NumList        -> Num NumList1 | @
NumList1       -> NumList2 | @
NumList2       -> ',' Num NumList2 | @

Stat           -> Read | Write | Empty | While | For | If  | DclrVar | FunCall | Assign                    ***
DclrVar        -> Type ID DclrVar2
DclrVar2       -> ';' | '=' Expression ';' | '[' INTEGER | ID ']' DclrVar3
DclrVar3       -> ';' | '=' {' NumList '}' ';'


Read           -> 'read' SupID ';'
Write          -> 'write' Expression ';'
Empty          -> ';'

Assign         -> SupID '=' Expression 


Expression     -> Item Expression_
Expression_    -> ('+' | '-') Item Expression_ | @
Item           -> Factor Item_
Item_          -> ('*' | '/') Factor Item_ | @
Factor         -> Num | SupID | '(' Expression ')' | '-' Factor_
Factor_        -> '(' Expression ')' | SupID | Num


While          -> 'while' '(' BooleanExp ')'  Block
For            -> 'for' '(' (Assign ';' | Empty) BooleanExp ';' Assign ')'  Block
If             -> 'if' '(' BooleanExp ')' Block If_
If_            -> 'else' Blcok | @

SupID          -> ID SupID1
SupID1         -> '[' INT | ID ']' | '(' VarList ')' | @

BooleanExp     -> Expression RltOp Expression

FunCall        -> ID '(' varList ')' ';'


RltOp          -> '<' | '>' | '==' | '<=' | '>=' | '!='

Type           -> 'int' | 'float' | 'void'

ID -> something
Num-> INT | FLOAT
INT-> something
FLOAT -> something

-----------------------------------------------------------------------------abort
Expression     -> Expression ('+' | '-') Item | Item
Item         -> Item ('*' | '/') Factor | Factor
//Dclr1           -> Type ID Dclr2
//DefFun         -> Type ID '(' DclrVarList ')'  Block                                 //或者如果一定要跟c语言一致的话，将 Block改为 '{' SubBlock '}'

-----------------------------------------*/
bool Parser();
void Program();
void DclrFun();
void FunBody();
void Block();
void SubBlock();
void Stat();
void FunCall();
void DclrVar();
void DclrVar2();
void DclrVar3();
void Read();
void Write();
void Empty();
void Expression();
void Expression_();
void Item();
void Item_();
void Factor();
void Factor_();
void While();
void For();
void Assign();
void If();
void If_(int start);
void SupId();
void SupId1();
void NormId();
void NormId1();
void BooleanExp();
void RltOp();
void Type();
void Num();
void DclrVarList();
void DclrVarList1(int type);
void DclrVarList2(int type);
void VarList();
void VarList1();
void VarList2();
void NumList();
void NumList1();
void NumList2();



bool HandleIdTable(Token ref, int type, int num = 1, int assLineNum = -1);//true normal, false abnormal


void printMidCodeFunCall(int point);
void printMidCodeArr(int point);
int getFirstId(int point);
int BackStep(int point);

extern vector<Token> arrToken;

