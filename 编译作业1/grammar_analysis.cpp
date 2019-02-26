#pragma warning(disable : 4996)

#include<iostream>
#include<string>
#include<list>
#include<map>
#include<algorithm>
#include"code.h"
#include"word.h"

//#include"word_analysis.cpp"
//#include"midcode.cpp"
using namespace std;

int location;
int isret, isret2;

string ptype = "";

char temp_char;
char temp_line[1024];
int num_para;

int isarr;
int ischar_fun;
int justchar;
int charvalue;
int ek = 0;


map <string, string> ss;
int ssnum = 0;

int intchar;
int temp_lc;
int temp_ll;
int temp_lines;
int temp_resultn;
int temp_symbol;
string temp_token;
int selec;
int result_expre;

void factor();
void term();
void expre();
void condition();
void ifM();
void whileM();
void forM();
void statements();
void assign();
int reparatab();
void callrtnfun(int funtype);
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

int have_error = 0;
string endla = new_label(1);

//���
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

//
bool test(string s) {
	if (findtab4(s))return true;
	else return false;
}

//�ж�
//get int/char/void
// 0: void    1: int/char function    2: iden   3: main   -1: ?
int judge() {
	
	if (symbol == 29) return -1;

	int mark_judge = 0;

	if (symbol == 4) mark_judge = 1; // void

	temp_lc = lc; //��ָ��
	temp_lines = lines; //����
	temp_ll = ll;
	temp_resultn = resultn;
	temp_symbol = symbol;
	temp_token = token;
	memset(temp_line, 0, sizeof(temp_line)); 
	strcpy(temp_line, line); //��ǰ��
	setmark(1);
	getsym(0);//����Ϊ ��ʶ��/main

	if (symbol == 5) { // main
		if (mark_judge != 1);
		return 3;
	}

	if (mark_judge == 1) { //void
		ll = temp_ll;
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		memset(line, 0, sizeof(line));
		strcpy(line, temp_line);
		temp_token = "";
		setmark(2);
		return 0;
	}

	getsym(0);//��������ͱ�������ķֽ�� 
	if (symbol == 25 || symbol == 29) { // int / char ����
		ll = temp_ll;
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		memset(line, 0, sizeof(line));
		strcpy(line, temp_line);
		temp_token = "";
		setmark(2);
		return 1;
	}
	if (symbol == 27 || symbol == 31 || symbol == 32) { //��ʶ��
		ll = temp_ll;
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		memset(line, 0, sizeof(line));
		strcpy(line, temp_line);
		temp_token = "";
		setmark(2);
		return 2;
	}

	ll = temp_ll;
	lc = temp_lc;
	lines = temp_lines;
	resultn = temp_resultn;
	symbol = temp_symbol;
	memset(token, 0, sizeof(token));
	strcpy(token, temp_token.c_str());
	memset(line, 0, sizeof(line));
	strcpy(line, temp_line);
	temp_token = "";
	setmark(2);
	return -1;
}

//����
void ejump(int end) {
	do {
		getsym(0);
		if (symbol != end && lc == ll) symbol = end;
	} while (symbol != end);
}
void error(int n) {
	have_error = 1;
	if (n == 0) cout << "#Line  " << lines << " �� ȱ�� ��ʶ������" << endl;
	if (n == 1) cout << "#Line  " << lines << " �� ȱ�� ��ʶ��" << endl;
	if (n == 2) cout << "#Line  " << lines << " �� ȱ�� ��ֵ���� = " << endl;
	if (n == 3) cout << "#Line  " << lines << " �� ȱ�� ����" << endl;
	if (n == 4) cout << "#Line  " << lines << " �� ȱ�� �ַ�" << endl;
	if (n == 5) cout << "#Line  " << lines << " �� ȱ�� ������ ] " << endl;
	if (n == 6) cout << "#Line  " << lines << " �� ȱ�� ������ ) " << endl;
	if (n == 7) cout << "#Line  " << lines << " �� ȱ�� ������ } " << endl;
	if (n == 8) cout << "#Line  " << lines << " �� ȱ�� main����" << endl;
	if (n == 9) cout << "#Line  " << lines << " �� ȱ�� ������ ( " << endl;
	if (n == 10) cout << "#Line  " << lines << " �� ȱ�� ������ [ " << endl;
	if (n == 11) cout << "#Line  " << lines << " �� �Ƿ����" << endl;
	if (n == 12) cout << "#Line  " << lines << " �� ȱ�� �ֺ� ��" << endl;
	if (n == 13) cout << "#Line  " << lines << " �� ��ʶ��δ����" << endl;
	if (n == 14) cout << "#Line  " << lines << " �� return������" << endl;
	if (n == 15) cout << "#Line  " << lines << " �� ȱ�� ���� �� " << endl;
	if (n == 16) cout << "#Line  " << lines << " �� ȱ�� �ӷ������" << endl;
	if (n == 17) cout << "#Line  " << lines << " �� �����С����" << endl;
	if (n == 18) cout << "#Line  " << lines << " �� ����ȱ�ٷ���ֵ" << endl;
	if (n == 19) cout << "#Line  " << lines << " �� �������" << endl;
	if (n == 20) cout << "#Line  " << lines << " �� ��0����" << endl;
	if (n == 21) cout << "#Line  " << lines << " �� ��ֵ������ߴ���" << endl;
	if (n == 22) cout << "#Line  " << lines << " �� ���鸳ֵȱ���±�" << endl;
	if (n == 23) cout << "#Line  " << lines << " �� ��������ֵ���ʹ���" << endl;
	if (n == 24) cout << "#Line  " << lines << " �� ��ֵ������Ͳ�ƥ��" << endl;
	if (n == 25) cout << "#Line  " << lines << " �� ����ȱ�ٲ���" << endl;
	if (n == 26) cout << "#Line  " << lines << " �� �޲����������ô���" << endl;
	if (n == 27) cout << "#Line  " << lines << " �� ��ʶ���ظ�����" << endl;
	if (n == 28) cout << "#Line  " << lines << " ��	��������ֵ" << endl;
	if (n == 29) cout << "#Line  " << lines << " ��	��������" << endl;
	if (n == 30) cout << "#Line  " << lines << " ��	��������ʱ������������" << endl;
}


