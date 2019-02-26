#include<fstream>
#include<sstream>
#include<iostream>
#include <algorithm>
#include<string>
#include<cstring>
#include<map>
#include<queue>
#include<vector>
#include"grammar.h"
#include"great.h"

using namespace std;

ifstream midcodefile; //midcode�ļ�
ofstream mips; //����ļ�
string codeline, s1, s2, s3, s4, s5, s6, s7, temp;//��¼���ڶ����к��е�ÿһ����
stringstream codeline2;
map <string, map <string, int>> records; // ջ
map <string, int> regs; //�Ĵ���
vector <string> glob; //ȫ�ֱ���
queue <string> parms; //����ջ
string loc_fun;//��ǰ���ں���
int fp_offset;//���fp��ƫ���������ڼ�¼�����;ֲ�������

//��ʼ��
void ready(string sss, string ssss) {
	if (true) {
		regs["$zero"] = 0;
		regs["$at"] = 0;
		regs["$v0"] = 0;
		regs["$v1"] = 0;
		regs["$a0"] = 0;
		regs["$a1"] = 0;
		regs["$a2"] = 0;
		regs["$a3"] = 0;
		regs["$t0"] = 0;
		regs["$t1"] = 0;
		regs["$t2"] = 0;
		regs["$t3"] = 0;
		regs["$t4"] = 0;
		regs["$t5"] = 0;
		regs["$t6"] = 0;
		regs["$t7"] = 0;
		regs["$t8"] = 0;
		regs["$t9"] = 0;
		regs["$s1"] = 0;
		regs["$s2"] = 0;
		regs["$s3"] = 0;
		regs["$s4"] = 0;
		regs["$s5"] = 0;
		regs["$s6"] = 0;
		regs["$s7"] = 0;
		regs["$gp"] = 0;
		regs["$sp"] = 0;
		regs["$fp"] = 0;
		regs["$ra"] = 0;
	}
	midcodefile.open(sss);
	if (!midcodefile.is_open()) {
		cout << "�Ҳ����м�����ļ���" << endl;
		exit(0);
	}
	mips.open(ssss);
	if (!mips.is_open()) {
		cout << "�޷���������ļ���" << endl;
		exit(0);
	}
}

//���ж��м�����ļ�
void getmidcode() {
	getline(midcodefile,codeline);
	codeline2.clear();
	codeline2.str(codeline);
	s1 = ""; s2 = ""; s3 = ""; s4 = ""; s5 = ""; s6 = ""; s7 = "";
	codeline2 >> s1 >> s2 >> s3 >> s4 >> s5 >> s6 >> s7;
}

//������������ر�������
int findtab2(string id) {	
	map <string, par>::iterator it1;
	it1 = tab[loc_fun].partab.find(id); //����Ѱ�Ҿֲ�����
	if (it1 != tab[loc_fun].partab.end()) {  //����ҵ��ˣ����ر�������
		return tab[loc_fun].partab[id].type;
	}
	else {
		map <string, glo>::iterator it2; //����ֲ�������û���ҵ�����ȫ�ֱ�������
		it2 = tab.find(id);
		if (it2 != tab.end()) {
			return tab[id].type;
		}
		else { //�������û�ҵ�������0 ���������ַ�����
			cout << "�Ҳ������� �� " << id << endl;
			return 0;
		}
	}
}

//�Ĵ�������
//���룺 ����ָ��i   �Ĵ���reg    
//��������Է���һ�����еļĴ���
string regact(string i, string reg) {
	//ռ�üĴ���
	if (i == "to use") {
		regs[reg] = 1;
	}
	//�ͷżĴ���
	if (i == "to free") {
		regs[reg] = 0;
	}
	//��ѯ�Ĵ���
	if (i == "to check") {
		if (regs[reg] == 1) return "no";
		else return "yes";
	}
	//���ؿ��е���ʱ�Ĵ���
	if (i == "get reg") {
		int n = 0;
		while (regs["$t" + to_string(n)] != 0) n++;
		regact("to use", "$t" + to_string(n));
		return "$t" + to_string(n);
	}
	return "";
}

