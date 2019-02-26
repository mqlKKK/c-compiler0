/*

#pragma warning(disable : 4996)

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include"word.h"
using namespace std;

struct par {
	int type = -1;  // 1：void函数 2：int/char函数 4：const常量 6：var变量 8：数组 10：para参数
	int value = -1;
	int addr = -1;
	int size = -1;  // 函数：参数个数   数组：元素个数  其他：-1
};
struct glo {
	int type = -1;  // 1：void函数 2：int函数 3：char函数 4：const常量 6：var变量 8：数组 10：para参数
	int value = -1;
	int addr = -1;
	int size = -1;  // 函数：参数个数   数组：元素个数  其他：-1
	map<string, par>  partab; //局部变量表
};
int location;
glo temp_glo;
par temp_par;
string temp_name;
string temp_func_name;
char temp_char;
int num_para;
map <string, glo> tab; //全局变量表
map <string, par> partab; //局变量表
int ad = 0;

int intchar;
int temp_lc;
int temp_lines;
int temp_resultn;
int temp_symbol;
string temp_token;
int selec;

vector<string> tempparatab(100);
vector<string> consttab(100);
vector<string> vartab(100);
vector<string> intfuntab(20);
vector<string> charfuntab(20);
vector<string> voidfuntab(20);
vector<string>::iterator it;

int findtab();
void factor();
void term();
void expre();
void condition();
void ifM();
void whileM();
void forM();
void statements();
void assign();
void reparatab();
void callrtnfun();
void callvoidfun();
void scanfM();
void printfM();
void returnM();
void statement();
void program();
void integer();
void constins();
void constintdef();
void constchardef();
void varins();
void vardef();
void paratab();
void statementss();
void rtnfundef();
void voidfundef();

//查表
int findtab() {

	map <string, par>::iterator it1;
	it1 = partab.find(token);
	if (it1 != partab.end()) {
		if (partab[token].type == 4)return 1;//常量
		if (partab[token].type == 6)return 2;//变量
		if (partab[token].type == 2)return 3;//int/char函数
		if (partab[token].type == 1)return 5;//void
	}
	else {
		map <string, glo>::iterator it2;
		it2 = tab.find(token);
		if (it2 != tab.end()) {
			if (tab[token].type == 4)return 1;//常量
			if (tab[token].type == 6)return 2;//变量
			if (tab[token].type == 2)return 3;//int/char函数
			if (tab[token].type == 1)return 5;//void
		}
		else {
			cout << "找不到符号 ： " << token;
			return 6;
		}
	}
}

//输出
void test() {
	map<string, glo>::const_iterator it;
	map<string, par>::const_iterator it2;
	for (it = tab.begin(); it != tab.end(); ++it) {
		cout << it->first << ":" << tab[it->first].type << " , " << tab[it->first].value << " , " << tab[it->first].addr << " , " << tab[it->first].size << endl;
		for (it2 = tab[it->first].partab.begin(); it2 != tab[it->first].partab.end(); ++it2) {

			cout << "--**--";
			cout << it2->first << ":" << tab[it->first].partab[it2->first].type << " , " << tab[it->first].partab[it2->first].value << " , " << tab[it->first].partab[it2->first].addr << " , " << tab[it->first].partab[it2->first].size << endl;

		}
			
	}
}

//建表项
void structglotab(int t, int v, int a, int s) {
	temp_glo.type = t;
	temp_glo.value = v;
	temp_glo.addr = a;
	temp_glo.size = s;
	temp_glo.partab = partab;
	if (t == 1 || t == 2) tab.insert(make_pair(temp_func_name, temp_glo));
	else tab.insert(make_pair(temp_name, temp_glo));
	if (t == 4) ad = ad + 4 * s;
}
void structpartab(int t, int v, int a, int s) {
	temp_par.type = t;
	temp_par.value = v;
	temp_par.addr = a;
	temp_par.size = s;
	partab.insert(make_pair(temp_name, temp_par));
	cout << temp_name << "  填表" << endl;
	if (t == 4) ad = ad + 4 * s;
}



//判断
//get int/char/void
// 0: void    1: int/char function    2: iden   3: main   -1: ?
int judge() {

	int mark_judge = 0;

	if (symbol == 4) mark_judge = 1; // void

	temp_lc = lc;
	temp_lines = lines;
	temp_resultn = resultn;
	temp_symbol = symbol;
	temp_token = token;

	getsym();//单词为 标识符/main

	if (symbol == 5) { // main
		if (mark_judge != 1);
		return 3;
	}

	if (mark_judge == 1) { //void
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		temp_token = "";
		return 0;
	}

	getsym();//函数定义和变量定义的分界点 
	if (symbol == 25 || symbol == 29) { // int / char 函数
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		temp_token = "";
		return 1;
	}
	if (symbol == 27 || symbol == 31 || symbol == 32) { //标识符
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		temp_token = "";
		return 2;
	}

	lc = temp_lc;
	lines = temp_lines;
	resultn = temp_resultn;
	symbol = temp_symbol;
	memset(token, 0, sizeof(token));
	strcpy(token, temp_token.c_str());
	temp_token = "";

	return -1;
}

//报错
void ejump(int end) {
	do {
		getsym();
		if (symbol != end && lc == ll) symbol = end;
	} while (symbol != end);
}
void error(int n) {
	if (n == 0) cout << "#Line  " << lines << " ： 缺少 标识符类型" << endl;
	if (n == 1) cout << "#Line  " << lines << " ： 缺少 标识符" << endl;
	if (n == 2) cout << "#Line  " << lines << " ： 缺少 赋值符号 = " << endl;
	if (n == 3) cout << "#Line  " << lines << " ： 缺少 整数" << endl;
	if (n == 4) cout << "#Line  " << lines << " ： 缺少 字符" << endl;
	if (n == 5) cout << "#Line  " << lines << " ： 缺少 右括号 ] " << endl;
	if (n == 6) cout << "#Line  " << lines << " ： 缺少 右括号 ) " << endl;
	if (n == 7) cout << "#Line  " << lines << " ： 缺少 右括号 } " << endl;
	if (n == 8) cout << "#Line  " << lines << " ： 缺少 main函数" << endl;
	if (n == 9) cout << "#Line  " << lines << " ： 缺少 左括号 ( " << endl;
	if (n == 10) cout << "#Line  " << lines << " ： 缺少 左括号 [ " << endl;
	if (n == 11) cout << "#Line  " << lines << " ： 非法语句" << endl;
	if (n == 12) cout << "#Line  " << lines << " ： 缺少 分号 ；" << endl;
	if (n == 13) cout << "#Line  " << lines << " ： 标识符未定义" << endl;
	if (n == 14) cout << "#Line  " << lines << " ： return语句错误" << endl;
	if (n == 15) cout << "#Line  " << lines << " ： 缺少 逗号 ， " << endl;
	if (n == 16) cout << "#Line  " << lines << " ： 缺少 加法运算符" << endl;
	if (n == 17) cout << "#Line  " << lines << " ： 数组大小错误" << endl;
}


//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
//get symbol --- return symbol
void factor() {
	if (symbol == 34) { //单词为 标识符
		if (findtab() == 1 || findtab() == 2) { //是变量或常量
			getsym();
			if (symbol == 27) { //单词为 [
				expre();
				if (symbol == 28) { //单词为 ]
					getsym();
					return;
				}
			}
		}
		else if (findtab() == 3) { //是 int/char函数
			callrtnfun();
			return;
		}
		else;
	}
	else if (symbol == 25) { //单词为 （
		expre();
		if (symbol == 26) { //单词为 ） 
			getsym();
			return;
		}
		else;
	}
	else if (symbol == 38) { //单词为 字符
		getsym();
		return;
	}
	else if (symbol == 13 || symbol == 14 || symbol == 35) { //单词为 + - 整数
		integer();
		getsym();
		return;
	}
	else;
}

//＜项＞::= ＜因子＞{＜乘法运算符＞＜因子＞}
//get symbol --- return symbol
void term() {
	factor();
	while (symbol == 15 || symbol == 16) { //单词为 * /
		getsym();
		factor();
	}
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
//not need symbol --- return symbol
void expre() {
	getsym();
	if (symbol == 13 || symbol == 14) { //单词为 + -
		getsym();
		term();
	}
	else term();
	while (symbol == 13 || symbol == 14) {
		getsym();
		term();
	}
}

//＜条件＞ ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ 
//not need symbol --- return symbol
void condition() {
	expre();
	if (symbol == 17) { //   <
		expre();
	}
	else if (symbol == 18) {  //   <=
		expre();
	}
	else if (symbol == 19) {  //   >
		expre();
	}
	else if (symbol == 20) {  //   >=
		expre();
	}
	else if (symbol == 21) {  //   !=
		expre();
	}
	else if (symbol == 22) {  //   ==
		expre();
	}
	else {

	}
}

//＜条件语句＞::= if ‘(’＜条件＞‘)’＜语句＞[else＜语句＞]
//not get symbol --- return symbol
void ifM() {
	getsym();
	if (symbol != 25) error(9); //单词为 （
	condition();
	if (symbol != 26) error(6); //单词为  ）
	cout << "这是条 条件语句" << endl;
	getsym();
	statement();
	if (symbol == 7) { //单词为 else
		getsym();
		statement();
	}
}

//＜循环语句1＞  :: = while ‘(’＜条件＞‘)’＜语句＞
//not get symbol --- return symbol
void whileM() {
	getsym();
	if (symbol != 25) error(9); //单词为 （
	condition();
	if (symbol != 26) error(9); //单词为  ）
	cout << "这是条 循环语句" << endl;
	getsym();
	statement();
}

//＜循环语句2＞::=for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜无符号整数＞')'＜语句＞
//not get symbol --- return symbol
void forM() {
	getsym();
	if (symbol != 25) error(9); //单词为 （
	getsym();
	if (symbol != 34)error(1); //单词为 标识符
	getsym();
	if (symbol != 33)error(2); //单词为 =
	expre();
	if (symbol != 32)error(12); //单词为 ；
	condition();
	if (symbol != 32)error(12);  //单词为 ；
	getsym();
	if (symbol != 34)error(1); //单词为 标识符
	getsym();
	if (symbol != 33)error(2); //单词为 =
	getsym();
	if (symbol != 34)error(1); //单词为 标识符
	getsym();
	if (symbol != 13 && symbol != 14) error(16); //单词为 + -
	getsym();
	if (symbol != 35) error(9); //单词为 无符号整数
	getsym();
	if (symbol != 26) error(6); //单词为 ）
	cout << "这是条 循环语句" << endl;
	getsym();
	statement();
}

//＜语句列＞::= ｛＜语句＞｝
//not get symbol --- return symbol
void statements() {
	getsym();
	while (symbol == 6 || symbol == 8 || symbol == 9 || symbol == 29 ||
		symbol == 34 || symbol == 10 || symbol == 11 || symbol == 32 || symbol == 12) {
		statement();
	}
}

//＜赋值语句＞   :: =＜标识符＞[‘[’＜表达式＞‘]’] = ＜表达式＞
//not get symbol --- return symbol
void assign() {
	getsym();
	if (symbol == 27) { // 单词为 [
		expre();
		if (symbol != 28) error(5); //单词为 ]
		getsym();
	}
	if (symbol != 33); //单词为 =
	expre();
}

//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}
//not need symbol --- return symbol
void reparatab() {
	expre();
	while (symbol == 31) { //单词为 ，
		expre();
	}
}

//＜有返回值函数调用语句＞ ::= ＜标识符＞[‘(’＜值参数表＞‘)'] 
//not need symbol --- return symbol
void callrtnfun() {
	getsym();
	if (symbol == 25) { //单词为 （
		reparatab();
		if (symbol != 26) error(9); //单词为  ）
		getsym();
		return;
	}
	else return;
}
//＜无返回值函数调用语句＞ ::= ＜标识符＞[‘(’＜值参数表＞‘)']
//not need symbol --- return symbol
void callvoidfun() {
	getsym();
	if (symbol == 25) { //单词为 （
		reparatab();
		if (symbol != 26) error(6); //单词为  ）
		getsym();
		return;
	}
	else return;
}

//＜读语句＞::=  scanf‘(’＜标识符＞{,＜标识符＞}‘)’
//not need symbol --- return symbol
void scanfM() {
	getsym();
	if (symbol != 25) error(9); //单词为 （
	getsym();
	if (symbol != 34) error(1); //单词为 标识符
	getsym();
	while (symbol == 31) { //单词为 ，
		getsym();
		if (symbol != 34) break; //单词为 标识符
		getsym();
	}
	if (symbol != 26) error(6); //单词为 )
	getsym();
}

//＜写语句＞::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’| printf ‘(’＜表达式＞‘)’
//not need symbol --- return symbol
void printfM() {
	getsym();
	if (symbol != 25)error(9); //单词为 （

	temp_lc = lc;
	temp_lines = lines;
	temp_resultn = resultn;
	temp_symbol = symbol;
	temp_token = token;

	getsym();
	if (symbol == 37) { //单词为 字符串
		getsym();
		if (symbol == 26) { //单词为 ）
			getsym();
			return;
		}
		if (symbol != 31) { //单词为 ，
			error(15);
		}
		expre();
		if (symbol != 26) error(6); //单词为 )
		getsym();
		return;
	}

	else {

		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		temp_token = "";

		expre();

		if (symbol == 26) { //单词为 ）
			getsym();
			return;
		}
		else error(6);
		return;
	}
}

//＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’] 
//not need symbol --- return symbol
void returnM() {
	getsym();
	if (symbol == 32) { //单词为 ；
		return;
	}
	else if (symbol == 25) { //单词为 （
		expre();
		if (symbol != 26) error(6); //单词为 ）
		getsym();
	}
	else {
		ejump(32);
		error(14);
	}
}

//＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’| 
//			   ＜赋值语句＞;｜＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜
//			   ＜读语句＞;｜＜写语句＞;｜＜空＞;|＜返回语句＞;
//  if  /  while  /  for  /  {  /  标识符+=/[  /  标识符； /  标识符+ （ /  scanf  /  printf  /  ；  /  return
//get symbol ---  return symbol
void statement() {
	switch (symbol)
	{
	case 6: { // if
		ifM();
		break;
	}
	case 8: { // while
		whileM();
		break;
	}
	case 9: { // for
		forM();
		break;
	}
	case 29: { // {  语句列
		statements();
		if (symbol != 30) error(7); //单词为 }
		getsym();
		break;
	}
	case 34: { // 标识符
		if (findtab() == 2) { // 变量
			assign();
			cout << "这是条 赋值语句" << endl;
			if (symbol != 32) { //单词为 ；
				ejump(32);
				error(12);
			}
		}
		else if (findtab() == 3) { //int / char 函数
			callrtnfun();
			cout << "这是条 有返回值函数调用语句" << endl;
			if (symbol != 32) { //单词为 ；
				ejump(32);
				error(12);
			}
		}
		else if (findtab() == 5) { // void 函数
			callvoidfun();
			cout << "这是条 无返回值函数调用语句" << endl;
			if (symbol != 32) { //单词为 ；
				ejump(32);
				error(12);
			}
		}
		else {
			ejump(32);
			error(13);
		}
		getsym();
		break;
	}
	case 10: { // scanf
		scanfM();
		cout << "这是条 读语句" << endl;
		if (symbol != 32) { //单词为 ；
			ejump(32);
			error(12);
		}
		getsym();
		break;
	}
	case 11: { //  printf
		printfM();
		cout << "这是条 写语句" << endl;
		if (symbol != 32) { //单词为 ；
			ejump(32);
			error(12);
		}
		getsym();
		break;
	}
	case 32: { //  <空> ；
		getsym();
		cout << "这是条 空语句" << endl;
		break;
	}
	case 12: { //  return  
		returnM();
		cout << "这是条 返回语句" << endl;
		if (symbol != 32) { //单词为 ；
			ejump(32);
			error(12);
		}
		getsym();
		break;
	}
	default: {
		ejump(32);
		error(11);
		getsym();
	}
	}
}
//===================================================================================================================
//＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program() {
	location = 0;
	getsym();
	if (symbol == 1)  //单词为 const --------［＜常量说明＞］
		constins();
	// 0: void    1: int/char function    2: iden   3: main   -1: ?
	selec = judge();
	if (selec == 2) varins(); // --------------------------------［＜变量说明＞］

	while (selec != 3 && selec != -1 && selec != 2) { //------------------------------ {＜有返回值函数定义＞ | ＜无返回值函数定义＞}
		if (selec == 0) { //void
			voidfundef();
		}
		else if (selec == 1) { //int/char
			rtnfundef();
		}
		else;
		selec = judge();
	}

	//void mian
	if (selec != 3) {
		ejump(5);
		error(8);
	}
	getsym();
	if (symbol != 25) error(9); //单词为 (
	getsym();
	if (symbol != 26) error(6);//单词为 )
	getsym();
	if (symbol != 29) error(10); //单词为 {
	location = 1;
	temp_func_name = "main";//*****************************
	cout << "这是条 主函数" << endl;
	statementss();
	if (symbol != 30) error(7); //单词为 }
	structglotab(1, 0, ad, num_para);//**************************
	cout << "呼 ~ 语法分析完了" << endl;
}
//===============================================================================================================
//＜整数＞ :: = ［＋｜－］＜无符号整数＞
//get symbol
void integer() {
	if (symbol == 35) { //单词为无符号整数
		return;
	}
	else if (symbol == 13 ) { //单词为加号
		getsym();
		if (symbol == 35) {
			return;
		}
	}
	else if (symbol == 14) { //单词为减号
		getsym();
		if (symbol == 35) {
			num = -num;
			return;
		}
	}
	else {
		error(3);
		num = 0; //默认为 0
	}
}
//＜常量说明＞ ::= const＜常量定义＞; { const＜常量定义＞; }
//get const ---- return symbol
void constins() {
	getsym();
	if (symbol == 2) //单词为 int
		constintdef();
	else if (symbol == 3) //单词为 char
		constchardef();
	else {
		error(0);
		ejump(32);
	}
	if (symbol == 32) { //单词为 ；
		cout << "这是条 常量说明" << endl;
	}

	while (true) {
		getsym();
		if (symbol != 1) break; //单词为 const
		getsym();
		if (symbol == 2) //单词为 int
			constintdef();
		else if (symbol == 3) //单词为 char
			constchardef();
		else {
			error(0);
			ejump(32);
		}
		if (symbol == 32)  //单词为 ；
			cout << "这是条 常量说明" << endl;
	}
}
//＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
//get int/char --- return symbol
void constintdef() {
	getsym();
	if (symbol != 34) { //单词是标识符
		error(1);
		ejump(32);
		return;
	}
	temp_name = token;//******************************************
	getsym();
	if (symbol != 33) { //单词是 = 
		error(2);
	}
	getsym();
	integer();
	if (location == 0) structglotab(4, num, ad, 1);//*******************************************
	else structpartab(4, num, ad, 1);//*******************************************
	while (true) {
		getsym();
		if (symbol != 31) break;  //单词为 ，
		getsym();
		if (symbol != 34) { //单词是标识符
			error(1);
			ejump(32);
			return;
		}
		temp_name = token; //************************************************
		getsym();
		if (symbol != 33) { //单词是 =
			error(2);
		}
		getsym();
		integer();
		if (location == 0) structglotab(4, num, ad, 1);//*******************************************
		else structpartab(4, num, ad, 1);//*******************************************
	}
}
void constchardef() {
	getsym();
	if (symbol != 34) { //单词是标识符
		error(1);
		ejump(32);
		return;
	}
	temp_name = token;//******************************************
	getsym();
	if (symbol != 33) { //单词是 = 
		error(2);
	}
	getsym();
	if (symbol != 38) { //单词是 字符
		error(4);
		temp_char = ' ';
	}
	else temp_char = token[0];
	if (location == 0) structglotab(4, temp_char, ad, 1);//*******************************************
	else structpartab(4, temp_char, ad, 1);//*******************************************
	while (true) {
		getsym();
		if (symbol != 31) break;  //单词为 ，
		getsym();
		if (symbol != 34) { //单词是标识符
			error(1);
			ejump(32);
			return;
		}
		temp_name = token;//******************************************
		getsym();
		if (symbol != 33) { //单词是 =
			error(2);
		}
		getsym();
		if (symbol != 38) { //单词是 字符
			error(4);
			temp_char = ' ';
		}
		else temp_char = token[0];
		if (location == 0) structglotab(4, temp_char, ad, 1);//*******************************************
		else structpartab(4, temp_char, ad, 1);//*******************************************
	}
}
//===============================================================================================================
//＜变量说明＞ :: = ＜变量定义＞; {＜变量定义＞; }
//get int/char ---- return symbol
void varins() {
	do {
		vardef();
		if (symbol != 32); //单词为 ；
		cout << "这是条 变量说明" << endl;
		getsym();
		selec = judge();
	} while (selec == 2);
}
//＜变量定义＞::= (int|char)(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’)} //无符号整数需大于0 
//not get --- return symbol
void vardef() {
	do {
		getsym();
		if (symbol != 34) { //单词是标识符
			error(1);
			ejump(32);
			return;
		}
		temp_name = token; //**********************************
		getsym();
		if (symbol == 27) { //单词为 [
			getsym();
			if (symbol != 35) { //单词为 无符号整数
				integer();
				if (num <= 0) {
					error(17);
					num = 1;
				}
				if (location == 0) structglotab(6, 0, ad, num);//*******************************************
				else structpartab(6, 0, ad, num);//******************************************
			}
			getsym();
			if (symbol != 28) { //单词为 ]
				error(5);
				ejump(32);
				return;
			}
			getsym();
		}
		if (location == 0) structglotab(6, 0, ad, num);//*******************************************
		else structpartab(6, 0, ad, num);//******************************************
	} while (symbol == 31); //单词为 ，
}
//===================================================================================================
//＜参数表＞    :: = ＜类型标识符＞＜标识符＞{ ,＜类型标识符＞＜标识符＞ }
//return symbol
void paratab() {
	num_para = 0;
	while (true) {
		getsym();
		if (symbol == 2) { //单词为int
			getsym();
			if (symbol == 34) { //单词为 标识符
				num_para++;
				structpartab(6, 0, ad, 0);//***********************************
				getsym();
			}
			else {
				ejump(31);
				error(1);
			}
		}
		else if (symbol == 3) { //单词为char
			getsym();
			if (symbol == 34) { //单词为 标识符
				num_para++;
				structpartab(6, 0, ad, 0); //*****************************************
				getsym();
			}
		}
		else {
			ejump(31);
			error(0);
		}
		if (symbol == 31) continue; // 单词为 ，
		else {
			cout << "这是条 参数表" << endl;
			break;
		}
	}
}
//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］｛＜语句＞｝
//return symbol
void statementss() {
	getsym();
	if (symbol == 1) {  //单词为 const --------［＜常量说明＞］
		constins();
	}

	if (symbol == 2 || symbol == 3) {
		varins();
	}

	while (symbol == 6 || symbol == 8 || symbol == 9 || symbol == 29 ||
		symbol == 34 || symbol == 10 || symbol == 11 || symbol == 32 || symbol == 12) {
		statement();
	}
}
//＜有返回值函数定义＞  ::=  (int | char) <标识符>[‘(’＜参数表＞‘)’]‘{’＜复合语句＞‘}’
//＜无返回值函数定义＞  ::=   void ＜标识符＞ [(’＜参数表＞‘)’]‘{’＜复合语句＞‘}’
//get symbol---return symbol
void rtnfundef() {
	getsym();
	if (symbol != 34); //单词为 标识符 ---已在judge中处理
	temp_func_name = token;//*****************************
	getsym();
	if (symbol == 25) { //单词为 （
		paratab();
		if (symbol != 26) error(6); //单词为 ）
		getsym();
	} 
	if (symbol != 29); //单词为 {  --------已在judge中处理
	cout << "这是条 有返回值函数定义" << endl;
	location = 1; //*************************************
	statementss();
	if (symbol != 30) error(7); //单词为 }
	location = 0; //****************************
	structglotab(2, 0, ad, num_para);//**************************
	partab.clear();
	getsym();
}
void voidfundef() {
	getsym();
	if (symbol != 34); //单词为 标识符 ---已在judge中处理
	temp_func_name = token;//*****************************
	getsym();
	if (symbol == 25) { //单词为 （
		paratab();
		if (symbol != 26)error(6); //单词为 ）
		getsym();
	}
	if (symbol != 29); //单词为 {  --------已在judge中处理

	cout << "这是条 无返回值函数定义" << endl;
	location = 1; //*************************************
	statementss();
	if (symbol != 30) error(7); //单词为 }
	location = 0; //****************************
	structglotab(1, 0, ad, num_para);//**************************
	partab.clear();
	getsym();
}

int main() {
	init();
	program();
	test();
	getchar();
	getchar();
	return 0;
}
*/