//�����ӣ�::= ����ʶ����������ʶ������[�������ʽ����]��|��(�������ʽ����)������������|���ַ��������з���ֵ����������䣾
//get symbol --- return symbol
void factor() {
	if (symbol == 34) { //����Ϊ ��ʶ��
		if (findtab(token) == 1 || findtab(token) == 2 || findtab(token) > 100) { //�Ǳ�������
			if (findtab3(token) == 4 || findtab3(token) == 6 || findtab3(token) == 8) justchar = 0;
			RPN_struct(0, "", token, ek);
			getsym(0);
			int mark = 0;
			if (justchar == 1) mark = 1;
			if (symbol == 27) { //����Ϊ [
				RPN_struct(1, "=[]", "", ek);
				RPN_struct(1, "[", "", ek);
				expre();
				
				if (symbol == 28) { //����Ϊ ]
					if (mark == 1) justchar = 1;
					RPN_struct(1, "]", "", ek);
					getsym(0);
					return;
				}
			}
		}
		else if (findtab3(token) == 3 || findtab3(token) == 2) { //�� int/char����
			if (findtab3(token) == 2) justchar = 0;
			int mark2 = 0;
			if (findtab3(token) == 3 && justchar == 1) mark2 = 1;
			callrtnfun(findtab3(token));
			RPN_struct(0, "", new_var(0), ek);
			if (mark2 == 1) justchar = 1;
			return;
		}
		else;
	}
	else if (symbol == 25) { //����Ϊ ��
		justchar = 0;
		RPN_struct(1, "(", "", ek);
		expre();

		if (symbol == 26) { //����Ϊ �� 
			RPN_struct(1, ")", "", ek);
			getsym(0);
			return;
		}
		else;
	}
	else if (symbol == 38) { //����Ϊ �ַ�
		charvalue = token[0];
		RPN_struct(0, "", to_string(charvalue), ek);
		getsym(0);
		return;
	}
	else if (symbol == 13 || symbol == 14 || symbol == 35) { //����Ϊ + - ����
		justchar = 0;
		integer();
		int mark2 = 0;
		if (num < 0) {
			mark2 = 1;
			num = - num;
			RPN_struct(1, "(", "", ek);
			RPN_struct(0, "", to_string(0), ek);
			RPN_struct(1, "-", "", ek);
		}
		RPN_struct(0, "", to_string(num), ek);
		if (mark2 == 1)RPN_struct(1, ")", "", ek);
		getsym(0);
		return;
	}
	else;
}

//���::= �����ӣ�{���˷�������������ӣ�}
//get symbol --- return symbol
void term() {
	factor();
	while (symbol == 15 || symbol == 16) { //����Ϊ * /
		justchar = 0;
		RPN_struct(1, token, "", ek);
		getsym(0);
		factor();
	}
}

//�����ʽ�� ::= �ۣ������ݣ��{���ӷ�����������}
//not need symbol --- return symbol
void expre() {
	getsym(0);
	if (symbol == 13 || symbol == 14) { //����Ϊ + -
		justchar = 0;
		if (symbol == 14) {
			RPN_struct(0, "", to_string(0), ek);
			RPN_struct(1, "-", "", ek);
		}
		getsym(0);
		term();
	}
	else term();
	while (symbol == 13 || symbol == 14) {
		justchar = 0;
		RPN_struct(1, token, "", ek);
		getsym(0);
		term();
	}
}

