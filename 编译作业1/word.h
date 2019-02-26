#pragma once

extern int num; //当前读到的整数

extern int lc; //行缓冲区指针

extern int ll; //行缓冲区长度

extern int lines; //行数

extern int resultn; //当前读的单词数

extern char token[200];//sym缓冲区

extern char line[1024]; //行缓冲区

extern int symbol; //当前读到的字符串的类型

void init();
void getsym(int i);
void word_end();
void setmark(int i);
