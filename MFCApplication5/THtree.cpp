#include "stdafx.h"
#include "MFCApplication5.h"
#include "MFCApplication5Dlg.h"
#include "THtree.h"


int clean()
{
	delete[] ht;
	delete[] hcd;
	return 1;
}

/*
* 函数功能：十进制转二进制函数  //////////////////////////////
* 函数参数：num  需要转化的十进制
bin[] 存放转化完二进制的数组
*/
void DecTBin(int num, int bin[8])
{
	int i = 0;
	for (i = 0; i<8; i++)
	{
		bin[i] = 0;
	}
	i = 0;
	while (num > 0)
	{
		bin[8 - 1 - i] = num % 2;
		num = num / 2;
		i++;
	}
}

/*
* 函数功能：第一次打开文件以及对哈夫曼树数组进行初始化
* 函数参数：fileadd 目标文件的存放地址
*/
bool InitFromFile(string fileadd)
{

	fstream infile(fileadd.c_str(), ios::binary | ios::in);
	//if (!infile) { cout << "error!" << endl; return 0; }
	int table[256];           //table[256]下标从0到255一一对应ASCII值
	int i, j;
	int len = 0, num = 0;
	unsigned char ch;

	for (i = 0; i<256; i++) { table[i] = 0; maplist[i] = -1; }          //初始化table和maplist数组

	int readlen = 0;
	char buffer[bufferlen];          //设置读取缓冲区,加快读取速度
	do                               //每次读取bufferlen大小的字符
	{
		infile.read(buffer, bufferlen);
		i = 0;
		readlen = infile.gcount();
		while (i<readlen)
		{
			ch = (unsigned char)buffer[i];
			table[ch]++;
			len++;
			i++;
		}
	} while (readlen == bufferlen);

	//计算出table数组中非零数个数即哈夫曼树叶子结点数
	for (i = 0; i<256; i++)
	{
		if (table[i] != 0) num++;
	}

	ht = new HTNode[2 * num - 1];     //哈夫曼树数组
	hcd = new HCode[num];     //哈夫曼编码数组

							  //对哈夫曼树数组ht和maplist数组进行赋值
	for (i = 0, j = 0; i<256; i++)
	{
		if (table[i] != 0)
		{
			ht[j].data = i;       //数组table下标i为所对应的ASCII码
			ht[j].weight = table[i];    //数组table[i]值为i的权值
			maplist[i] = j;  //建立字符与哈夫曼编码的映射
			j++;

		}
	}
	nodenum = num;
	textlen = len;
	//cout << "............文件长度:" << len << "获取结点数" << nodenum << endl;
	infile.clear();
	infile.close();
	return 1;
}

/*
* 函数功能：构造哈夫曼树
* 函数参数：ht 哈夫曼树数组
*           n 哈夫曼树叶子结点数
*/
void HTCreat(HTNode ht[], int n)
{
	//cout << "............构造哈夫曼树过程" << endl;
	int i, k, lnode, rnode;
	int min1, min2;

	//初始化哈夫曼树数组每个元素的parent,rchild,lchild
	for (i = 0; i<2 * n - 1; i++)
	{
		ht[i].parent = ht[i].rchild = ht[i].lchild = -1;

	}

	//对哈夫曼树数组的叶子结点和扩充结点进行操作
	for (i = n; i<2 * n - 1; i++)
	{
		min1 = min2 = 2147483647;
		lnode = rnode = -1;

		//寻找所有结点中权值最小的两个结点的地址下标 lnode rnode
		for (k = 0; k <= i - 1; k++)
		{
			if (ht[k].parent == -1)
			{
				if (ht[k].weight<min1)
				{
					min2 = min1;
					min1 = ht[k].weight;
					rnode = lnode;
					lnode = k;
				}
				else if (ht[k].weight<min2)
				{
					min2 = ht[k].weight;
					rnode = k;
				}
			}
		}

		//将最小两个结点的父母值parent赋值为扩充结点的地址下标
		ht[lnode].parent = i;
		ht[rnode].parent = i;

		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		//将父母结点rchild,lchild赋值为两最小结点的地址下标
		ht[i].lchild = lnode;
		ht[i].rchild = rnode;
	}
}

/*
* 函数功能：构造哈夫曼编码
* 函数参数: ht 哈夫曼树数组
*           hcd 存放哈夫曼编码
*           n 哈夫曼结点数
*/

