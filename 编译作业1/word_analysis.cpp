#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
using namespace std;

#define CONSTSY 1
#define INTSY 2
#define CHARSY 3
#define VOIDSY 4
#define MAINSY 5
#define IFSY 6
#define ELSESY 7
#define WHILESY 8
#define FORSY 9
#define SCANFSY 10
#define PRINTFSY 11
#define RETURNSY 12

#define PLUSSY 13
#define MINUSSY 14
#define TIMESSY 15 
#define DIVSY 16 
#define LESSY 17
#define LEEY 18
#define GRTSY 19
#define GRESY 20
#define NEQSY 21
#define EQLSY 22
#define SGQSY 23
#define QUOSY 24
#define LPASY 25
#define RPASY 26
#define LBRASY 27
#define RBRASY 28
#define LCURSY 29
#define RCURSY 30
#define COMMASY 31
#define SEMISY 32
#define ASSIGNSY 33
#define IDENSY 34
#define UNSIGNSY 35
#define SIGNSY 36
#define STRINGSY 37
#define CHAR2SY 38
#define ERROR2 39

string key[50]; //�����ֱ�
char line[1024]; //�л�����
int lc; //��ָ��
int ll; //�г���
int lines; //�б��
char token[200]; //sym������
int tc; //sym������ָ��
int symbol; //��ǰ�������ַ���������
char ch; //��ǰ�������ַ�
int num; //��ǰ����������
int resultn; //��ǰ���ĵ�����
ifstream infile; //Դ�����ļ�
ofstream outfile; //�ʷ���������ļ�

char filename[20];

