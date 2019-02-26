#include<fstream>
#include<iostream>
#include<string>
#include<stack>
#include<deque>
#include<map>
using namespace std;


struct par {
	int type = -1;  // 1��void���� 2��int/char���� 4��const������4int 5char�� 6��var������6int 7char�� 8������(8int 9char) 10��para����
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
};
struct glo {
	int type = -1;  // 1��void���� 2��int���� 3��char���� 4��const���� 6��var���� 8������ 10��para���� 12���ַ���
	int value = -1;
	int addr = -1;
	int size = -1;  // ��������������   ���飺Ԫ�ظ���  ������-1
	map<string, par>  partab; //�ֲ�������
};
map <string, glo> tab; //ȫ�ֱ�����
map <string, par> partab; //�ֱ�����

struct code {
	string op = "";
	string arg1 = "";
	string arg2 = "";
	string result = "";
};

deque<string> arg_stack[10];
stack<string> op_stack[10];
stack<string> re_stack[10];
ofstream midcode; //����ļ�
int var_num = 0;
int label_num = 0;

string new_var(int i) {
	string var_name;
	if (i == 1) var_name = "@t" + to_string(var_num++);
	else var_name = "@t" + to_string(var_num - 1);
	return var_name;
}

string new_label(int i) {
	string label_name;
	if (i == 1) label_name	= "label_" + to_string(label_num++);
	else label_name = "label_" + to_string(label_num - 1);
	return label_name;
}

//���
int findtab(string token0) {

	map <string, par>::iterator it1;
	it1 = partab.find(token0);
	if (it1 != partab.end()) {
		if (partab[token0].type == 4 || partab[token0].type == 5)return 1;//����
		if (partab[token0].type == 6 || partab[token0].type == 7)return 2;//����
		if (partab[token0].type == 2 || partab[token0].type == 3)return 3;//int/char����
		if (partab[token0].type == 1)return 5;//void
		if (partab[token0].type == 8 || partab[token0].type == 9)return 100 + partab[token0].size;//����
	}
	else {
		map <string, glo>::iterator it2;
		it2 = tab.find(token0);
		if (it2 != tab.end()) {
			if (tab[token0].type == 4 || tab[token0].type == 5)return 1;//����
			if (tab[token0].type == 6 || tab[token0].type == 7)return 2;//����
			if (tab[token0].type == 2 || tab[token0].type == 3)return 3;//int/char����
			if (tab[token0].type == 1)return 5;//void
			if (tab[token0].type == 8 || tab[token0].type == 9)return 100 + tab[token0].size;//����
		}
		else {
			cout << "�Ҳ������� �� " << token0;
			return 6;
		}
	}
}
int findtab3(string token0) {

	map <string, par>::iterator it1;
	it1 = partab.find(token0);
	if (it1 != partab.end()) {
		return partab[token0].type;
	}
	else {
		map <string, glo>::iterator it2;
		it2 = tab.find(token0);
		if (it2 != tab.end()) {
			return tab[token0].type;
		}
		else {
			cout << "�Ҳ������� �� " << token0;
			return -1;
		}
	}
}

string temp_func_name;
int findtab4(string token0) {

	map <string, glo>::iterator it1;
	if (temp_func_name == "") {
		it1 = tab.find(token0);
		if (it1 != tab.end()) {
			return 0;
		}
		else return 1;
	}
	else {
		map <string, par>::iterator it2;
		it2 = partab.find(token0);
		if (it2 != partab.end()) {
			return 0;
		}
		else {
			return 1;
		}
	}
}

glo temp_glo;
par temp_par;
string temp_name;

int ad = 0;
//������
void structglotab(int t, int v, int a, int s) {
	temp_glo.type = t;
	temp_glo.value = v;
	temp_glo.addr = a;
	temp_glo.size = s;
	temp_glo.partab = partab;
	if (t == 1 || t == 2 || t == 3) {
		tab[temp_func_name] = temp_glo;
		cout << temp_func_name << "  ���1" << endl;
	}
	else {
		tab[temp_name] = temp_glo;
		cout << temp_name << "  ���2" << endl;
	}
	if (t == 4) ad = ad + 4 * s;
}
void structpartab(int t, int v, int a, int s) {
	temp_par.type = t;
	temp_par.value = v;
	temp_par.addr = a;
	temp_par.size = s;
	partab[temp_name] = temp_par;
	cout << temp_name << "  ���3" << endl;
	if (t == 4) ad = ad + 4 * s;
}