void HCCreat(HTNode ht[], HCode hcd[], int n)
{
	//cout << "............编码过程:" << endl;
	int i, p, c;
	HCode hc;                     //进行哈夫曼编码时，是从叶子结点开始编码，最后到根结点
	hc = new char[n];             //hc数组作为中转站，来储存编码
	int start;

	for (i = 0; i<n; i++)
	{
		//tmplen = 0;
		start = n - 1;
		hc[start] = '\0';
		c = i;
		p = ht[i].parent;             //p是i的父母
		while (p != -1)
		{
			if (ht[p].lchild == c)  //结点i 是其父母p 左孩子结点
			{
				hc[--start] = '0';
				//tmplen++;
			}
			else
			{
				hc[--start] = '1';   //结点i 是其父母p 右孩子结点
				//tmplen++;
			}
			c = p;
			p = ht[p].parent;
		}
		hcd[i] = new char[n - start];
		strcpy(hcd[i], &hc[start]);
	}
	delete[] hc;
}

/*
void WriteFile(char *tmp)
{
int i;
for(i = 0; i<8; i++)
cout<<tmp[i];
cout<<' ';
tmp = "";
}
*/

/*
* 函数功能：二进制转进制
*/
unsigned char BinTDec(char *tmp)
{
	char ch = 0;
	int i;
	for (i = 0; i<8; i++)
	{
		ch = (unsigned char)pow(2.0, 8 - i - 1)*(tmp[i] - 48) + ch;
	}
	return ch;
}

/*
* 函数功能：将编码写入文件中
* 函数参数：hcd 存放哈夫曼编码数组
*           fileadd 原文件地址字符串
*           filecode 新压缩文件的地址字符串
*           filecode2 新生产文件2的地址字符串
*/
void ShowFile(HCode hcd[], string fileadd, string filecode, string filecode2)
{
	fstream infile(fileadd.c_str(), ios::in | ios::binary);
	fstream outfile(filecode.c_str(), ios::out);
	fstream outfile2(filecode2.c_str(), ios::out);
	//if (!outfile) cout << "creat file fail!" << endl;
	//if (!outfile2) cout << "creat file fail!" << endl;

	char inbuffer[bufferlen];
	int readlen = 0, i = 0;

	//向文件filecode里写入每个结点对应得编码
	for (int i = 0; i<nodenum; i++)
	{
		outfile.write((char*)&ht[i].data, sizeof(char));
		outfile << ":";
		outfile << hcd[i] << endl;
	}

	//向文件filecode2里写入哈夫曼编码
	do
	{

		infile.read(inbuffer, bufferlen);
		readlen = infile.gcount();
		for (i = 0; i<readlen; i++)
		{

			outfile2 << hcd[maplist[(unsigned char)inbuffer[i]]] << ",";

		}


	} while (readlen == bufferlen);

	outfile2.close();
	outfile.close();
	infile.close();

}