//����ʷ��������������̨���ļ���
void result() {
	
	cout << resultn++ << "	" << key[symbol] << "		" << token << endl;
	outfile << resultn - 1 << "	" << key[symbol] << "		" << token << endl;
	
	if (resultn > 9999999) {
		cout << "����ʧ�ܣ�" << endl;
		exit(0);
	}
}
//��ʼ�������ֺ������ַ�,�����ļ�
void init() {
	if (true) {
		key[1] = "CONSTSY";
		key[2] = "INTSY";
		key[3] = "CHARSY";
		key[4] = "VOIDSY";
		key[5] = "MAINSY";
		key[6] = "IFSY";
		key[7] = "ELSESY";
		key[8] = "WHILESY";
		key[9] = "FORSY";
		key[10] = "SCANFSY";
		key[11] = "PRINTFSY";
		key[12] = "RETURNSY";

		key[13] = "PLUSSY";
		key[14] = "MINUSSY";
		key[15] = "TIMESSY";
		key[16] = "DIVSY";
		key[17] = "LESSY";
		key[18] = "LEEY";
		key[19] = "GRTSY";
		key[20] = "GRESY";
		key[21] = "NEQSY";
		key[22] = "EQLSY";
		key[23] = "SGQSY";
		key[24] = "QUOSY";
		key[25] = "LPASY";
		key[26] = "RPASY";
		key[27] = "LBRASY";
		key[28] = "RBRASY";
		key[29] = "LCURSY";
		key[30] = "RCURSY";
		key[31] = "COMMASY";
		key[32] = "SEMISY";
		key[33] = "ASSIGNSY";
		key[34] = "IDENSY";
		key[35] = "UNSIGNSY";
		key[36] = "SIGNSY";
		key[37] = "STRINGSY";
		key[38] = "CHAR2SY";
		key[39] = "ERROR2";
	}
	cout << "init finish" << endl;

	resultn = 1;
	cout << "������Դ�����ļ�����" << endl;
	cin.getline(filename, 90);
	infile.open(filename);
	if (!infile.is_open()) {
		cout << "�Ҳ��������ļ���" << endl;
		exit(0);
	}
	outfile.open("result.txt");
	if (!outfile.is_open()) {
		cout << "�޷���������ļ���" << endl;
		exit(0);
	}

}
//�õ�һ���ַ�
streampos pos;
void setmark(int i) {
	if (i == 1) {
		pos = infile.tellg();
	}
	if (i == 2) {
		infile.seekg(pos);
	}
}
char getch(int i) {
	if (lc == ll) {
		lines++;
		if (infile.getline(line, 512)) {
			ll = strlen(line);
			lc = 0;
			return('\n');
		}
		else
			return EOF;
	}
	else lc++;
	//cout << "����  " << line[lc - 1] << endl;
	return line[lc - 1];
}
//�ж��Ƿ��Ѿ������ļ�
bool isEND() {
	if (ch == EOF) {
		cout << "END OF READING" << endl;
		infile.close();
		outfile.close();
		getchar();
		exit(0);
	}
	return true;
}
//����һ������
void next(int i) {
	memset(token, 0, sizeof(token));
	tc = 0;
	ch = getch(i);
}
//�ж�token�Ƿ��ǹؼ���
int isKEY() {
	if (strcmp(token, "const") == 0) return 1;
	if (strcmp(token, "int") == 0) return 2;
	if (strcmp(token, "char") == 0) return 3;
	if (strcmp(token, "void") == 0) return 4;
	if (strcmp(token, "main") == 0) return 5;
	if (strcmp(token, "if") == 0) return 6;
	if (strcmp(token, "else") == 0) return 7;
	if (strcmp(token, "while") == 0) return 8;
	if (strcmp(token, "for") == 0) return 9;
	if (strcmp(token, "scanf") == 0) return 10;
	if (strcmp(token, "printf") == 0) return 11;
	if (strcmp(token, "return") == 0) return 12;
	return 34;
}
//��ӡ����
void printerror(int op) {
	if (op == 1) cout << "#Line  " << lines << " �� ���ڲ��Ϸ����ַ� [!]" << endl;
	if (op == 2) cout << "#Line  " << lines << " �� ȱ���ҵ����� " << endl;
	if (op == 3) cout << "#Line  " << lines << " �� �ַ����д��ڲ��Ϸ��ַ� '" << ch << "'" << endl;
	if (op == 4) cout << "#Line  " << lines << " �� �ַ���ȱ��[\"] " << endl;

}
//�ʷ�����
void getsym(int i) {
	next(i);
	//���Ե��ʼ�Ŀո��Ʊ�������з�
	while (ch == ' ' || ch == '	' || ch == '\n') {
		ch = getch(i);
		continue;
	}
	if (isEND())
	{
		//-A-������,��ʶ��
		if (isalpha(ch) || ch == '_') {
			do {
				token[tc++] = tolower(ch);
				ch = getch(i);
			} while (isalnum(ch) || ch == '_'); 
			//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
			if(ch >= 32 && ch <= 126)lc--;
			symbol = isKEY();
			result();
			//next(i);
			//continue;
		}
		//-A-�޷�������
		else if (isdigit(ch)) {
			num = 0;
			do {
				num = num * 10 + (ch - '0');
				token[tc++] = ch;
				ch = getch(i);
			} while (isdigit(ch));
			//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
			if (ch >= 32 && ch <= 126)lc--;
			symbol = 35;
			result();
			//next(i);
			//continue;
		}
		//-A-˫�ַ��ֽ�� < > ! =
		else if (ch == '<') {
			token[tc++] = ch;
			ch = getch(i);
			if (ch == '=') {
				token[tc++] = ch;
				symbol = 18;
			}
			else {
				//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
				if (ch >= 32 && ch <= 126)lc--;
				symbol = 17;	
			}
			result();
			//next(i);
			//continue;
		}
		else if (ch == '>') {
			token[tc++] = ch;
			ch = getch(i);
			if (ch == '=') {
				token[tc++] = ch;
				symbol = 20;
			}
			else {
				//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
				if (ch >= 32 && ch <= 126)lc--;
				symbol = 19;
			}
			result();
			//next(i);
			//continue;
		}
		else if (ch == '!') {
			token[tc++] = ch;
			ch = getch(i);
			if (ch == '=') {
				token[tc++] = ch;
				symbol = 21;
				result();
			}
			else {
				//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
				if (ch >= 32 && ch <= 126)lc--;
				symbol = 21;
				//result(2);
				printerror(1);
			}
			//next(i);
			//continue;
		}
		else if (ch == '=') {
			token[tc++] = ch;
			ch = getch(i);
			if (ch == '=') {
				token[tc++] = ch;
				symbol = 22;
			}
			else {
				//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
				if (ch >= 32 && ch <= 126)lc--;
				symbol = 33;
			}
			result();
			//next(i);
			//continue;
		}
		//-A-�ַ�
		else if (ch == '\'') {
			ch = getch(i);
			if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || isalnum(ch) || ch == '_')
				token[tc++] = ch;
			else {
				//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
				if (ch >= 32 && ch <= 126) lc--;
				printerror(2);
			}
			ch = getch(i);
			if (ch != '\'') {
				printerror(2);
				//���ch�ǻ��з�������Ҫ������ָ�루���˻����lc =  -1��
				if (ch >= 32 && ch <= 126) lc--;
			}
			symbol = 38; //�ݴ�����һ���ַ������������ַ���������ַ��������ķ���ȡ
			result();
			//next(i);
			//continue;
		}
		//-A-�ַ���
		else if (ch == '"') {
			while (true) {
				ch = getch(i);
				//����������\n�������ַ����Ķ���
				if (ch == '"' || ch == '\n') break;
				else if (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) token[tc++] = ch;
				else printerror(3);
			}
			if (ch != '"') printerror(4);
			symbol = 37; //�ݴ�
			result();	
			//next(i);
			//continue;
		}
		//���ֽڷ�
		else if (ch == '+') {
			token[tc++] = ch;
			symbol = 13;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '-') {
			token[tc++] = ch;
			symbol = 14;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '*') {
			token[tc++] = ch;
			symbol = 15;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '/') {
			token[tc++] = ch;
			symbol = 16;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '(') {
			token[tc++] = ch;
			symbol = 25;
			result();
			//next(i);
			//continue;
		}
		else if (ch == ')') {
			token[tc++] = ch;
			symbol = 26;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '[') {
			token[tc++] = ch;
			symbol = 27;
			result();
			//next(i);
			//continue;
		}
		else if (ch == ']') {
			token[tc++] = ch;
			symbol = 28;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '{') {
			token[tc++] = ch;
			symbol = 29;
			result();
			//next(i);
			//continue;
		}
		else if (ch == '}') {
			token[tc++] = ch;
			symbol = 30;
			result();
			//next(i);
			//continue;
		}
		else if (ch == ',') {
			token[tc++] = ch;
			symbol = 31;
			result();
			//next(i);
			//continue;
		}
		else if (ch == ';') {
			token[tc++] = ch;
			symbol = 32;
			result();
			//next(i);
			//continue;
		}
		else printerror(1);
	}
}

void word_end() {
	outfile.close();
}
