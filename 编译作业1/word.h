#pragma once

extern int num; //��ǰ����������

extern int lc; //�л�����ָ��

extern int ll; //�л���������

extern int lines; //����

extern int resultn; //��ǰ���ĵ�����

extern char token[200];//sym������

extern char line[1024]; //�л�����

extern int symbol; //��ǰ�������ַ���������

void init();
void getsym(int i);
void word_end();
void setmark(int i);
