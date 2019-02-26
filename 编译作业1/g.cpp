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
	int type = -1;  // 1��void���� 2��int/char���� 4��const���� 6��var���� 8������ 10��para����
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
};
struct glo {
	int type = -1;  // 1��void���� 2��int���� 3��char���� 4��const���� 6��var���� 8������ 10��para����
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
	map<string, par>  partab; //�ֲ�������
};
int location;
glo temp_glo;
par temp_par;
string temp_name;
string temp_func_name;
char temp_char;
int num_para;
map <string, glo> tab; //ȫ�ֱ�����
map <string, par> partab; //�ֱ�����
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

//���
int findtab() {

	map <string, par>::iterator it1;
	it1 = partab.find(token);
	if (it1 != partab.end()) {
		if (partab[token].type == 4)return 1;//����
		if (partab[token].type == 6)return 2;//����
		if (partab[token].type == 2)return 3;//int/char����
		if (partab[token].type == 1)return 5;//void
	}
	else {
		map <string, glo>::iterator it2;
		it2 = tab.find(token);
		if (it2 != tab.end()) {
			if (tab[token].type == 4)return 1;//����
			if (tab[token].type == 6)return 2;//����
			if (tab[token].type == 2)return 3;//int/char����
			if (tab[token].type == 1)return 5;//void
		}
		else {
			cout << "�Ҳ������� �� " << token;
			return 6;
		}
	}
}

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

//������
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
	cout << temp_name << "  ���" << endl;
	if (t == 4) ad = ad + 4 * s;
}