void midecodeinit() {
	midcode.open("midcode.txt");
	if (!midcode.is_open()) {
		cout << "�޷��м���봴������ļ���" << endl;
		exit(0);
	}
}

void ccode(string op, string arg1, string arg2, string result) {
	//����
	if		(op == "const") midcode << op << " " << arg1 << " " << result << " = " << arg2 << endl;
	else if (op == "var")	midcode << arg1 << " " << result << endl;
	else if (op == "var[]")	midcode << "array" << " " << arg1 << " " << result << " " << arg2 << endl;
	else if (op == "func")	midcode << arg1 << " " << result << " ()" << endl;
	else if (op == "para")	midcode << op << " " << arg1 << " " << result << endl;
	else if (op == "rtn")	midcode << "ret" << " " << result << endl;
	else if (op == "cmp")	midcode << arg1 << " " << result << " " << arg2 << endl;
	else if (op == "=")		midcode << result << " = " << arg1 << endl;
	else if (op == "+")		midcode << result << " = " << arg1 << " + " << arg2 << endl;
	else if (op == "-")		midcode << result << " = " << arg1 << " - " << arg2 << endl;
	else if (op == "*")		midcode << result << " = " << arg1 << " * " << arg2 << endl;
	else if (op == "/")		midcode << result << " = " << arg1 << " / " << arg2 << endl;
	else if (op == "=[]")	midcode << result << " = " << arg1 << " [ " << arg2 << " ] " << endl;
	else if (op == "[]=")	midcode << result << " [ " << arg2 << " ] " << " = " << arg1  << endl;
	else if (op == "BZ")	midcode << op << " " << result << endl;//
	else if (op == "BNZ")	midcode << op << " " << result << endl;//
	else if (op == "GOTO")	midcode << op << " " << result << endl;//
	else if (op == "set")	midcode << result << " :" << endl;
	else if (op == "push")	midcode << op << " " << result << endl;
	else if (op == "call")	midcode << op << " " << arg1 << endl;//
	else if (op == "scanf")	midcode << op << " " << arg1 << endl;
	else if (op == "printf") {
		if (arg2 == "i") midcode << op << " " << arg1 << endl;
		else if (arg2 == "c") midcode << op << " '" << (char)atoi(arg1.c_str()) << "'" << endl;
		else midcode << op << " \"" << arg1 << "\"" << endl;
	}
	else midcode << op << ", " << arg1 << ", " << arg2 << ", " << result << endl;
}

void RPN_struct(int act, string op, string arg, int k) {
	// ������ջ
	if (act == 0) {
		arg_stack[k].push_back(arg);
		cout << arg << " �������ջ" << endl;
	}
	// �����ջ     ( [ ) ] + - * / []=
	if (act == 1) {
		if (op_stack[k].empty() || op == "(" || op == "[") { // ���ջΪ��  ����  opΪ������
			op_stack[k].push(op);
			cout << op << "	�������ջ" << endl;
			return;
		}
		else if (op == ")" || op == "]") { // �����������
			string o;
			if (op == ")") o = "(";
			if (op == "]") o = "[";
			while (op_stack[k].top() != o ) { //���û�ж���������
				string opt = op_stack[k].top();
				op_stack[k].pop();
				cout << opt << " ��ջ" << endl;
				
				arg_stack[k].push_back(opt);
				cout << opt << "	�������ջ" << endl;			
				if (op_stack[k].empty()) {
					cout << "#ȱ��������" << endl;
					break;
				}
			}
			op_stack[k].pop(); //�����ų�ջ
			
		
		}
		else if (op == "+" || op == "-") { // ���Ϊ�Ӽ���
			while (!op_stack[k].empty() && op_stack[k].top() != "(" && op_stack[k].top() != "[") {
				string opt = op_stack[k].top();
				op_stack[k].pop();
				cout << opt << " ��ջ" << endl;

				arg_stack[k].push_back(opt);
				cout << opt << "	�������ջ" << endl;
			}
			op_stack[k].push(op);
			cout << op << "	�������ջ" << endl;
			return;
		}
		else if (op == "*" || op == "/") { // ���Ϊ�˳���
			while (op_stack[k].top() == "=[]" || op_stack[k].top() == "*" || op_stack[k].top() == "/") {
				string opt = op_stack[k].top();
				op_stack[k].pop();
				cout << opt << " ��ջ" << endl;

				arg_stack[k].push_back(opt);
				cout << opt << "	�������ջ" << endl;
				if (op_stack[k].empty())break;
			}
			op_stack[k].push(op);
			cout << op << "	�������ջ" << endl;
			return;
		}
		else if (op == "=[]") { // ���Ϊ����
			while (op_stack[k].top() == "=[]") {
				string opt = op_stack[k].top();
				op_stack[k].pop();
				cout << opt << " ��ջ" << endl;

				arg_stack[k].push_back(opt);
				cout << opt << "	�������ջ" << endl;
				if (op_stack[k].empty())break;
			}
			
			op_stack[k].push(op);
			cout << op << "	�������ջ" << endl;
			return;
		}
		else cout << "�Ƿ������" << endl;
	}
}