//���ڴ��ж�ȡ���ݷŵ��Ĵ���
//���룺����ָ��i   ��ʶ��s   ָ���Ĵ���reg   �Ƿ�ָ���Ĵ���op
//��������ش�����ݵļĴ���
string load(string i, string s, string reg, int op, string off) {
	int loc;
	string loc2;
	string reg1, reg2, reg3;
	int n = 0;
	if (i == "to stack" && op == 0) {
		if (records[loc_fun].count(s) == 1) {
			loc = records[loc_fun][s];

			reg1 = regact("get reg", "");
			mips << "lw " << reg1 << ", " << loc << "($fp)" << endl;
			regact("to use", reg1);
			mips << "subi $sp, $sp, 4" << endl;
			mips << "sw " << reg1 << ", " << fp_offset << "($fp)" << endl;
			regact("to free", reg1);
			fp_offset -= 4;
			return "";
		}
		else {
			vector<string>::iterator it = find(glob.begin(), glob.end(), s);
			if (it == glob.end()) {
				cout << "from 151" << endl;
				system("pause");
				exit(0);
			}
			loc2 = *it;
			reg1 = regact("get reg", "");
			mips << "la " << reg1 << ", " << loc2 << endl;
			regact("to use", reg1);
			reg2 = regact("get reg", "");
			mips << "lw " << reg2 << " (" << reg1 << ")" << endl;
			regact("to use", reg2);
			regact("to free", reg1);
			mips << "subi $sp, $sp, 4" << endl;
			mips << "sw " << reg2 << ", " << fp_offset << "($fp)" << endl;
			regact("to free", reg2);
			fp_offset -= 4;
			return "";
		}
	}
	if (i == "to reg" && op == 0) {
		//������
		if (s[0] <= '9' && s[0] >= '0' || s[0] == '-') {
			reg1 = regact("get reg", "");
			mips << "li " << reg1 << " " << s << endl;
			return reg1;
		}
		//�Ǿֲ�����
		else if (records[loc_fun].count(s) == 1) {
			loc = records[loc_fun][s];
			if (off == "") {
				reg1 = regact("get reg", "");
				mips << "lw " << reg1 << " " << loc << "($fp)" << endl;
				return reg1;
			}
			else {
				reg1 = regact("get reg", "");
				mips << "add " << off << " " << off << " " << loc << endl;
				mips << "add " << off << " " << off << " $fp" << endl;
				mips << "lw " << reg1 << " " << "(" << off << ")" << endl;
				return reg1;
			}
		}
		//��ȫ�ֱ���
		else {
			vector<string>::iterator it = find(glob.begin(), glob.end(), s);
			if (it == glob.end()) {
				cout << "��ȫ����û�ҵ����� : " << s << endl;
				system("pause");
				exit(0);
			}
			loc2 = *it;
			if (off == "") {
				reg1 = regact("get reg", "");
				mips << "la " << reg1 << " " << loc2 << endl;
				reg2 = regact("get reg", "");
				mips << "lw " << reg2 << " (" << reg1 << ")" << endl;
				regact("to use", reg2);
				regact("to free", reg1);
				return reg2;
			}
			else {
				reg1 = regact("get reg", "");
				mips << "la " << reg1 << " " << loc2 << endl;

				mips << "sub " << reg1 << " " << reg1 << " " << off << endl;


				reg2 = regact("get reg", "");
				mips << "lw " << reg2 << " (" << reg1 << ")" << endl;
				regact("to use", reg2);
				regact("to free", reg1);
				return reg2;
			}
		}
	}
	if (i == "to reg" && op == 1) {
		//������
		if (s[0] <= '9' && s[0] >= '0' || s[0] == '-') {
			mips << "li " << reg << " " << s << endl;
			return "";
		}
		//�Ǿֲ�����
		else if (records[loc_fun].count(s) == 1) {
			loc = records[loc_fun][s];
			if (off == "") {
				mips << "lw " << reg << " " << loc << "($fp)" << endl;
				return "";
			}
			else {
				reg1 = regact("get reg", "");
				mips << "add " << off << " " << off << " " << loc << endl;
				mips << "add " << off << " " << off << " $fp" << endl;
				mips << "lw " << reg << " " << "(" << off << ")" << endl;
				return "";
			}
		}
		//��ȫ�ֱ���
		else {
			vector<string>::iterator it = find(glob.begin(), glob.end(), s);
			if (it == glob.end()) {
				cout << "��ȫ����û�ҵ����� : " << s << endl;
				system("pause");
				exit(0);
			}
			loc2 = *it;
			if (off == "") {
				reg1 = regact("get reg", "");
				mips << "la " << reg1 << " " << loc2 << endl;
				regact("to use", reg1);
				mips << "lw " << reg << " (" << reg1 << ")" << endl;
				regact("to use", reg);
				regact("to free", reg1);
				return "";
			}
			else {
				reg1 = regact("get reg", "");
				mips << "la " << reg1 << " " << loc2 << endl;
				
				mips << "sub " << reg1 << " " << reg1 << " " << off << endl;

				mips << "lw " << reg << " (" << reg1 << ")" << endl;
				regact("to use", reg);
				regact("to free", reg1);
				return "";
			}
		}
	}
}