//�ж�
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

	getsym();//����Ϊ ��ʶ��/main

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

	getsym();//��������ͱ�������ķֽ�� 
	if (symbol == 25 || symbol == 29) { // int / char ����
		lc = temp_lc;
		lines = temp_lines;
		resultn = temp_resultn;
		symbol = temp_symbol;
		memset(token, 0, sizeof(token));
		strcpy(token, temp_token.c_str());
		temp_token = "";
		return 1;
	}
	if (symbol == 27 || symbol == 31 || symbol == 32) { //��ʶ��
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

//����
void ejump(int end) {
	do {
		getsym();
		if (symbol != end && lc == ll) symbol = end;
	} while (symbol != end);
}
void error(int n) {
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
}


//�����ӣ�::= ����ʶ����������ʶ������[�������ʽ����]��|��(�������ʽ����)������������|���ַ��������з���ֵ����������䣾
//get symbol --- return symbol
void factor() {
	if (symbol == 34) { //����Ϊ ��ʶ��
		if (findtab() == 1 || findtab() == 2) { //�Ǳ�������
			getsym();
			if (symbol == 27) { //����Ϊ [
				expre();
				if (symbol == 28) { //����Ϊ ]
					getsym();
					return;
				}
			}
		}
		else if (findtab() == 3) { //�� int/char����
			callrtnfun();
			return;
		}
		else;
	}
	else if (symbol == 25) { //����Ϊ ��
		expre();
		if (symbol == 26) { //����Ϊ �� 
			getsym();
			return;
		}
		else;
	}
	else if (symbol == 38) { //����Ϊ �ַ�
		getsym();
		return;
	}
	else if (symbol == 13 || symbol == 14 || symbol == 35) { //����Ϊ + - ����
		integer();
		getsym();
		return;
	}
	else;
}

//���::= �����ӣ�{���˷�������������ӣ�}
//get symbol --- return symbol
void term() {
	factor();
	while (symbol == 15 || symbol == 16) { //����Ϊ * /
		getsym();
		factor();
	}
}

//�����ʽ�� ::= �ۣ������ݣ��{���ӷ�����������}
//not need symbol --- return symbol
void expre() {
	getsym();
	if (symbol == 13 || symbol == 14) { //����Ϊ + -
		getsym();
		term();
	}
	else term();
	while (symbol == 13 || symbol == 14) {
		getsym();
		term();
	}
}

//�������� ::=  �����ʽ������ϵ������������ʽ���������ʽ�� 
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

//��������䣾::= if ��(������������)������䣾[else����䣾]
//not get symbol --- return symbol
void ifM() {
	getsym();
	if (symbol != 25) error(9); //����Ϊ ��
	condition();
	if (symbol != 26) error(6); //����Ϊ  ��
	cout << "������ �������" << endl;
	getsym();
	statement();
	if (symbol == 7) { //����Ϊ else
		getsym();
		statement();
	}
}

//��ѭ�����1��  :: = while ��(������������)������䣾
//not get symbol --- return symbol
void whileM() {
	getsym();
	if (symbol != 25) error(9); //����Ϊ ��
	condition();
	if (symbol != 26) error(9); //����Ϊ  ��
	cout << "������ ѭ�����" << endl;
	getsym();
	statement();
}

//��ѭ�����2��::=for'('����ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)���޷���������')'����䣾
//not get symbol --- return symbol
void forM() {
	getsym();
	if (symbol != 25) error(9); //����Ϊ ��
	getsym();
	if (symbol != 34)error(1); //����Ϊ ��ʶ��
	getsym();
	if (symbol != 33)error(2); //����Ϊ =
	expre();
	if (symbol != 32)error(12); //����Ϊ ��
	condition();
	if (symbol != 32)error(12);  //����Ϊ ��
	getsym();
	if (symbol != 34)error(1); //����Ϊ ��ʶ��
	getsym();
	if (symbol != 33)error(2); //����Ϊ =
	getsym();
	if (symbol != 34)error(1); //����Ϊ ��ʶ��
	getsym();
	if (symbol != 13 && symbol != 14) error(16); //����Ϊ + -
	getsym();
	if (symbol != 35) error(9); //����Ϊ �޷�������
	getsym();
	if (symbol != 26) error(6); //����Ϊ ��
	cout << "������ ѭ�����" << endl;
	getsym();
	statement();
}

//������У�::= ������䣾��
//not get symbol --- return symbol
void statements() {
	getsym();
	while (symbol == 6 || symbol == 8 || symbol == 9 || symbol == 29 ||
		symbol == 34 || symbol == 10 || symbol == 11 || symbol == 32 || symbol == 12) {
		statement();
	}
}

//����ֵ��䣾   :: =����ʶ����[��[�������ʽ����]��] = �����ʽ��
//not get symbol --- return symbol
void assign() {
	getsym();
	if (symbol == 27) { // ����Ϊ [
		expre();
		if (symbol != 28) error(5); //����Ϊ ]
		getsym();
	}
	if (symbol != 33); //����Ϊ =
	expre();
}

//��ֵ������ ::= �����ʽ��{,�����ʽ��}
//not need symbol --- return symbol
void reparatab() {
	expre();
	while (symbol == 31) { //����Ϊ ��
		expre();
	}
}

//���з���ֵ����������䣾 ::= ����ʶ����[��(����ֵ��������)'] 
//not need symbol --- return symbol
void callrtnfun() {
	getsym();
	if (symbol == 25) { //����Ϊ ��
		reparatab();
		if (symbol != 26) error(9); //����Ϊ  ��
		getsym();
		return;
	}
	else return;
}
//���޷���ֵ����������䣾 ::= ����ʶ����[��(����ֵ��������)']
//not need symbol --- return symbol
void callvoidfun() {
	getsym();
	if (symbol == 25) { //����Ϊ ��
		reparatab();
		if (symbol != 26) error(6); //����Ϊ  ��
		getsym();
		return;
	}
	else return;
}

//������䣾::=  scanf��(������ʶ����{,����ʶ����}��)��
//not need symbol --- return symbol
void scanfM() {
	getsym();
	if (symbol != 25) error(9); //����Ϊ ��
	getsym();
	if (symbol != 34) error(1); //����Ϊ ��ʶ��
	getsym();
	while (symbol == 31) { //����Ϊ ��
		getsym();
		if (symbol != 34) break; //����Ϊ ��ʶ��
		getsym();
	}
	if (symbol != 26) error(6); //����Ϊ )
	getsym();
}

//��д��䣾::= printf ��(�� ���ַ�����,�����ʽ�� ��)��| printf ��(�����ַ����� ��)��| printf ��(�������ʽ����)��
//not need symbol --- return symbol
void printfM() {
	getsym();
	if (symbol != 25)error(9); //����Ϊ ��

	temp_lc = lc;
	temp_lines = lines;
	temp_resultn = resultn;
	temp_symbol = symbol;
	temp_token = token;

	getsym();
	if (symbol == 37) { //����Ϊ �ַ���
		getsym();
		if (symbol == 26) { //����Ϊ ��
			getsym();
			return;
		}
		if (symbol != 31) { //����Ϊ ��
			error(15);
		}
		expre();
		if (symbol != 26) error(6); //����Ϊ )
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

		if (symbol == 26) { //����Ϊ ��
			getsym();
			return;
		}
		else error(6);
		return;
	}
}

//��������䣾   ::=  return[��(�������ʽ����)��] 
//not need symbol --- return symbol
void returnM() {
	getsym();
	if (symbol == 32) { //����Ϊ ��
		return;
	}
	else if (symbol == 25) { //����Ϊ ��
		expre();
		if (symbol != 26) error(6); //����Ϊ ��
		getsym();
	}
	else {
		ejump(32);
		error(14);
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
	case 29: { // {  �����
		statements();
		if (symbol != 30) error(7); //����Ϊ }
		getsym();
		break;
	}
	case 34: { // ��ʶ��
		if (findtab() == 2) { // ����
			assign();
			cout << "������ ��ֵ���" << endl;
			if (symbol != 32) { //����Ϊ ��
				ejump(32);
				error(12);
			}
		}
		else if (findtab() == 3) { //int / char ����
			callrtnfun();
			cout << "������ �з���ֵ�����������" << endl;
			if (symbol != 32) { //����Ϊ ��
				ejump(32);
				error(12);
			}
		}
		else if (findtab() == 5) { // void ����
			callvoidfun();
			cout << "������ �޷���ֵ�����������" << endl;
			if (symbol != 32) { //����Ϊ ��
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
		cout << "������ �����" << endl;
		if (symbol != 32) { //����Ϊ ��
			ejump(32);
			error(12);
		}
		getsym();
		break;
	}
	case 11: { //  printf
		printfM();
		cout << "������ д���" << endl;
		if (symbol != 32) { //����Ϊ ��
			ejump(32);
			error(12);
		}
		getsym();
		break;
	}
	case 32: { //  <��> ��
		getsym();
		cout << "������ �����" << endl;
		break;
	}
	case 12: { //  return  
		returnM();
		cout << "������ �������" << endl;
		if (symbol != 32) { //����Ϊ ��
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
//������ ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program() {
	location = 0;
	getsym();
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
	getsym();
	if (symbol != 25) error(9); //����Ϊ (
	getsym();
	if (symbol != 26) error(6);//����Ϊ )
	getsym();
	if (symbol != 29) error(10); //����Ϊ {
	location = 1;
	temp_func_name = "main";//*****************************
	cout << "������ ������" << endl;
	statementss();
	if (symbol != 30) error(7); //����Ϊ }
	structglotab(1, 0, ad, num_para);//**************************
	cout << "�� ~ �﷨��������" << endl;
}
//===============================================================================================================
//�������� :: = �ۣ������ݣ��޷���������
//get symbol
void integer() {
	if (symbol == 35) { //����Ϊ�޷�������
		return;
	}
	else if (symbol == 13 ) { //����Ϊ�Ӻ�
		getsym();
		if (symbol == 35) {
			return;
		}
	}
	else if (symbol == 14) { //����Ϊ����
		getsym();
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
	getsym();
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
		getsym();
		if (symbol != 1) break; //����Ϊ const
		getsym();
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
	getsym();
	if (symbol != 34) { //�����Ǳ�ʶ��
		error(1);
		ejump(32);
		return;
	}
	temp_name = token;//******************************************
	getsym();
	if (symbol != 33) { //������ = 
		error(2);
	}
	getsym();
	integer();
	if (location == 0) structglotab(4, num, ad, 1);//*******************************************
	else structpartab(4, num, ad, 1);//*******************************************
	while (true) {
		getsym();
		if (symbol != 31) break;  //����Ϊ ��
		getsym();
		if (symbol != 34) { //�����Ǳ�ʶ��
			error(1);
			ejump(32);
			return;
		}
		temp_name = token; //************************************************
		getsym();
		if (symbol != 33) { //������ =
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
	if (symbol != 34) { //�����Ǳ�ʶ��
		error(1);
		ejump(32);
		return;
	}
	temp_name = token;//******************************************
	getsym();
	if (symbol != 33) { //������ = 
		error(2);
	}
	getsym();
	if (symbol != 38) { //������ �ַ�
		error(4);
		temp_char = ' ';
	}
	else temp_char = token[0];
	if (location == 0) structglotab(4, temp_char, ad, 1);//*******************************************
	else structpartab(4, temp_char, ad, 1);//*******************************************
	while (true) {
		getsym();
		if (symbol != 31) break;  //����Ϊ ��
		getsym();
		if (symbol != 34) { //�����Ǳ�ʶ��
			error(1);
			ejump(32);
			return;
		}
		temp_name = token;//******************************************
		getsym();
		if (symbol != 33) { //������ =
			error(2);
		}
		getsym();
		if (symbol != 38) { //������ �ַ�
			error(4);
			temp_char = ' ';
		}
		else temp_char = token[0];
		if (location == 0) structglotab(4, temp_char, ad, 1);//*******************************************
		else structpartab(4, temp_char, ad, 1);//*******************************************
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
		getsym();
		selec = judge();
	} while (selec == 2);
}
//���������壾::= (int|char)(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]��)} //�޷������������0 
//not get --- return symbol
void vardef() {
	do {
		getsym();
		if (symbol != 34) { //�����Ǳ�ʶ��
			error(1);
			ejump(32);
			return;
		}
		temp_name = token; //**********************************
		getsym();
		if (symbol == 27) { //����Ϊ [
			getsym();
			if (symbol != 35) { //����Ϊ �޷�������
				integer();
				if (num <= 0) {
					error(17);
					num = 1;
				}
				if (location == 0) structglotab(6, 0, ad, num);//*******************************************
				else structpartab(6, 0, ad, num);//******************************************
			}
			getsym();
			if (symbol != 28) { //����Ϊ ]
				error(5);
				ejump(32);
				return;
			}
			getsym();
		}
		if (location == 0) structglotab(6, 0, ad, num);//*******************************************
		else structpartab(6, 0, ad, num);//******************************************
	} while (symbol == 31); //����Ϊ ��
}
//===================================================================================================
//��������    :: = �����ͱ�ʶ��������ʶ����{ ,�����ͱ�ʶ��������ʶ���� }
//return symbol
void paratab() {
	num_para = 0;
	while (true) {
		getsym();
		if (symbol == 2) { //����Ϊint
			getsym();
			if (symbol == 34) { //����Ϊ ��ʶ��
				num_para++;
				structpartab(6, 0, ad, 0);//***********************************
				getsym();
			}
			else {
				ejump(31);
				error(1);
			}
		}
		else if (symbol == 3) { //����Ϊchar
			getsym();
			if (symbol == 34) { //����Ϊ ��ʶ��
				num_para++;
				structpartab(6, 0, ad, 0); //*****************************************
				getsym();
			}
		}
		else {
			ejump(31);
			error(0);
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
	getsym();
	if (symbol == 1) {  //����Ϊ const --------�ۣ�����˵������
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
//���з���ֵ�������壾  ::=  (int | char) <��ʶ��>[��(������������)��]��{����������䣾��}��
//���޷���ֵ�������壾  ::=   void ����ʶ���� [(������������)��]��{����������䣾��}��
//get symbol---return symbol
void rtnfundef() {
	getsym();
	if (symbol != 34); //����Ϊ ��ʶ�� ---����judge�д���
	temp_func_name = token;//*****************************
	getsym();
	if (symbol == 25) { //����Ϊ ��
		paratab();
		if (symbol != 26) error(6); //����Ϊ ��
		getsym();
	} 
	if (symbol != 29); //����Ϊ {  --------����judge�д���
	cout << "������ �з���ֵ��������" << endl;
	location = 1; //*************************************
	statementss();
	if (symbol != 30) error(7); //����Ϊ }
	location = 0; //****************************
	structglotab(2, 0, ad, num_para);//**************************
	partab.clear();
	getsym();
}
void voidfundef() {
	getsym();
	if (symbol != 34); //����Ϊ ��ʶ�� ---����judge�д���
	temp_func_name = token;//*****************************
	getsym();
	if (symbol == 25) { //����Ϊ ��
		paratab();
		if (symbol != 26)error(6); //����Ϊ ��
		getsym();
	}
	if (symbol != 29); //����Ϊ {  --------����judge�д���

	cout << "������ �޷���ֵ��������" << endl;
	location = 1; //*************************************
	statementss();
	if (symbol != 30) error(7); //����Ϊ }
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