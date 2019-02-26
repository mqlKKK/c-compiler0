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

string key[50]; //保留字表
char line[1024]; //行缓冲区
int lc; //行指针
int ll; //行长度
int lines; //行编号
char token[200]; //sym缓冲区
int tc; //sym缓冲区指针
int symbol; //当前读到的字符串的类型
char ch; //当前读到的字符
int num; //当前读到的整数
int resultn; //当前读的单词数
ifstream infile; //源代码文件
ofstream outfile; //词法分析结果文件

char filename[20];

//输出词法分析结果到控制台和文件中
void result() {
	
	cout << resultn++ << "	" << key[symbol] << "		" << token << endl;
	outfile << resultn - 1 << "	" << key[symbol] << "		" << token << endl;
	
	if (resultn > 9999999) {
		cout << "编译失败！" << endl;
		exit(0);
	}
}
//初始化保留字和特殊字符,读入文件
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
	cout << "请输入源代码文件名：" << endl;
	cin.getline(filename, 90);
	infile.open(filename);
	if (!infile.is_open()) {
		cout << "找不到输入文件！" << endl;
		exit(0);
	}
	outfile.open("result.txt");
	if (!outfile.is_open()) {
		cout << "无法创建输出文件！" << endl;
		exit(0);
	}

}
//得到一个字符
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
	//cout << "读到  " << line[lc - 1] << endl;
	return line[lc - 1];
}
//判断是否已经读完文件
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
//读下一个单词
void next(int i) {
	memset(token, 0, sizeof(token));
	tc = 0;
	ch = getch(i);
}
//判断token是否是关键字
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
//打印错误
void printerror(int op) {
	if (op == 1) cout << "#Line  " << lines << " ： 存在不合法的字符 [!]" << endl;
	if (op == 2) cout << "#Line  " << lines << " ： 缺少右单引号 " << endl;
	if (op == 3) cout << "#Line  " << lines << " ： 字符串中存在不合法字符 '" << ch << "'" << endl;
	if (op == 4) cout << "#Line  " << lines << " ： 字符串缺少[\"] " << endl;

}
//词法分析
void getsym(int i) {
	next(i);
	//忽略单词间的空格，制表符，换行符
	while (ch == ' ' || ch == '	' || ch == '\n') {
		ch = getch(i);
		continue;
	}
	if (isEND())
	{
		//-A-保留字,标识符
		if (isalpha(ch) || ch == '_') {
			do {
				token[tc++] = tolower(ch);
				ch = getch(i);
			} while (isalnum(ch) || ch == '_'); 
			//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
			if(ch >= 32 && ch <= 126)lc--;
			symbol = isKEY();
			result();
			//next(i);
			//continue;
		}
		//-A-无符号整数
		else if (isdigit(ch)) {
			num = 0;
			do {
				num = num * 10 + (ch - '0');
				token[tc++] = ch;
				ch = getch(i);
			} while (isdigit(ch));
			//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
			if (ch >= 32 && ch <= 126)lc--;
			symbol = 35;
			result();
			//next(i);
			//continue;
		}
		//-A-双字符分界符 < > ! =
		else if (ch == '<') {
			token[tc++] = ch;
			ch = getch(i);
			if (ch == '=') {
				token[tc++] = ch;
				symbol = 18;
			}
			else {
				//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
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
				//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
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
				//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
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
				//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
				if (ch >= 32 && ch <= 126)lc--;
				symbol = 33;
			}
			result();
			//next(i);
			//continue;
		}
		//-A-字符
		else if (ch == '\'') {
			ch = getch(i);
			if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || isalnum(ch) || ch == '_')
				token[tc++] = ch;
			else {
				//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
				if (ch >= 32 && ch <= 126) lc--;
				printerror(2);
			}
			ch = getch(i);
			if (ch != '\'') {
				printerror(2);
				//如果ch是换行符，则不需要回退行指针（回退会出现lc =  -1）
				if (ch >= 32 && ch <= 126) lc--;
			}
			symbol = 38; //容错，将第一个字符当做读到的字符，后面的字符继续按文法读取
			result();
			//next(i);
			//continue;
		}
		//-A-字符串
		else if (ch == '"') {
			while (true) {
				ch = getch(i);
				//读到”或者\n，结束字符串的读入
				if (ch == '"' || ch == '\n') break;
				else if (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) token[tc++] = ch;
				else printerror(3);
			}
			if (ch != '"') printerror(4);
			symbol = 37; //容错
			result();	
			//next(i);
			//continue;
		}
		//单分节符
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
