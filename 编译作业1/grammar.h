#pragma once

#include<string>
#include<map>
using namespace std;

extern map<string,string> ss;

extern struct par {
	int type = -1;  // 1��void���� 2��int/char���� 4��const���� 6��var���� 8������ 10��para����
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
};
extern struct glo {
	int type = -1;  // 1��void���� 2��int���� 3��char���� 4��const���� 6��var���� 8������ 10��para����
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
	map<string, par>  partab; //�ֲ�������
};

extern map <string, glo> tab; //ȫ�ֱ�����
extern map <string, par> partab; //�ֱ�����

void main2();