//�������� ::=  �����ʽ������ϵ������������ʽ���������ʽ�� 
//not need symbol --- return symbol
void condition() {
	justchar = 1;
	expre();
	if (justchar == 1) error(29);
	string e1 = RPN_reck(ek), e2;
	if (e1 == "arrayerror") error(19);
	if (e1 == "div0") error(20);
	if (symbol == 17) { //   <
		justchar = 1;
		expre();
		if (justchar == 1) error(29);
		e2 = RPN_reck(ek);
		if (e2 == "arrayerror") error(19);
		if (e2 == "div0") error(20);
		ccode("cmp", e1, e2, "<");
	}
	else if (symbol == 18) {  //   <=
		justchar = 1;
		expre();
		if (justchar == 1) error(29);
		e2 = RPN_reck(ek);
		if (e2 == "arrayerror") error(19);
		if (e2 == "div0") error(20);
		ccode("cmp", e1, e2, "<=");
	}
	else if (symbol == 19) {  //   >
		justchar = 1;
		expre();
		if (justchar == 1) error(29);
		e2 = RPN_reck(ek);
		if (e2 == "arrayerror") error(19);
		if (e2 == "div0") error(20);
		ccode("cmp", e1, e2, ">");
	}
	else if (symbol == 20) {  //   >=
		justchar = 1;
		expre();
		if (justchar == 1) error(29);
		e2 = RPN_reck(ek);
		if (e2 == "arrayerror") error(19);
		if (e2 == "div0") error(20);
		ccode("cmp", e1, e2, ">=");
	}
	else if (symbol == 21) {  //   !=
		justchar = 1;
		expre();
		if (justchar == 1) error(29);
		e2 = RPN_reck(ek);
		if (e2 == "arrayerror") error(19);
		if (e2 == "div0") error(20);
		ccode("cmp", e1, e2, "!=");
	}
	else if (symbol == 22) {  //   ==
		justchar = 1;
		expre();
		if (justchar == 1) error(29);
		e2 = RPN_reck(ek);
		if (e2 == "arrayerror") error(19);
		if (e2 == "div0") error(20);
		ccode("cmp", e1, e2, "==");
	}
	else {
		ccode("cmp", e1, "1", "==");
	}
}

//��������䣾::= if ��(������������)������䣾[else����䣾]
//not get symbol --- return symbol
void ifM() {
	string name1 = new_label(1);
	getsym(0);
	if (symbol != 25) error(9); //����Ϊ ��
	condition();
	if (symbol != 26) error(6); //����Ϊ  ��
	ccode("BZ", "", "", name1); //��������ת
	cout << "������ �������" << endl;
	getsym(0);
	statement();
	
	if (symbol == 7) { //����Ϊ else
		string name2 = new_label(1);
		ccode("GOTO", "", "", name2);
		ccode("set", "", "", name1);
		getsym(0);
		statement();
		ccode("set", "", "", name2);
	}
	else ccode("set", "", "", name1);
}

//��ѭ�����1��  :: = while ��(������������)������䣾
//not get symbol --- return symbol
void whileM() {
	
	string name1 = new_label(1);
	string name2 = new_label(1);
	getsym(0);
	ccode("set", "", "", name2); //
	if (symbol != 25) error(9); //����Ϊ ��
	condition();
	if (symbol != 26) error(9); //����Ϊ  ��
	ccode("BZ", "", "", name1); //��������ת
	cout << "������ ѭ�����" << endl;
	getsym(0);
	statement();
	ccode("GOTO", "", "", name2); //
	ccode("set", "", "", name1); //
}

//��ѭ�����2��::=for'('����ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)���޷���������')'����䣾
//not get symbol --- return symbol
void forM() {
	//int x;
	string id, ex1, ex2, ex3, op;
	string start = new_label(1);
	string loop = new_label(1);
	string end = new_label(1);;
	getsym(0);
	if (symbol != 25) error(9); //����Ϊ ��
	getsym(0);
	if (symbol != 34)error(1); //����Ϊ ��ʶ��
	id = token;
	getsym(0);
	if (symbol != 33)error(2); //����Ϊ =
	expre(); //��ִ��һ��
	ex1 = RPN_reck(ek);
	if (ex1 == "arrayerror") error(19);
	if (ex1 == "div0") error(20);
	ccode("=", ex1, "", id);
	ccode("GOTO", "", "", start);
	ccode("set", "", "", loop);
	if (symbol != 32)error(12); //����Ϊ ��
	condition();
	ccode("BZ", "", "", end);
	if (symbol != 32)error(12);  //����Ϊ ��
	getsym(0);
	if (symbol != 34)error(1); //����Ϊ ��ʶ�� id
	getsym(0);
	if (symbol != 33)error(2); //����Ϊ =
	getsym(0);
	if (symbol != 34)error(1); //����Ϊ ��ʶ�� id 
	getsym(0);
	if (symbol != 13 && symbol != 14) error(16); //����Ϊ + -
	op = token;
	getsym(0);
	if (symbol != 35) error(9); //����Ϊ �޷������� 
	ex3 = token;
	//x = num;
	getsym(0);
	if (symbol != 26) error(6); //����Ϊ ��
	cout << "������ ѭ�����" << endl;
	getsym(0);
	ccode("set", "", "", start);
	statement();
	ccode(op, id, ex3, id);
	ccode("GOTO", "", "", loop);
	ccode("set", "", "", end);
}