//�ӼĴ����ж�ȡ���ݷŵ��ڴ�
//���룺������ݵļĴ���reg  ������ݵı���id
void store(string reg, string id, string off) {
	string regt, regt2;
	//����Ǿֲ�����
	if (records[loc_fun].count(id) == 1) {
		if (off == "") mips << "sw " << reg << " " << records[loc_fun][id] << "($fp)" << endl;
		else {
			regt = regact("get reg", "");
			mips << "li " << regt << " " << " " << records[loc_fun][id] << endl;
			mips << "add " << regt << " " << regt << " " << off << endl;
			//mips << "sw " << reg << " " << regt << "($fp)" << endl;
			mips << "add " << regt << " " << regt << " " << "$fp" << endl;
			mips << "sw " << reg << " " << "(" << regt << ")" << endl;
			regact("to free", regt);
		}
	}
	else {
		vector<string>::iterator it = find(glob.begin(), glob.end(), id);
		//������Ǿֲ�����Ҳ����ȫ�ֱ�������ôӦ�����м����������м����@t����ŵ�ջ����������ú����Ļ��¼
		if (it == glob.end()) {
			mips << "subi $sp $sp 4" << endl;
			mips << "sw " << reg << " " << fp_offset << "($fp)" << endl;
			records[loc_fun][id] = fp_offset;
			fp_offset -= 4;
			return;
		}
		//�����ȫ�ֱ���
		string loc = *it;
		if (off != "") {
			regt = regact("get reg", "");
			mips << "la " << regt << " " << loc << endl;
			mips << "sub " << regt << " " << regt << " " << off << endl;
			mips << "sw " << reg << " " << "(" << regt << ")" << endl;
			regact("to free", regt);
		}
		else {
			regt = regact("get reg", "");
			mips << "la " << regt << " " << loc << endl;
			mips << "sw " << reg << " " << "(" << regt << ")" << endl;
			regact("to free", regt);
		}
	}
}

//���� .data ��  ������һ�� codeline��
void data() {
	mips << ".data" << endl;
	while (true) {
		getmidcode();
		if (s1 == "const" && s2 == "int") {
			mips << s3 << ": .word " << s5 << endl;
			glob.push_back(s3);
		}
		else if (s1 == "const" && s2 == "char") {
			mips << s3 << ": .word " << (int)s5[0] << endl;
			glob.push_back(s3);
		}
		else if (s1 == "array") {
			mips << s3 << ": .space " << 4*atoi(s4.c_str()) << endl;
			glob.push_back(s3);
		}
		else if ((s1 == "int" || s1 == "char") && (s3 != "()")) {
			mips << s2 << ": .space " << "4" << endl;
			glob.push_back(s2);
		}
		else break;
	}
	map<string, string>::iterator it;
	it = ss.begin();
	while (it != ss.end()) {
		mips << it->second << ": .asciiz " << "\"" << it->first << "\"" << endl;
		it++;
	}
}