/*
* 函数功能：压缩哈夫曼编码and写入文件
* 函数参数：hcd 哈夫曼编码数组
*           fileadd 原文件路径字符串
*           fileadd2 压缩文件路径字符串
*           fileadd3 真正的压缩文件
*/
void ConvertFile(HCode hcd[], string fileadd, string fileadd2,string fileadd3)
{
	///////////////压缩并写入文件过程/////////////////////////////////////////////////
	fstream infile(fileadd.c_str(), ios::in | ios::binary);
	fstream outfile(fileadd2.c_str(), ios::out | ios::binary);
	fstream outfile3(fileadd3.c_str(), ios::out );
	//if (!infile) cout << "open file fail!" << endl;
	//if (!outfile) cout << "creat file fail!" << endl;
	//if (!outfile3) cout << "creat reducefile fail!" << endl;
	//unsigned
	char ch;
	/////////////写入哈夫曼树/////////////////////////////////////
	ch = nodenum;
	outfile.write(&ch, 1);        //写入结点数
	ch = 8;
	outfile.write(&ch, 1);        //写入补位数(预写入)
	codelen = 0;
	outfile.write((char *)&codelen, 4);  //写入压缩后的文件的哈夫曼编码总长度(预写入)
	int h = 0;
	for (h = 0; h<nodenum; h++)
	{
		outfile.write((char*)&ht[h].data, sizeof(char));  //写入节点信息
		outfile.write((char*)&ht[h].weight, sizeof(int));  //写入权值
	}

	char tmp[8];   //设置缓冲区
	char outbuffer[bufferlen];    //设置写入缓冲区
	char *tmpcd;
	int i = 0, last = 0, j, k;    //last记录tmp

	char inbuffer[bufferlen];
	int readlen = 0;
	h = 0;
    //哈夫曼编码压缩开始
	do
	{
		infile.read(inbuffer, bufferlen);
		readlen = infile.gcount();    //本次读取的字节数
		tmpcd = hcd[maplist[(unsigned char)inbuffer[i]]];
		for (i = 0; i<readlen; )
		{

			for (j = last; j<8 && *tmpcd != '\0'; j++)
			{
				tmp[j] = *tmpcd;
				tmpcd++;
			}
			if (j == 8 && *tmpcd == '\0')    //tmp[8]恰好满八位，恰好读完一个字符完整
			{                                //的哈夫曼编码
				last = 0;     //tmp[8]清零
				i++;
				ch = BinTDec(tmp);
				outbuffer[h] = ch;    //将字符写入outbuffer[]
				h++;
				codelen++;   //压缩文件长加度加一
				if (h == bufferlen)    //如果满1024字节
				{
					outfile.write(outbuffer, bufferlen);   //写入压缩文件
					outfile3.write(outbuffer, bufferlen);  //写入压缩文件
					h = 0;    //清空outbuffer[]
				}
				if (i<readlen) tmpcd = hcd[maplist[(unsigned char)inbuffer[i]]];
				else   //下一个readlen
				{
					i = 0;
					break;
				}
			}
			else if (j<8 && *tmpcd == '\0')    //tmp[8]没满八位，恰好读完一个字符完
			{                                  //整的哈夫曼编码
				last = j;
				i++;
				if (i<readlen) tmpcd = hcd[maplist[(unsigned char)inbuffer[i]]];
				else
				{
					i = 0;
					break;
				}
				/////继续循换////
			}
			else if (j == 8 && *tmpcd != '\0')    //tmp[8]恰好满八位，没读完一个字符完
			{                                     //整的哈夫曼编码
				last = 0;    //tmp[8]清零
				ch = BinTDec(tmp);    //tmp[8]里的二进制转十进制
				outbuffer[h] = ch;    
				h++;
				codelen++;   //压缩文件长加度加一
				if (h == bufferlen)
				{
					outfile.write(outbuffer, bufferlen);    //写入压缩文件
					outfile3.write(outbuffer, bufferlen);    //写入压缩文件
					h = 0;
				}
			}
		}
	} while (readlen == bufferlen);

	////////////判断需不需要补码///////////////////////////
	if (j == 8)  //不需要补码
	{
		outfile.write(outbuffer, h);    //写入压缩文件
		outfile3.write(outbuffer, h);    //写入压缩文件
	}
	else if (j<8)  //补码
	{
		for (k = j; k<8; k++)
		{
			tmp[k] = '0';    //补0
		}
		ch = BinTDec(tmp);

		outbuffer[h] = ch;
		h++;
		outfile.write(outbuffer, h);    //写入压缩文件
		outfile3.write(outbuffer, h);    //写入压缩文件
		codelen++;   //压缩文件长加度加一
	}
	ch = 8 - j;
	rearnum = 8 - j;
	//cout << "............补位吗:" << rearnum << endl;
	outfile.seekp(1, ios::beg);
	outfile.write(&ch, 1);       //写入真正的补位数
	//cout << "............压缩后文件的哈夫曼编码长度:" << codelen << endl;
	outfile.seekp(2, ios::beg);
	outfile.write((char*)&codelen, 4);  //写入真正的压缩后的文件的哈夫曼编码总长度长度

	outfile.close();
	infile.close();
}


///////////////////////////////从文件解压/////////////////////////////////////////////
void DecompressionFile(string fileadd2, string fileadd3)
{
	//cout<<"............解压并输出新文件过程:"<<endl;
	fstream infile(fileadd2.c_str(), ios::in | ios::binary);
	fstream outfile(fileadd3.c_str(), ios::out | ios::binary);
	cout << endl;
	/////////////////读出哈夫曼树的数据/////////////
	int h = 0;
	char buffer[bufferlen];      //读入文件的缓冲区
	char outbuffer[bufferlen];   //写入文件的缓冲区

	infile.read(buffer, 1);
	nodenum = (unsigned char)*buffer;//哈夫曼树结点数
	if (nodenum == 0) nodenum = 256;
	cout << "............读出哈夫曼树结点数:" << nodenum << endl;

	infile.read(buffer, 1);
	rearnum = (unsigned char)*buffer;
	cout << "............读出哈夫曼编码补位数:" << rearnum << endl;

	infile.read((char*)&codelen, 4);
	cout << "............读出压缩后的文件的哈夫曼编码总长度:" << codelen << endl;

	//cout<<"  读出哈夫曼树数据...."<<endl;
	ht = new HTNode[2 * nodenum - 1];
	hcd = new HCode[nodenum];
	//hcdlen=new int[nodenum];
	for (h = 0; h<nodenum; h++)
	{
		infile.read(&ht[h].data, 1);
		infile.read((char*)&ht[h].weight, 4);
	}
	//////构走哈夫曼树///////
	HTCreat(ht, nodenum);
	//////构造哈夫曼编码/////
	HCCreat(ht, hcd, nodenum);
	///////////////////////////////////////////////

	///////////////////////解压并输出解压文件////////////////////////
	char *buffertmp = new char;
	int bin[8], j = 0, i = 0;
	int coderead = 0;       //记录以度的长度,用于判断何时达到文件最后一字节(用codelen比较)
	int readlen = 0;
	int child = 0;
	int last = 2 * nodenum - 2; //解压时记录上次指示器的位置
	child = last;
	unsigned char outp;

	h = 0;
	do
	{
		infile.read(buffer, bufferlen);
		readlen = infile.gcount();
		for (j = 0; j<readlen; j++)
		{
			coderead++;
			outp = buffer[j];
			DecTBin(outp, bin);
			if (coderead == codelen)   //达到文件尾
			{
				for (i = 0; i <= 8 - rearnum; i++)
				{

					if (ht[child].lchild == -1 && ht[child].rchild == -1)
					{
						//cout<<ht[child].data;
						outbuffer[h] = ht[child].data;
						h++;
						if (h == bufferlen) { outfile.write(outbuffer, bufferlen); h = 0; }

						last = 2 * nodenum - 2;
						if (i == 8 - rearnum)
						{
							if (h != 0) outfile.write(outbuffer, h);
							child = last;
							break;
						}
						else i--;
					}
					else if (i != 8)
					{
						if (bin[i] == 0) last = ht[child].lchild;
						else if (bin[i] == 1) last = ht[child].rchild;
					}
					child = last;
				}

			}
			else             //没达到文件尾
			{
				for (i = 0; i <= 8; i++)
				{
					if (ht[child].lchild == -1 && ht[child].rchild == -1)
					{
						//cout<<ht[child].data;
						outbuffer[h] = ht[child].data;
						h++;
						if (h == bufferlen)
						{
							outfile.write(outbuffer, bufferlen);
							h = 0;
						}
						last = 2 * nodenum - 2;
						if (i == 8)
						{
							child = last;
							break;
						}
						else i--;

					}
					else if (i != 8)
					{
						if (bin[i] == 0) last = ht[child].lchild;
						else if (bin[i] == 1) last = ht[child].rchild;
					}
					child = last;
				}

			}
		}
	} while (readlen == bufferlen);
	//cout<<j<<endl;
	infile.close();
	outfile.close();
}

