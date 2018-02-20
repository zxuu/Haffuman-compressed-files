#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>

using namespace std;
struct HTNode
{
	char data;   //结点值
	int weight;  //权值;
	int parent;
	int lchild;
	int rchild;
};

typedef char* HCode;

HTNode *ht;
HCode *hcd;
//int  *hcdlen;   //对应哈夫曼编码长度
//long *cdlen;    //哈夫曼编码总长度
int maplist[256];    //建立字符与哈夫曼编码的映射
int nodenum = 0;         //哈夫曼树结点数
int rearnum = 0;          //哈夫曼编码尾补码
int textlen = 0;          //需压缩的文件长度
int codelen = 0;          //压缩后的文件的哈夫曼编码总长度
int const bufferlen = 1024;               //设置读取缓冲区长度

int clean();
void DecTBin(int num, int bin[8]);
bool InitFromFile(string fileadd);
void HTCreat(HTNode ht[], int n);
void HCCreat(HTNode ht[], HCode hcd[], int n);
unsigned char BinTDec(char *tmp);
void ShowFile(HCode hcd[], string fileadd, string filecode, string filecode2);
void ConvertFile(HCode hcd[], string fileadd, string fileadd2, string fileReduce);
void DecompressionFile(string fileadd2, string fileadd3);
string Compression(string fileadd);
string Decompression(string fileadd2);


#endif