//���� .text ��
void text() {
	int n = 0;
	string reg1, reg2, reg3;
	mips << ".text" << endl;
	mips << "subi $sp $sp 4" << endl;
	mips << "sw $fp ($sp)" << endl;      //����ɻ��fp
	mips << "move $fp $sp" << endl;      //�»��fp----------------------
	mips << "subi $sp $sp 4" << endl;
	mips << "sw $ra ($sp)" << endl;      //�»�ķ��ص�ַ
	mips << "j main" << endl;
	while (codeline != "") {
		//��������
		if ((s3 == "()") && (s1 == "int" || s1 == "char" || s1 == "void"))
		{
			//mips << "*******----��������" << endl;
			loc_fun = s2;
			map <string, int> record; 
			records[loc_fun] = record; //һ�������Ļ��¼
			mips << s2 << ":";
			fp_offset = -8; // 0(fp) �����һ�����fp     -4(fp) ��ŷ��ص�ַ
			getmidcode();
		}
		//��������
		else if (s1 == "para")
		{
			//mips << "*******----��������" << endl;
			int para_num = 0;
			while (s1 == "para") {
				mips << "subi $sp $sp 4" << endl;
				//����С��4����ʹ�� $a �Ĵ���
				if (para_num <= 3) {
					mips << "sw $a" << para_num << " " << fp_offset << "($fp)" << endl;
					records[loc_fun][s3] = fp_offset;
				}
				//��������4���������ʹ�� $t �Ĵ���
				else {
					mips << "sw $t" << 13 - para_num << " " << fp_offset << "($fp)" << endl;
					regact("to free", "$t" + to_string(13 - para_num));
					records[loc_fun][s3] = fp_offset;
				}
				fp_offset -= 4;
				para_num++;
				getmidcode();
			}
		}
		//�����ڱ�����������
		else if (s1 == "const" || s1 == "int" || s1 == "char" || s1 == "array")
		{		
			//mips << "*******----�����ڱ�����������" << endl;
			while (s1 == "const" || s1 == "int" || s1 == "char" || s1 == "array") {	
				//����ǳ���
				if (s1 == "const") {
					mips << "subi $sp $sp 4" << endl;
					reg1 = regact("get reg", ""); //�õ�һ�����мĴ���
					if (s2 == "int") mips << "li " << reg1 << " " << s5 << endl; //�ѳ������ոշ��صļĴ���  
					else mips << "li " << reg1 << " " << (int)s5[0] << endl; //������ַ���ת������
					regact("to use", reg1); 
					mips << "sw "<<reg1<<" " << fp_offset << "($fp)" << endl; //���Ĵ�����ֵ���浽ջ��
					regact("to free", reg1); //�ͷżĴ���
					records[loc_fun][s3] = fp_offset; //���������뵽�ú����Ļ��¼
					fp_offset -= 4;
				}
				//����Ǳ���
				else if (s1 == "int" || s1 == "char") {
					mips << "subi $sp $sp 4" << endl;
					records[loc_fun][s2] = fp_offset;//���뵽�ú����Ļ��¼
					fp_offset -= 4;
				}
				//���������
				else {
					int arraysize = atoi(s4.c_str()) * 4;
					records[loc_fun][s3] = fp_offset; //�����鿪ʼ��λ�ü��뵽�ú����Ļ��¼
					mips << "subi $sp $sp "<< arraysize << endl; //Ϊ������ջ�����ٿռ�
					for (int i = 0; i < atoi(s4.c_str()); i++) { 
						fp_offset -= 4;
					}
				}
				getmidcode();
			}
		}
		//��ֵ���1: a = x + y
		else if (s2 == "=" && (s4 == "+" || s4 == "-" || s4 == "*" || s4 == "/"))
		{
			//mips << "*******----��ֵ���1��a = x + y" << endl;
			reg1 = load("to reg",s3, "", 0, "");//�����������ص��Ĵ���
			reg2 = load("to reg",s5, "", 0, "");
			
			reg3 = regact("get reg", "");
			if (s4 == "+") mips << "add " << reg3 << " " << reg1 << " " << reg2 << endl;
			if (s4 == "-") mips << "sub " << reg3 << " " << reg1 << " " << reg2 << endl;
			if (s4 == "*") {
				mips << "mul " << reg3 << " " << reg1 << " " << reg2 << endl;
			}
			if (s4 == "/") {
				mips << "div " << reg3 << " " << reg1 << " " << reg2 << endl;
			}
			regact("to use", reg3);
			
			store(reg3, s1, ""); //���Ĵ����еĽ���ŵ��ڴ���

			regact("to free", reg1);
			regact("to free", reg2);
			regact("to free", reg3);
			getmidcode();
		}
		//��ֵ���2: a = b
		else if (s2 == "=" && s4 == "")
		{
			//mips << "*******----��ֵ���2��a = b" << endl;
			if (s3 != "RET") {
				reg1 = load("to reg", s3, "", 0, ""); //�����������ص��Ĵ�����
				store(reg1, s1, ""); //���Ĵ�����ֵ��ŵ��ڴ���
				regact("to free", reg1);
				getmidcode();
			}
			//�����������RET
			else {
				store("$v1", s1, ""); //���Ĵ�����ֵ��ŵ��ڴ���
				getmidcode();
			}
		}
		//��ֵ���3: a [ x ] = b
		else if (s4 == "]")
		{
			//mips << "*******----��ֵ���3��a [ x ] = b" << endl;
			reg1 = load("to reg", s6, "", 0, "");	
			reg2 = load("to reg", s3, "", 0, "");
			reg3 = load("to reg", "-4", "", 0, "");
			mips << "mul " << reg3 << " " << reg3 << " " << reg2 << endl;
			regact("to free", reg2);
			store(reg1, s1, reg3);
			regact("to free", reg1);
			regact("to free", reg3);
			getmidcode();
		}
		//��ֵ���4: t = a [ x ]  //��һ������ֻ�������м���ʱ����@t
		else if (s4 == "[") 
		{
			//mips << "*******----��ֵ���4�� t = a [ x ]" << endl;
			reg1 = load("to reg", s5, "", 0, "");//x
			reg2 = load("to reg", "-4", "", 0, "");
			mips << "mul " << reg2 << " " << reg2 << " " << reg1 << endl;// x*-4
			regact("to free", reg1);
			reg3 = load("to reg", s3, "", 0, reg2);//a[x]
			regact("to free", reg2);
		
			//mips << "lw " << reg1 << " " << records[loc_fun][s3] - atoi(s5.c_str()) * 4 << "($fp)" << endl;

			mips << "subi $sp $sp 4" << endl;
			mips << "sw " << reg3 << " " << fp_offset << "($fp)" << endl;
			regact("to free", reg3);

			records[loc_fun][s1] = fp_offset;
			fp_offset -= 4;
			getmidcode();
		}
		//����1: a == b  
		else if (s2 == "==" || s2 == "!=" || s2 == "<" || s2 == "<=" || s2 == ">" || s2 == ">=")
		{
			//mips << "*******----����1�� a == b" << endl;
			reg1 = load("to reg", s1, "", 0, "");
			reg2 = load("to reg", s3, "", 0, "");
			reg3 = regact("get reg", "");
			mips << "sub " << reg3 << " " << reg1 << " " << reg2 << endl;
			regact("to use", reg3);
			string op = s2;
			getmidcode();
			//����ʱת��
			if (s1 == "BNZ") {
				if (op == "==") mips << "beq " << reg3 << " $zero " << s2 << endl;
				if (op == "!=") mips << "bne " << reg3 << " $zero " << s2 << endl;
				if (op == "<") mips << "bltz " << reg3 << " "<< s2 << endl;
				if (op == "<=") mips << "blez " << reg3 << " " << s2 << endl;
				if (op == ">") mips << "bgtz " << reg3 << " " << s2 << endl;
				if (op == ">=") mips << "bgez " << reg3 << " " << s2 << endl;
				//mips << "nop " << endl;
			}
			//������ʱת��
			else if (s1 == "BZ") {
				if (op == "==") mips << "bne " << reg3 << " $zero " << s2 << endl;
				if (op == "!=") mips << "beq " << reg3 << " $zero " << s2 << endl;
				if (op == "<") mips << "bgez " << reg3 << " " << s2 << endl;
				if (op == "<=") mips << "bgtz " << reg3 << " " << s2 << endl;
				if (op == ">") mips << "blez " << reg3 << " " << s2 << endl;
				if (op == ">=") mips << "bltz " << reg3 << " " << s2 << endl;
				//mips << "nop " << endl;
			}
			else;
			regact("to free", reg1);
			regact("to free", reg2);
			regact("to free", reg3);
			getmidcode();
		}
		//���
		else if (s2 == ":") 
		{
			mips << s1 << s2 << endl;
			getmidcode();
		}
		//��ת���
		else if (s1 == "GOTO") {
			mips << "j " << s2 << endl;
			//mips << "nop " << endl;
			getmidcode();
		}
		//������
		else if (s1 == "printf") {

			if (findtab2(s2) == 6 || findtab2(s2) == 8 || findtab2(s2) == 4) {
				reg1 = load("to reg", s2, "", 0, "");
				mips << "move $a0 " << reg1 << endl;
				regact("to free", reg1);
				mips << "li $v0 1" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}
			else if (findtab2(s2) == 7 || findtab2(s2) == 9 || findtab2(s2) == 5) {
				reg1 = load("to reg", s2, "", 0, "");
				mips << "move $a0 " << reg1 << endl;
				regact("to free", reg1);
				mips << "li $v0 11" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}
			
			else if (records[loc_fun].count(s2) == 1) {
				reg1 = load("to reg", s2, "", 0, "");
				mips << "move $a0 " << reg1 << endl;
				regact("to free", reg1);
				mips << "li $v0 1" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}
			
			else if (s2[0] == '\'') { //����ַ�
				int tempchar = s2[1];
				mips << "li $a0 " << tempchar << endl;
				mips << "li $v0 11" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}

			else if (s2[0] == '-' || (s2[0]>='0'&&s2[0]<='9')) { //�������
				int tempchar = s2[1];
				mips << "li $a0 " << atoi(s2.c_str()) << endl;
				mips << "li $v0 1" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}

			else {
				
				char s[200];
				memset(s, 0, sizeof(s));
				int i = 0, j = 0;
				int flag = 0;
				do {
					if (codeline[i] == '"' && flag == 1) {
						break;
					}
					if (codeline[i] == '"' && flag == 0) {
						flag = 1;
						i++;
					}
					if (flag == 1) s[j++] = codeline[i];
					i++;

				} while (true);
				string s2 = s;
				cout << s2 << ">>>>>>>" << ss[s2] << endl;
				mips << "la $a0 "<< ss[s2] << endl;
				mips << "li $v0 4" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}
		}
		//�������
		else if (s1 == "scanf") {
			if (findtab2(s2) == 6 || findtab2(s2) == 8) mips << "li $v0 5" << endl;
			else if (findtab2(s2) == 7 || findtab2(s2) == 9) mips << "li $v0 12" << endl;
			else {
				cout << "from line 627" << endl;
				system("pause");
				exit(0);
			}

			if (records[loc_fun].count(s2) == 1) {
				mips << "syscall" << endl;
				mips << "sw $v0 " << records[loc_fun][s2] << "($fp)" << endl;
			}
			else {
				mips << "syscall" << endl;
				vector<string>::iterator it = find(glob.begin(), glob.end(), s2);
				if (it == glob.end()) {
					cout << "û�ҵ� �� " << s2 << endl;
					system("pause");
					exit(0);
				}
				string loc = *it;
				reg2 = regact("get reg", "");
				mips << "la " << reg2 << " " << loc << endl;
				regact("to use", reg2);
				mips << "sw $v0 (" << reg2 << ")" << endl;
				regact("to free", reg2);
			}
			getmidcode();
		}
		//�������
		else if (s1 == "ret")
		{
			//mips << "*******----�������" << endl;
			if (s2 != "") {
				load("to reg", s2, "$v1", 1, "");
			}
			reg1 = regact("get reg", "");
			mips << "move " << reg1 << " $ra" << endl;     //��ǰ�ķ��ص�ַת���� $t0
			regact("to use", reg1);
			mips << "lw $ra -4($fp)" << endl;   //�������������һ�����¼�� ra ת���� $ra
			mips << "move $sp $fp" << endl;     //���¹���sp ����ǰ��� $fp ����һ�����¼�� $fp
			mips << "lw $fp ($fp)" << endl;   //���¹���fp
			mips << "addi $sp $sp 4" << endl;  //����spָ��
			mips << "jr "<< reg1 << endl;        //�ؼҿ�	
			//mips << "nop "<< endl;
			regact("to free", reg1);
			getmidcode();
			//�����һ�������Ǻ���������˵����������
			if ((s3 == "()") && (s1 == "int" || s1 == "char" || s1 == "void")) {
				records.erase(loc_fun);
			}
		}
		//��������
		else if (s1 == "call" || s1 == "push")
		{
			//mips << "*******----��������" << endl;
			int para_num2 = 0;
			while (s1 == "push") {
				if (para_num2 <= 3) load("to reg", s2, "$a" + to_string(para_num2), 1, "");
				else {
					load("to reg", s2, "$t" + to_string(13 - para_num2), 1, "");
					regact("to use", "$t" + to_string(13 - para_num2));
				}
				para_num2++;
				getmidcode();
			}
			if (s1 != "call") {
				cout << "from 690" << endl;
				system("pause");
				exit(0);
			}
			mips << "subi $sp $sp 4" << endl;
			mips << "sw $fp ($sp)" << endl;      //����ɻ��fp
			mips << "move $fp $sp" << endl;      //�»��fp
			mips << "subi $sp $sp 4" << endl;   
			mips << "sw $ra ($sp)" << endl;      //�»�ķ��ص�ַ
			mips << "jal " << s2 << endl;
			//mips << "nop" << endl;
			getmidcode();
		}
		else {
			mips << "????????-----" << s1 << s2 << s3 << s4 << s5 << endl;
			getmidcode();
		}
	}
}

int main() {
	
	main2();
	
	ready("midcode.txt", "mipscode.txt");
	data();
	text();

	system("pause");
	midcodefile.close();
	mips.close();
	cout << "**********�ֿ�*****************" << endl;
	main3();
	system("pause");
	return 0;
}