string Compression(string fileadd)
{
	int i;
	for (i = 0; i<fileadd.length(); i++)
		if (fileadd[i] == '\\') fileadd[i] = '/';

	string fileadd2, filecode, filecode2, filecode3;                               //+
	fileadd2 = fileadd + ".bin";

	filecode = fileadd + "_code.txt";
	filecode2 = fileadd + "_hfcode.txt";
	filecode3 = fileadd + "_reducecode.txt";
	//+

	InitFromFile(fileadd);     //从文件中初始化哈夫曼树
	HTCreat(ht, nodenum);       //构造哈夫曼树
	HCCreat(ht, hcd, nodenum);   //构造哈夫曼编码
	ShowFile(hcd, fileadd, filecode, filecode2);     //展现并写入文件
	ConvertFile(hcd, fileadd, fileadd2, filecode3);   //压缩并写入文件                //+
	clean();
	return fileadd2;
}
string Decompression(string fileadd2)
{
	int i;
	for (i = 0; i<fileadd2.length(); i++)
		if (fileadd2[i] == '\\') fileadd2[i] = '/';
	string fileclass;
	string fileadd3;
	for (i = fileadd2.length() - 5; fileadd2[i] != '.' && i>0; i--)   fileclass.insert(0, fileadd2.substr(i, 1));

	if (i != 0)   fileadd3 = fileadd2.substr(0, i) + ".new" + '.' + fileclass;
	else fileadd3 = fileadd2.substr(0, fileadd2.length() - 4) + ".new";
	DecompressionFile(fileadd2, fileadd3);
	clean();
	return fileadd3;
}

void CMFCApplication5Dlg::OnBnClickedButton1()
{
	string fileadd;
	string fileadd2;
	CString m_strFilePath1 = _T("");
	LPCTSTR szFilter = _T("ALLSUPORTFILE(*.*)|*.*|");
	CFileDialog dlgFileOpenImg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if (dlgFileOpenImg.DoModal() == IDOK)
	{
		m_strFilePath1 = dlgFileOpenImg.GetPathName();
		fileadd = (LPCSTR)(CStringA)(m_strFilePath1);
		fileadd2 = Compression(fileadd);/////////////////////////////////////////
	}
	else
	{
		return;
	}
}

void CMFCApplication5Dlg::OnBnClickedButton2()
{
	string fileadd;
	string fileadd2;
	CString m_strFilePath = _T("");
	LPCTSTR szFilter = _T("Text Files(*.bin)|*.bin|All Files (*.*)|*.*||");
	CFileDialog dlgFileOpenImg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if (dlgFileOpenImg.DoModal() == IDOK)
	{
		m_strFilePath = dlgFileOpenImg.GetPathName();
		fileadd = (LPCSTR)(CStringA)(m_strFilePath);
		fileadd2 = Decompression(fileadd);////////////////////////////////////////////
	}
	else
	{
		return;
	}
}