string RPN_reck(int k) {
    int res = 0;
	while (!op_stack[k].empty()) {
		string opt = op_stack[k].top();
		op_stack[k].pop();
		cout << opt << " ��ջ" << endl;

		arg_stack[k].push_back(opt);
		cout << opt << "	�������ջ" << endl;
	}

	deque<string>::iterator pos;
	for (pos = arg_stack[k].begin(); pos != arg_stack[k].end(); pos++)
		cout << *pos << endl;


	string arg1, arg2, op;
	while (!arg_stack[k].empty()) {
		if (arg_stack[k].front() == "=[]") { //�ж��Ƿ�Խ��
			op = arg_stack[k].front(); arg_stack[k].pop_front();
			arg2 = re_stack[k].top(); re_stack[k].pop();
			arg1 = re_stack[k].top(); re_stack[k].pop();
			//cout << arg2 << "%%%%%%%%%%%%%%%" << arg1 << "%%%%%%%%%%%%%%%%%%%%%"  << endl;
			int find = findtab(arg1);
			if (find < 100) res = 1;
			//cout <<find<<"%%%%%%%%%%%%%%%"<< atoi(arg2.c_str()) << "%%%%%%%%%%%%%%%%%%%%%"<< (find - 100) << endl;
			if (find > 100 && atoi(arg2.c_str())>= (find-100)) res = 2;
			ccode(op, arg1, arg2, new_var(1));
			temp_name = new_var(0);
			if (findtab3(arg1) == 9) structpartab(7, 0, ad, 1);
			else structpartab(6, 0, ad, 1);
			re_stack[k].push(new_var(0));
		}
		else if (arg_stack[k].front() == "/") { //�ж��Ƿ�Խ��
			op = arg_stack[k].front(); arg_stack[k].pop_front();
			arg2 = re_stack[k].top(); re_stack[k].pop();
			arg1 = re_stack[k].top(); re_stack[k].pop();
			if (atoi(arg2.c_str()) == 0 && arg2[0]<='9' && arg2[0]>='0') res = 3;
			ccode(op, arg1, arg2, new_var(1));
			temp_name = new_var(0);
			structpartab(6, 0, ad, 1);
			re_stack[k].push(new_var(0));
		}
		else if (arg_stack[k].front() == "+" || arg_stack[k].front() == "-" || arg_stack[k].front() == "*" ) {
			op = arg_stack[k].front(); arg_stack[k].pop_front();
			arg2 = re_stack[k].top(); re_stack[k].pop();
			arg1 = re_stack[k].top(); re_stack[k].pop();
			ccode(op, arg1, arg2, new_var(1));
			temp_name = new_var(0);
			structpartab(6, 0, ad, 1);
			re_stack[k].push(new_var(0));
		}
		else {
			if (!arg_stack[k].empty()) {
				re_stack[k].push(arg_stack[k].front());
				arg_stack[k].pop_front();
			}
		}
	}
	if (re_stack[k].empty()) {
		cout << "# ���ʽ����ջ����" << endl;
		system("pause");
		exit(0);
	}
	arg1 = re_stack[k].top();
	re_stack[k].pop();
	if (res == 0) return arg1;
	else if (res == 3) return "div0";
	else return "arrayerror";
	
}

void mid_end() {
	midcode.close();
}



