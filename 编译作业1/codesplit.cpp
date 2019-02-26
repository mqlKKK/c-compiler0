#include<fstream>
#include<sstream>
#include<iostream>
#include<list>
#include<map>
#include<sstream>
#include"code.h"
using namespace std;

#define Bmax 1000
#define linemax 1000

ifstream txt1; //midcode�ļ�
ofstream txt2; //����ļ�

map <string, string>sreg;
map <string, int> vars;

struct Bstr {
	list<string> stc;
};
Bstr B[Bmax];
int Bn;
int bnum = 1;
string gcodeline, codes[linemax];
string ss1, ss2, ss3, ss4, ss5, ss6, ss7;
stringstream gcodeline2;
int line = 0;
int line_main;

void open() {
	txt1.open("midcode.txt");
	if (!txt1.is_open()) {
		cout << "�Ҳ����м�����ļ���" << endl;
		exit(0);
	}
}
void open2() {
	txt2.open("midcode2.txt");
	if (!txt2.is_open()) {
		cout << "�޷���������ļ���" << endl;
		exit(0);
	}
}

void getcode() {
	while (true) {
		getline(txt1, gcodeline);
		if (gcodeline == "") {
			break;
		}
		codes[line++] = gcodeline;
	}
}

void f1(int i, string op) {
	gcodeline2.clear();
	if (op == "scan") gcodeline2.str(codes[i]);
	else  gcodeline2.str(op);
	ss1 = ""; ss2 = ""; ss3 = ""; ss4 = ""; ss5 = ""; ss6 = ""; ss7 = "";
	gcodeline2 >> ss1 >> ss2 >> ss3 >> ss4 >> ss5 >> ss6 >> ss7;
}

int f2(string s1, string s2, string s3) {
	int i;
	for (i = 1; i < line; i++) {
		if (codes[i] == s1 || codes[i] == s2 || codes[i] == s3) {
			return i;
			break;
		}
	}
	return -2;
}

void del(int k) {
	int i, j;
	line--;
	for (i = k; i < line; i++) {
		codes[i] = codes[i + 1];
	}
}

void isnum() {

}

//ɨ��ֿ�
// BZ  BNZ  GOTO  call
void scan() {
	int i, j, k = 1, nextj = 0, mark[linemax];//��¼��������к�
	string label, label2;
	for (i = 0; i < Bmax; i++) mark[i] = -1;

	mark[0] = k++;

	for (i = 1; i < line; i++) {
		f1(i, "scan");
		//if (ss2 == "=" && is)
		if ((ss3 == "()") && (ss1 == "int" || ss1 == "char" || ss1 == "void")) {
			mark[i] = k++;
			continue;
		}
		/*
		if (ss2 == ":") {
			label = ss1; //label : �滻����
			mark[i] = k++;
			while (true) {
				i++;
				f1(i, "scan");
				if (ss2 == ":") {
					label2 = ss1; //lablel2 : ��Ҫ���滻
					for (j = 0; j < line; j++) {
						f1(j, "scan");
						if (ss2 == label2) codes[j] = ss1 + " " + label;
					}
					del(j);
				}
				else {
					i--;
					break;
				}
			}
			continue;
		}
		*/
		if (ss1 == "GOTO" || ss1 == "BZ" || ss1 == "BNZ") {
			i++;
			mark[i] = k++;
		}
	}

	for (i = 0; i <= line; i++) {
		if (mark[i] != -1) {
			Bn++;
			B[Bn].stc.push_back(codes[i]);
		}
		else B[Bn].stc.push_back(codes[i]);
	}
	list <string>::iterator it1;
	for (i = 1; i <= Bn; i++) {
		cout << "B " << i << " :" << endl;
		for (it1 = B[i].stc.begin(); it1 != B[i].stc.end(); it1++) cout << *it1 << endl;
		cout << "-------------------" << endl;
	}
}