//������У�::= ������䣾��
//not get symbol --- return symbol
void statements() {
	getsym(0);
	while (symbol == 6 || symbol == 8 || symbol == 9 || symbol == 29 ||
		symbol == 34 || symbol == 10 || symbol == 11 || symbol == 32 || symbol == 12) {
		statement();
	}
}

//����ֵ��䣾   :: =����ʶ����[��[�������ʽ����]��] = �����ʽ��
//not get symbol --- return symbol
void assign() {
	int leftchar = 0;
	string name = token;
	if (!(findtab(name) == 2 || findtab(name) > 100)) error(21);

	if (findtab3(name) == 7 || findtab3(name) == 9)leftchar = 1;

	string index = "", e2;
	getsym(0);
	if (isarr == 1 && symbol != 27) error(22);
	if (symbol == 27) { // ����Ϊ [
		expre();
		index = RPN_reck(ek);
		if (index == "arrayerror") error(19);
		else if (index == "div0") error(20); 
		if (symbol != 28) error(5); //����Ϊ ]
		getsym(0);
	}
	if (symbol != 33); //����Ϊ =
	justchar = 1;
	expre();
	if (leftchar != justchar) error(24);
	e2 = RPN_reck(ek);
	if (e2 == "arrayerror") error(19);
	if (e2 == "div0") error(20);
	if (index == "") ccode("=", e2 , "",name);
	else ccode("[]=", e2, index, name);
}

//��ֵ������ ::= �����ʽ��{,�����ʽ��}
//not need symbol --- return symbol
int reparatab() {
	string e[10];
	int num = 0, num2 = 0;
	ek++;
	expre();
	num2++;
	e[num++] = RPN_reck(ek);
	ek--;
	if (e[num] == "arrayerror") error(19);
	if (e[num] == "div0") error(20);
	//ccode("push", "", "", e);
	while (symbol == 31) { //����Ϊ ��	
		ek++;
		expre();
		num2++;
		e[num++] = RPN_reck(ek);
		ek--;
		if (e[num] == "arrayerror") error(19);
		if (e[num] == "div0") error(20);
		//ccode("push", "", "", e);
	}
	for (int i = 0; i < num; i++) {
		ccode("push", "", "", e[i]);
	}
	return num2;
}

//���з���ֵ����������䣾 ::= ����ʶ����[��(����ֵ��������)'] 
//not need symbol --- rtn symbol
void callrtnfun(int funtype) {
	int flag = 1;
	string fname = token;
	getsym(0);
	if (symbol == 25) { //����Ϊ ��
		flag = 0;
		if (tab[fname].addr == 0) {
			error(26);
			flag = 1;
		}
		else {
			if (reparatab() != tab[fname].size) error(30);
			if (symbol != 26) error(6); //����Ϊ  ��
			ccode("call", fname, "", "");
			ccode("=", "RET", "", new_var(1));
			temp_name = new_var(0);
			if (funtype == 2) structpartab(6, 0, ad, num);
			else structpartab(7, 0, ad, num);
			getsym(0);
			return;
		}
	}
	if (flag == 1) {
		ccode("call", fname, "", "");
		ccode("=", "RET", "", new_var(1));
		temp_name = new_var(0);
		if (funtype == 2) structpartab(6, 0, ad, num);
		else structpartab(7, 0, ad, num);
		return;
	}
}
//���޷���ֵ����������䣾 ::= ����ʶ����[��(����ֵ��������)']
//not need symbol --- return symbol
void callvoidfun() {
	string fname = token;
	getsym(0);
	if (symbol == 25) { //����Ϊ ��
		if (reparatab() != tab[fname].size) error(30);
		if (symbol != 26) error(6); //����Ϊ  ��
		ccode("call", fname, "", "");
		getsym(0);
		return;
	}
	else {
		ccode("call", fname, "", "");
		return;
	}
}

//������䣾::=  scanf��(������ʶ����{,����ʶ����}��)��
//not need symbol --- return symbol
void scanfM() {
	getsym(0);
	if (symbol != 25) error(9); //����Ϊ ��
	getsym(0);
	if (symbol != 34) error(1); //����Ϊ ��ʶ��
	ccode("scanf", token, "", "sym");
	getsym(0);
	while (symbol == 31) { //����Ϊ ��
		getsym(0);
		if (symbol != 34) break; //����Ϊ ��ʶ��
		ccode("scanf", token, "", "sym");
		getsym(0);
	}
	if (symbol != 26) error(6); //����Ϊ )
	getsym(0);
}

