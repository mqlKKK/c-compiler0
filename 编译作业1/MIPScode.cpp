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

ifstream midcodefile; //midcode文件
ofstream mips; //输出文件
string codeline, s1, s2, s3, s4, s5, s6, s7, temp;//记录正在读的行和行的每一部分
stringstream codeline2;
map <string, map <string, int>> records; // 栈
map <string, int> regs; //寄存器
vector <string> glob; //全局变量
queue <string> parms; //参数栈
string loc_fun;//当前所在函数
int fp_offset;//相对fp的偏移量（用于记录参数和局部变量）

//初始化
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
		cout << "找不到中间代码文件！" << endl;
		exit(0);
	}
	mips.open(ssss);
	if (!mips.is_open()) {
		cout << "无法创建输出文件！" << endl;
		exit(0);
	}
}

//按行读中间代码文件
void getmidcode() {
	getline(midcodefile,codeline);
	codeline2.clear();
	codeline2.str(codeline);
	s1 = ""; s2 = ""; s3 = ""; s4 = ""; s5 = ""; s6 = ""; s7 = "";
	codeline2 >> s1 >> s2 >> s3 >> s4 >> s5 >> s6 >> s7;
}

//输入变量，返回变量类型
int findtab2(string id) {	
	map <string, par>::iterator it1;
	it1 = tab[loc_fun].partab.find(id); //首先寻找局部变量
	if (it1 != tab[loc_fun].partab.end()) {  //如果找到了，返回变量类型
		return tab[loc_fun].partab[id].type;
	}
	else {
		map <string, glo>::iterator it2; //如果局部变量中没有找到，在全局变量中找
		it2 = tab.find(id);
		if (it2 != tab.end()) {
			return tab[id].type;
		}
		else { //如果还是没找到，返回0 （可能是字符串）
			cout << "找不到符号 ： " << id << endl;
			return 0;
		}
	}
}

//寄存器管理
//输入： 操作指令i   寄存器reg    
//输出：可以返回一个空闲的寄存器
string regact(string i, string reg) {
	//占用寄存器
	if (i == "to use") {
		regs[reg] = 1;
	}
	//释放寄存器
	if (i == "to free") {
		regs[reg] = 0;
	}
	//查询寄存器
	if (i == "to check") {
		if (regs[reg] == 1) return "no";
		else return "yes";
	}
	//返回空闲的临时寄存器
	if (i == "get reg") {
		int n = 0;
		while (regs["$t" + to_string(n)] != 0) n++;
		regact("to use", "$t" + to_string(n));
		return "$t" + to_string(n);
	}
	return "";
}