//���������ӱ��ʽ
//���һ�����A�Ǳ��ʽ
//����Ѱ��
//���A.var3���ı䣨������ss1��λ�ã� ֹͣѰ��
//����ҵ����B��A����ͬ��var1��var2��op  ɾ��B���������������B.var3�滻ΪA.var3
void del_com_expre() {
	list <string>::iterator it, it2, itmark;
	for (int i = 1; i <= Bn; i++) { //����ÿ����������ӱ��ʽ
		for (itmark = B[i].stc.begin(); itmark != B[i].stc.end(); itmark++) {//�������ÿһ��
			f1(1, *itmark);
			if (ss2 == "=") { // ����Ǳ��ʽ
				string var1 = ss3;
				string var2 = ss5;
				string op = ss4;
				string var3 = ss1;
				for (it = itmark; it != B[i].stc.end(); it++) { //����Ѱ�Ҳ�����___________________________________________________
					if (it == itmark) continue;
					f1(1, *it);
					if (ss1 == var3) break;
					if (ss2 == "=" && var1 == ss3 && var2 == ss5 && op == ss4 && ss3 != "RET" && ss1[0] == '@') { //�ҵ�����ͬ�ı��ʽ
						string varx = ss1;
						it = B[i].stc.erase(it);
						for (it2 = it; it2 != B[i].stc.end(); it2++) { //�滻var3
							f1(1, *it2);
							if (ss1 == varx) *it2 = var3 + " " + ss2 + " " + ss3 + " " + ss4 + " " + ss5 + " " + ss6 + " " + ss7;
							if (ss2 == varx) *it2 = ss1 + " " + var3 + " " + ss3 + " " + ss4 + " " + ss5 + " " + ss6 + " " + ss7;
							if (ss3 == varx) *it2 = ss1 + " " + ss2 + " " + var3 + " " + ss4 + " " + ss5 + " " + ss6 + " " + ss7;
							if (ss4 == varx) *it2 = ss1 + " " + ss2 + " " + ss3 + " " + var3 + " " + ss5 + " " + ss6 + " " + ss7;
							if (ss5 == varx) *it2 = ss1 + " " + ss2 + " " + ss3 + " " + ss4 + " " + var3 + " " + ss6 + " " + ss7;
							if (ss6 == varx) *it2 = ss1 + " " + ss2 + " " + ss3 + " " + ss4 + " " + ss5 + " " + var3 + " " + ss7;
							if (ss7 == varx) *it2 = ss1 + " " + ss2 + " " + ss3 + " " + ss4 + " " + ss5 + " " + ss6 + " " + var3;
						}
					}
				}//________________________________________________________________________________________________________________
			}
		}
	}
	//_______________________________________________������____________________________________________________________________
	cout << "--------------------------����-------------------------" << endl;
	for (int i = 1; i <= Bn; i++) {
		cout << "B " << i << " :" << endl;
		for (it = B[i].stc.begin(); it != B[i].stc.end(); it++) cout << *it << endl;
		cout << "-------------------" << endl;
	}
}

//ȫ�ּĴ������� ���ü�����
//ɨ��ÿһ���飬��¼ÿһ�����������ô��� ����
void reg_plant() {
	for (int i = 1; i <= Bn; i++) {
		list <string>::iterator it, it2, itmark;
		for (it = B[i].stc.begin(); it != B[i].stc.end(); it++) {
			f1(1, *it);
			//��ֵ���1: a = x + y
			if (ss2 == "=" && (ss4 == "+" || ss4 == "-" || ss4 == "*" || ss4 == "/")) {
				if (findtab3(ss1) != -1 && !(ss1[0] >= '0' && ss1[0] <= '9')) vars[ss1]++;
				if (findtab3(ss3) != -1 && !(ss3[0] >= '0' && ss3[0] <= '9')) vars[ss3]++;
				if (findtab3(ss5) != -1 && !(ss5[0] >= '0' && ss5[0] <= '9')) vars[ss5]++;
				continue;
			}
			//��ֵ���2: a = b
			if (ss2 == "=" && ss4 == "") {
				if (findtab3(ss1) != -1 && !(ss1[0] >= '0' && ss1[0] <= '9')) vars[ss1]++;
				if (findtab3(ss3) != -1 && !(ss3[0] >= '0' && ss3[0] <= '9')) vars[ss3]++;
				continue;
			}
			//��ֵ���3: a [ x ] = b
			if (ss4 == "]") {
				if (findtab3(ss6) != -1 && !(ss6[0] >= '0' && ss6[0] <= '9')) vars[ss6]++;
				continue;
			}
			//��ֵ���4: t = a [ x ]
			if (ss4 == "[") {
				if (findtab3(ss1) != -1 && !(ss1[0] >= '0' && ss1[0] <= '9')) vars[ss1]++;
				continue;
			}
			//����1: a == b  
			if (ss2 == "==" || ss2 == "!=" || ss2 == "<" || ss2 == "<=" || ss2 == ">" || ss2 == ">=") {
				if (findtab3(ss1) != -1 && !(ss1[0] >= '0' && ss1[0] <= '9')) vars[ss1]++;
				if (findtab3(ss3) != -1 && !(ss3[0] >= '0' && ss3[0] <= '9')) vars[ss3]++;
				continue;
			}
			//�������
			if (ss1 == "push") {
				if (findtab3(ss2) != -1 && !(ss2[0] >= '0' && ss2[0] <= '9')) vars[ss2]++;
				continue;
			}
		}
	}
	
	map<string, int>::iterator it = vars.begin();
	int maxnum = 0;
	while (it != vars.end())
	{
		if (it->second > maxnum) maxnum = it->second;
		it++;
	}
	it = vars.begin();
	int  x = 0;
	
	while (true) {
		if (it->second == maxnum) {
			cout << it->first << " : " << it->second << endl;
			if (x < 8) {
				sreg[it->first] = "$s" + to_string(x);
				x++;
			}
			else {
				cout << "�Ż�����" << endl;
				break;
			}
		}
		it++;
		if (it == vars.end()) {
			maxnum--;
			if (maxnum == 0) {
				cout << "�Ż�����" << endl;
				break;
			}
			it = vars.begin();
		}	
	}

}

//д��
void writetxt() {
	open2();
	for (int i = 1; i <= Bn; i++) {
		list <string>::iterator it, it2, itmark;
		for (it = B[i].stc.begin(); it != B[i].stc.end(); it++) {
			txt2 << *it << endl;
		}
	}
	txt2.close();
}

void main3() {
	open();
	getcode();
	scan();
	del_com_expre();
	reg_plant();
	writetxt();
}