//��д��䣾::= printf ��(�� ���ַ�����,�����ʽ�� ��)��| printf ��(�����ַ����� ��)��| printf ��(�������ʽ����)��
//not need symbol --- return symbol
void printfM() {
	string ptfs;
	string sssnum, e;
	getsym(0);
	if (symbol != 25)error(9); //����Ϊ ��

	temp_lc = lc;
	temp_lines = lines;
	temp_resultn = resultn;
	temp_symbol = symbol;
	temp_token = token;

	getsym(0);
	if (symbol == 37) { //����Ϊ �ַ���
		ptfs = token;
		if (ss.count(token) == 0 && ptfs != "") {
			sssnum = "s" + to_string(ssnum++);
			ss[token] = sssnum;	
		}
		getsym(0);
		if (symbol == 26) { //����Ϊ ��
			if (ptfs != "")ccode("printf", ptfs, "s", "sym");
			getsym(0);
			return;
		}
		if (symbol != 31) { //����Ϊ ��
			error(15);
		}
		justchar = 1;
		expre();
		e = RPN_reck(ek);
		if (e == "arrayerror") error(19);
		if (e == "div0") error(20);
		ccode("printf", ptfs, "s", "sym");
										 //CHAR����             CAHR����			'a'                 ����
		//if (justchar == 1 && (findtab3(e) == 5 || findtab3(e) == 7 || findtab3(e) == -1 || findtab3(e) == 3))ccode("printf", e, "c", "sym");
		if (justchar == 1 && findtab3(e) == -1) ccode("printf", e, "c", "sym");
		else ccode("printf", e, "i", "sym");
		if (symbol != 26) error(6); //����Ϊ )
		getsym(0);
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

		justchar = 1;
		expre();
		e = RPN_reck(ek);
		if (e == "arrayerror") error(19);
		if (e == "div0") error(20);
		if (justchar == 1 && findtab3(e) == -1 ) ccode("printf", e, "c", "sym");
		else ccode("printf", e, "i", "sym");

		if (symbol == 26) { //����Ϊ ��
			getsym(0);
			return;
		}
		else error(6);
		return;
	}
}

//��������䣾   ::=  return[��(�������ʽ����)��] 
//not need symbol --- return symbol
void returnM() {
	string e;
	getsym(0);
	if (symbol == 32 && location == 3) {
		ccode("GOTO", "", "", endla);
	}
	else {


		if (symbol == 32) { //����Ϊ ��
			ccode("rtn", "", "", "");
			return;
		}
		else if (symbol == 25) { //����Ϊ ��
			justchar = 1;
			expre();
			e = RPN_reck(ek);
			if (justchar != ischar_fun) error(23);
			if (e == "arrayerror") error(19);
			if (e == "div0") error(20);
			//ccode("rtn", "", "", e);
			if (ischar_fun != 1)ccode("rtn", "", "", e);
			else {
				if (findtab3(e) == 5 || findtab3(e) == 7 || findtab3(e) == 3) ccode("rtn", "", "", e);
				else {
					int x = atoi(e.c_str());
					char c = (char)x;
					string sss(1, c);

					string ssss = new_var(1);
					temp_name = new_var(0);
					structpartab(7, x, ad, 1);
					ccode("=", to_string(x), "", new_var(0));
					ccode("rtn", "", "", new_var(0));

					//ccode("rtn", "", "", "'" + sss + "'");
				}

			}
			if (symbol != 26) error(6); //����Ϊ ��
			getsym(0);
		}
		else {
			ejump(32);
			error(14);
		}

	}
}

