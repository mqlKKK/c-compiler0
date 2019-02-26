#pragma once

#include<string>
using namespace std;

extern struct par {
	int type = -1;  // 1��void���� 2��int/char���� 4��const���� 6��var���� 8������ 10��para����
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
};
extern struct glo {
	int type = -1;  // 1��void���� 2��int���� 3��char���� 4��const���� 6��var���� 8������ 10��para���� 12���ַ���
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
	map<string, par>  partab; //�ֲ�������
};
extern map <string, glo> tab;
extern map <string, par> partab;

extern glo temp_glo;
extern par temp_par;
extern string temp_name;
extern string temp_func_name;
extern int ad;

int findtab(string token0);
int findtab3(string token0);
string new_var(int i);
string new_label(int i);
void midecodeinit();
void ccode(string op, string arg1, string arg2, string result);
void RPN_struct(int act, string op, string arg, int k);
string RPN_reck(int k);
void mid_end();
void structglotab(int t, int v, int a, int s);
void structpartab(int t, int v, int a, int s);

int findtab4(string token0);