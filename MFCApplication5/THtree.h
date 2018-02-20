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
	char data;   //���ֵ
	int weight;  //Ȩֵ;
	int parent;
	int lchild;
	int rchild;
};

typedef char* HCode;

HTNode *ht;
HCode *hcd;
//int  *hcdlen;   //��Ӧ���������볤��
//long *cdlen;    //�����������ܳ���
int maplist[256];    //�����ַ�������������ӳ��
int nodenum = 0;         //�������������
int rearnum = 0;          //����������β����
int textlen = 0;          //��ѹ�����ļ�����
int codelen = 0;          //ѹ������ļ��Ĺ����������ܳ���
int const bufferlen = 1024;               //���ö�ȡ����������

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