//����䣾 ::= ��������䣾����ѭ����䣾| ��{��������У���}��| 
//			   ����ֵ��䣾;�����з���ֵ����������䣾;  |���޷���ֵ����������䣾;��
//			   ������䣾;����д��䣾;�����գ�;|��������䣾;
//  if  /  while  /  for  /  {  /  ��ʶ��+=/[  /  ��ʶ���� /  ��ʶ��+ �� /  scanf  /  printf  /  ��  /  return
//get symbol ---  return symbol
void statement() {
	switch (symbol)
	{
	case 6: { // if
		if (location == 2 || location == 1 || location == 3) isret = 0;
		ifM();
		break;
	}
	case 8: { // while
		if (location == 2 || location == 1 || location == 3) isret = 0;
		whileM();
		break;
	}
	case 9: { // for
		if (location == 2 || location == 1 || location == 3) isret = 0;
		forM();
		break;
	}
	case 29: { // {  �����
		statements();
		if (symbol != 30) error(7); //����Ϊ }
		getsym(0);
		break;
	}
	case 34: { // ��ʶ��
		if (location == 2 || location == 1 || location == 3) isret = 0;
		if (findtab(token) == 2 || findtab(token) > 100) { // ����
			if (findtab(token) == 2) isarr = 0;
			else isarr = 1;
			assign();
			cout << "������ ��ֵ���" << endl;
			if (symbol != 32) { //����Ϊ ��
				ejump(32);
				error(12);
			}
		}
		else if (findtab(token) == 3) { //int / char ����
			callrtnfun(findtab3(token));
			cout << "������ �з���ֵ�����������" << endl;
			if (symbol != 32) { //����Ϊ ��
				ejump(32);
				error(12);
			}
		}
		else if (findtab(token) == 5) { // void ����
			callvoidfun();
			cout << "������ �޷���ֵ�����������" << endl;
			if (symbol != 32) { //����Ϊ ��
				ejump(32);
				error(12);
			}
		}
		else if (findtab(token) == 1) { 
			ejump(32);	
			error(28);
		}
		else {
			ejump(32);
			error(13);
		}
		getsym(0);
		break;
	}
	case 10: { // scanf
		if (location == 2 || location == 1 || location == 3) isret = 0;
		scanfM();
		cout << "������ �����" << endl;
		if (symbol != 32) { //����Ϊ ��
			ejump(32);
			error(12);
		}
		getsym(0);
		break;
	}
	case 11: { //  printf
		if (location == 2 || location == 1 || location == 3) isret = 0;
		printfM();
		cout << "������ д���" << endl;
		if (symbol != 32) { //����Ϊ ��
			ejump(32);
			error(12);
		}
		getsym(0);
		break;
	}
	case 32: { //  <��> ��
		getsym(0);
		cout << "������ �����" << endl;
		break;
	}
	case 12: { //  return  
		if (location == 1) isret2 = 1;
		if (location == 2 || location == 1 || location == 3) isret = 1;
		returnM();
		cout << "������ �������" << endl;
		if (symbol != 32) { //����Ϊ ��
			ejump(32);
			error(12);
		}
		getsym(0);
		break;
	}
	default: {
		ejump(32);
		error(11);
		getsym(0);
	}
	}
}
//===================================================================================================================
//������ ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program() {
	
	temp_func_name = "";

	location = 0;
	getsym(0);
	if (symbol == 1)  //����Ϊ const --------�ۣ�����˵������
		constins();
	// 0: void    1: int/char function    2: iden   3: main   -1: ?
	selec = judge();
	if (selec == 2) varins(); // --------------------------------�ۣ�����˵������

	while (selec != 3 && selec != -1 && selec != 2) { //------------------------------ {���з���ֵ�������壾 | ���޷���ֵ�������壾}
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
	ccode("func", "void", "", "main");
	getsym(0);
	if (symbol != 25) error(9); //����Ϊ (
	getsym(0);
	if (symbol != 26) error(6);//����Ϊ )
	getsym(0);
	if (symbol != 29) error(10); //����Ϊ {
	location = 3;
	temp_func_name = "main";//*****************************
	cout << "������ ������" << endl;
	statementss();
	if (symbol != 30) error(7); //����Ϊ }
	ccode("set", "", "", endla);
	//if (isret != 1) ccode("rtn", "", "", "");
	structglotab(1, 0, ad, num_para);//**************************
	/*
	while (!ss.empty()) {
		temp_name = ss.front();
		ss.pop_front();
		structglotab(12, 0, ad, 1);//**************************
	}
	*/
	cout << "�� ~ �﷨��������" << endl;
}
//===============================================================================================================
//�������� :: = �ۣ������ݣ��޷���������
//get symbol
void integer() {
	if (symbol == 35) { //����Ϊ�޷�������
		return;
	}
	else if (symbol == 13) { //����Ϊ�Ӻ�
		getsym(0);
		if (symbol == 35) {
			return;
		}
	}
	else if (symbol == 14) { //����Ϊ����
		getsym(0);
		if (symbol == 35) {
			num = -num;
			return;
		}
	}
	else {
		error(3);
		num = 0; //Ĭ��Ϊ 0
	}
}
//������˵���� ::= const���������壾; { const���������壾; }
//get const ---- return symbol
void constins() {
	getsym(0);
	if (symbol == 2) //����Ϊ int
		constintdef();
	else if (symbol == 3) //����Ϊ char
		constchardef();
	else {
		error(0);
		ejump(32);
	}
	if (symbol == 32) { //����Ϊ ��
		cout << "������ ����˵��" << endl;
	}

	while (true) {
		getsym(0);
		if (symbol != 1) break; //����Ϊ const
		getsym(0);
		if (symbol == 2) //����Ϊ int
			constintdef();
		else if (symbol == 3) //����Ϊ char
			constchardef();
		else {
			error(0);
			ejump(32);
		}
		if (symbol == 32)  //����Ϊ ��
			cout << "������ ����˵��" << endl;
	}
}
//���������壾 ::= int����ʶ��������������{,����ʶ��������������} | char����ʶ���������ַ���{ ,����ʶ���������ַ��� }
//get int/char --- return symbol
void constintdef() {
	getsym(0);
	if (symbol != 34) { //�����Ǳ�ʶ��
		error(1);
		ejump(32);
		return;
	}
	temp_name = token;//******************************************
	if (test(token) == false) error(27);
	getsym(0);
	if (symbol != 33) { //������ = 
		error(2);
	}
	getsym(0);
	integer();
	if (location == 0) structglotab(4 , num, ad, 1);//*******************************************
	else structpartab(4, num, ad, 1);//*******************************************
	ccode("const", "int", to_string(num), temp_name);
	while (true) {
		getsym(0);
		if (symbol != 31) break;  //����Ϊ ��
		getsym(0);
		if (symbol != 34) { //�����Ǳ�ʶ��
			error(1);
			ejump(32);
			return;
		}
		temp_name = token; //************************************************
		if (test(token) == false) error(27);
		getsym(0);
		if (symbol != 33) { //������ =
			error(2);
		}
		getsym(0);
		integer();
		if (location == 0) structglotab(4 , num, ad, 1);//*******************************************
		else structpartab(4, num, ad, 1);//*******************************************
		ccode("const", "int", to_string(num), temp_name);
		//if (num >= 0) ccode("const", "int", to_string(num), temp_name);
		//else {
			//string n = token;
			//n = "-" + n;
			//ccode("const", "int", n, temp_name);
		//}
	}
}
void constchardef(){
	getsym(0);
	if (symbol != 34) { //�����Ǳ�ʶ��
		error(1);
		ejump(32);
		return;
	}
	temp_name = token;//******************************************
	if (test(token) == false) error(27);
	getsym(0);
	if (symbol != 33) { //������ = 
		error(2);
	}
	getsym(0);
	if (symbol != 38) { //������ �ַ�
		error(4);
		temp_char = ' ';
	}
	else temp_char = token[0];
	if (location == 0) structglotab(5, temp_char, ad, 1);//*******************************************
	else structpartab(5, temp_char, ad, 1);//*******************************************
	string s(1, temp_char);
	ccode("const", "char", s, temp_name);
	while (true) {
		getsym(0);
		if (symbol != 31) break;  //����Ϊ ��
		getsym(0);
		if (symbol != 34) { //�����Ǳ�ʶ��
			error(1);
			ejump(32);
			return;
		}

		temp_name = token;//******************************************
		if (test(token) == false) error(27);
		getsym(0);
		if (symbol != 33) { //������ =
			error(2);
		}
		getsym(0);
		if (symbol != 38) { //������ �ַ�
			error(4);
			temp_char = ' ';
		}
		else temp_char = token[0];
		if (location == 0) structglotab(5, temp_char, ad, 1);//*******************************************
		else structpartab(5, temp_char, ad, 1);//*******************************************
		string s(1, temp_char);
		ccode("const", "char", s, temp_name);
	}
}
//===============================================================================================================
//������˵���� :: = ���������壾; {���������壾; }
//get int/char ---- return symbol
void varins() {
	do {
		vardef();
		if (symbol != 32); //����Ϊ ��
		cout << "������ ����˵��" << endl;
		getsym(0);
		selec = judge();
	} while (selec == 2);
}
//���������壾::= (int|char)(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]��)} //�޷������������0 
//not get --- return symbol
void vardef() {
	int ischar = 0;
	if (symbol == 3) ischar = 1;
	do {
		getsym(0);
		if (symbol != 34) { //�����Ǳ�ʶ��
			error(1);
			ejump(32);
			return;
		}
		temp_name = token; //**********************************
		if (test(token) == false) error(27);
		getsym(0);
		if (symbol == 27) { //����Ϊ [
			getsym(0);
			if (symbol == 35) { //����Ϊ �޷�������
				integer();
				if (num <= 0) {
					error(17);
					num = 1;
				}
				if (location == 0) structglotab(8 + ischar, 0, ad, num);//*******************************************
				else structpartab(8 + ischar , 0, ad, num);//******************************************
				if (ischar == 0) ccode("var[]", "int", token, temp_name);
				if (ischar == 1) ccode("var[]", "char", token, temp_name);
			}
			else error(17);
			getsym(0);
			if (symbol != 28) { //����Ϊ ]
				error(5);
				ejump(32);
				return;
			}
			getsym(0);
		}
		else {
			if (location == 0) structglotab(6 + ischar, 0, ad, num);//*******************************************
			else structpartab(6 + ischar, 0, ad, num);//******************************************
			if (ischar == 0) ccode("var", "int", "", temp_name);
			if (ischar == 1) ccode("var", "char", "", temp_name);
		}
		
	} while (symbol == 31); //����Ϊ ��
}
//===================================================================================================
//��������    :: = �����ͱ�ʶ��������ʶ����{ ,�����ͱ�ʶ��������ʶ���� }
//return symbol
void paratab() {
	
	num_para = 0;
	while (true) {
		getsym(0);
		if (symbol == 2) { //����Ϊint
			getsym(0);
			if (symbol == 34) { //����Ϊ ��ʶ��
				temp_name = token;
				ptype = "6" + ptype;
				ccode("para", "int", "", token);
				num_para++;
				structpartab(6, 0, ad, 0);//***********************************
				getsym(0);
			}
			else {
				ejump(31);
				error(1);
			}
		}
		else if (symbol == 3) { //����Ϊchar
			getsym(0);
			if (symbol == 34) { //����Ϊ ��ʶ��
				temp_name = token;
				ptype = "7" + ptype;
				ccode("para", "char", "", token);
				num_para++;
				structpartab(7, 0, ad, 0); //*****************************************
				getsym(0);
			}
		}
		else {
			ejump(31);
			error(25);
		}
		if (symbol == 31) continue; // ����Ϊ ��
		else {
			cout << "������ ������" << endl;
			break;
		}
	}

}
//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����䣾��
//return symbol
void statementss() {
	getsym(0);
	if (symbol == 1) {  //����Ϊ const --------�ۣ�����˵������
		constins();
	}

	if (symbol == 2 || symbol == 3) {
		varins();
	}
	structglotab(2 + ischar_fun, 0, ad, num_para);//**************************
	while (symbol == 6 || symbol == 8 || symbol == 9 || symbol == 29 ||
		symbol == 34 || symbol == 10 || symbol == 11 || symbol == 32 || symbol == 12) {
		statement();
	}
}
//���з���ֵ�������壾  ::=  (int | char) <��ʶ��>[��(������������)��]��{����������䣾��}��
//���޷���ֵ�������壾  ::=   void ����ʶ���� [(������������)��]��{����������䣾��}��
//get symbol---return symbol
void rtnfundef() {
	int x = 1;
	if (symbol == 2) ischar_fun = 0;
	if (symbol == 3) ischar_fun = 1;
	getsym(0);
	if (symbol != 34); //����Ϊ ��ʶ�� ---����judge�д���
	temp_func_name = token;//*****************************
	//structglotab(2 + ischar_fun, 0, ad, num_para);//**************************
	if (ischar_fun == 0) ccode("func", "int", "", token);
	if (ischar_fun == 1) ccode("func", "char", "", token);
	getsym(0);
	if (symbol == 25) { //����Ϊ ��
		paratab();
		if (symbol != 26) error(6); //����Ϊ ��
		getsym(0);
	}
	else x = 0;
	if (symbol != 29); //����Ϊ {  --------����judge�д���
	cout << "������ �з���ֵ��������" << endl;
	location = 1; //*************************************
	isret2 = 0; isret = 0;
	statementss();
	//structglotab(2 + ischar_fun, 0, ad, num_para);//**************************
	if (symbol != 30) error(7); //����Ϊ }
	if (isret2 == 0) error(18);
	if (isret != 1) ccode("rtn", "", "", "0");
	location = 0; //****************************
	isret2 = 0; isret = 0;
	structglotab(2 + ischar_fun, 0, x, num_para);//**************************
	partab.clear();
	getsym(0);
}
void voidfundef() {
	int x = 1;
	getsym(0);
	ischar_fun = -1;
	if (symbol != 34); //����Ϊ ��ʶ�� ---����judge�д���
	temp_func_name = token;//*****************************
	structglotab(1, 0, x, num_para);//**************************
	getsym(0);
	ccode("func", "void", "", temp_func_name);
	if (symbol == 25) { //����Ϊ ��
		paratab();
		if (symbol != 26)error(6); //����Ϊ ��
		getsym(0);
	}
	else x = 0;
	if (symbol != 29); //����Ϊ {  --------����judge�д���

	cout << "������ �޷���ֵ��������" << endl;
	location = 2; //*************************************
	isret = 0;
	statementss();
	if (symbol != 30) error(7); //����Ϊ }
	if (isret != 1) ccode("rtn", "", "", "");
	location = 0; //****************************
	isret = 0;
	partab.clear();
	getsym(0);
}

void main2() {
	init();
	word_end();
	midecodeinit();
	
	program();
	test();
	mid_end();
	if (have_error == 1) {
		cout << "***������ڴ���****" << endl;
		system("pause");
		exit(0);
	}
	cout << "***���������������MIPS����****" << endl;
	getchar();
	getchar();
}