//从内存中读取数据放到寄存器
//输入：操作指令i   标识符s   指定寄存器reg   是否指定寄存器op
//输出：返回存放数据的寄存器
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
		//是数字
		if (s[0] <= '9' && s[0] >= '0' || s[0] == '-') {
			reg1 = regact("get reg", "");
			mips << "li " << reg1 << " " << s << endl;
			return reg1;
		}
		//是局部变量
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
		//是全局变量
		else {
			vector<string>::iterator it = find(glob.begin(), glob.end(), s);
			if (it == glob.end()) {
				cout << "在全局中没找到变量 : " << s << endl;
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
		//是数字
		if (s[0] <= '9' && s[0] >= '0' || s[0] == '-') {
			mips << "li " << reg << " " << s << endl;
			return "";
		}
		//是局部变量
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
		//是全局变量
		else {
			vector<string>::iterator it = find(glob.begin(), glob.end(), s);
			if (it == glob.end()) {
				cout << "在全局中没找到变量 : " << s << endl;
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

//从寄存器中读取数据放到内存
//输入：存放数据的寄存器reg  存放数据的变量id
void store(string reg, string id, string off) {
	string regt, regt2;
	//如果是局部变量
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
		//如果不是局部变量也不是全局变量，那么应该是中间代码产生的中间变量@t，存放到栈顶，并加入该函数的活动记录
		if (it == glob.end()) {
			mips << "subi $sp $sp 4" << endl;
			mips << "sw " << reg << " " << fp_offset << "($fp)" << endl;
			records[loc_fun][id] = fp_offset;
			fp_offset -= 4;
			return;
		}
		//如果是全局变量
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

//生成 .data 段  （返回一行 codeline）
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

//生成 .text 段
void text() {
	int n = 0;
	string reg1, reg2, reg3;
	mips << ".text" << endl;
	mips << "subi $sp $sp 4" << endl;
	mips << "sw $fp ($sp)" << endl;      //保存旧活动的fp
	mips << "move $fp $sp" << endl;      //新活动的fp----------------------
	mips << "subi $sp $sp 4" << endl;
	mips << "sw $ra ($sp)" << endl;      //新活动的返回地址
	mips << "j main" << endl;
	while (codeline != "") {
		//函数定义
		if ((s3 == "()") && (s1 == "int" || s1 == "char" || s1 == "void"))
		{
			//mips << "*******----函数定义" << endl;
			loc_fun = s2;
			map <string, int> record; 
			records[loc_fun] = record; //一个函数的活动记录
			mips << s2 << ":";
			fp_offset = -8; // 0(fp) 存放上一个活动的fp     -4(fp) 存放返回地址
			getmidcode();
		}
		//函数参数
		else if (s1 == "para")
		{
			//mips << "*******----函数参数" << endl;
			int para_num = 0;
			while (s1 == "para") {
				mips << "subi $sp $sp 4" << endl;
				//参数小于4个，使用 $a 寄存器
				if (para_num <= 3) {
					mips << "sw $a" << para_num << " " << fp_offset << "($fp)" << endl;
					records[loc_fun][s3] = fp_offset;
				}
				//参数大于4个，多余的使用 $t 寄存器
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
		//函数内变量常量定义
		else if (s1 == "const" || s1 == "int" || s1 == "char" || s1 == "array")
		{		
			//mips << "*******----函数内变量常量定义" << endl;
			while (s1 == "const" || s1 == "int" || s1 == "char" || s1 == "array") {	
				//如果是常量
				if (s1 == "const") {
					mips << "subi $sp $sp 4" << endl;
					reg1 = regact("get reg", ""); //得到一个空闲寄存器
					if (s2 == "int") mips << "li " << reg1 << " " << s5 << endl; //把常量给刚刚返回的寄存器  
					else mips << "li " << reg1 << " " << (int)s5[0] << endl; //如果是字符先转换类型
					regact("to use", reg1); 
					mips << "sw "<<reg1<<" " << fp_offset << "($fp)" << endl; //将寄存器的值保存到栈顶
					regact("to free", reg1); //释放寄存器
					records[loc_fun][s3] = fp_offset; //将常量加入到该函数的活动记录
					fp_offset -= 4;
				}
				//如果是变量
				else if (s1 == "int" || s1 == "char") {
					mips << "subi $sp $sp 4" << endl;
					records[loc_fun][s2] = fp_offset;//加入到该函数的活动记录
					fp_offset -= 4;
				}
				//如果是数组
				else {
					int arraysize = atoi(s4.c_str()) * 4;
					records[loc_fun][s3] = fp_offset; //将数组开始的位置加入到该函数的活动记录
					mips << "subi $sp $sp "<< arraysize << endl; //为数组在栈顶开辟空间
					for (int i = 0; i < atoi(s4.c_str()); i++) { 
						fp_offset -= 4;
					}
				}
				getmidcode();
			}
		}
		//赋值语句1: a = x + y
		else if (s2 == "=" && (s4 == "+" || s4 == "-" || s4 == "*" || s4 == "/"))
		{
			//mips << "*******----赋值语句1：a = x + y" << endl;
			reg1 = load("to reg",s3, "", 0, "");//将操作数加载到寄存器
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
			
			store(reg3, s1, ""); //将寄存器中的结果放到内存中

			regact("to free", reg1);
			regact("to free", reg2);
			regact("to free", reg3);
			getmidcode();
		}
		//赋值语句2: a = b
		else if (s2 == "=" && s4 == "")
		{
			//mips << "*******----赋值语句2：a = b" << endl;
			if (s3 != "RET") {
				reg1 = load("to reg", s3, "", 0, ""); //将操作数加载到寄存器中
				store(reg1, s1, ""); //将寄存器的值存放到内存中
				regact("to free", reg1);
				getmidcode();
			}
			//如果操作数是RET
			else {
				store("$v1", s1, ""); //将寄存器的值存放到内存中
				getmidcode();
			}
		}
		//赋值语句3: a [ x ] = b
		else if (s4 == "]")
		{
			//mips << "*******----赋值语句3：a [ x ] = b" << endl;
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
		//赋值语句4: t = a [ x ]  //第一个参数只可能是中间临时变量@t
		else if (s4 == "[") 
		{
			//mips << "*******----赋值语句4： t = a [ x ]" << endl;
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
		//条件1: a == b  
		else if (s2 == "==" || s2 == "!=" || s2 == "<" || s2 == "<=" || s2 == ">" || s2 == ">=")
		{
			//mips << "*******----条件1： a == b" << endl;
			reg1 = load("to reg", s1, "", 0, "");
			reg2 = load("to reg", s3, "", 0, "");
			reg3 = regact("get reg", "");
			mips << "sub " << reg3 << " " << reg1 << " " << reg2 << endl;
			regact("to use", reg3);
			string op = s2;
			getmidcode();
			//满足时转移
			if (s1 == "BNZ") {
				if (op == "==") mips << "beq " << reg3 << " $zero " << s2 << endl;
				if (op == "!=") mips << "bne " << reg3 << " $zero " << s2 << endl;
				if (op == "<") mips << "bltz " << reg3 << " "<< s2 << endl;
				if (op == "<=") mips << "blez " << reg3 << " " << s2 << endl;
				if (op == ">") mips << "bgtz " << reg3 << " " << s2 << endl;
				if (op == ">=") mips << "bgez " << reg3 << " " << s2 << endl;
				//mips << "nop " << endl;
			}
			//不满足时转移
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
		//标号
		else if (s2 == ":") 
		{
			mips << s1 << s2 << endl;
			getmidcode();
		}
		//跳转语句
		else if (s1 == "GOTO") {
			mips << "j " << s2 << endl;
			//mips << "nop " << endl;
			getmidcode();
		}
		//输出语句
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
			
			else if (s2[0] == '\'') { //输出字符
				int tempchar = s2[1];
				mips << "li $a0 " << tempchar << endl;
				mips << "li $v0 11" << endl;
				mips << "syscall" << endl;
				getmidcode();
			}

			else if (s2[0] == '-' || (s2[0]>='0'&&s2[0]<='9')) { //输出数字
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
		//输入语句
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
					cout << "没找到 ： " << s2 << endl;
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
		//返回语句
		else if (s1 == "ret")
		{
			//mips << "*******----返回语句" << endl;
			if (s2 != "") {
				load("to reg", s2, "$v1", 1, "");
			}
			reg1 = regact("get reg", "");
			mips << "move " << reg1 << " $ra" << endl;     //当前的返回地址转交给 $t0
			regact("to use", reg1);
			mips << "lw $ra -4($fp)" << endl;   //参数区保存的上一个活动记录的 ra 转交给 $ra
			mips << "move $sp $fp" << endl;     //重新管理sp （当前活动的 $fp 是上一个活动记录的 $fp
			mips << "lw $fp ($fp)" << endl;   //重新管理fp
			mips << "addi $sp $sp 4" << endl;  //不让sp指空
			mips << "jr "<< reg1 << endl;        //回家咯	
			//mips << "nop "<< endl;
			regact("to free", reg1);
			getmidcode();
			//如果下一条代码是函数声明，说明函数结束
			if ((s3 == "()") && (s1 == "int" || s1 == "char" || s1 == "void")) {
				records.erase(loc_fun);
			}
		}
		//函数调用
		else if (s1 == "call" || s1 == "push")
		{
			//mips << "*******----函数调用" << endl;
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
			mips << "sw $fp ($sp)" << endl;      //保存旧活动的fp
			mips << "move $fp $sp" << endl;      //新活动的fp
			mips << "subi $sp $sp 4" << endl;   
			mips << "sw $ra ($sp)" << endl;      //新活动的返回地址
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
	cout << "**********分块*****************" << endl;
	main3();
	system("pause");
	return 0;
}
