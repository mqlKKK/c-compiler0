#pragma once

#include<string>
#include<map>
using namespace std;

extern map<string,string> ss;

extern struct par {
	int type = -1;  // 1：void函数 2：int/char函数 4：const常量 6：var变量 8：数组 10：para参数
	int value = -1;
	int addr = -1;
	int size = -1;  // 函数：参数个数   数组：元素个数  其他：-1
};
extern struct glo {
	int type = -1;  // 1：void函数 2：int函数 3：char函数 4：const常量 6：var变量 8：数组 10：para参数
	int value = -1;
	int addr = -1;
	int size = -1;  // 函数：参数个数   数组：元素个数  其他：-1
	map<string, par>  partab; //局部变量表
};

extern map <string, glo> tab; //全局变量表
extern map <string, par> partab; //局变量表

void main2();

