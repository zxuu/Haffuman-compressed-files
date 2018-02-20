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
* �������ܣ�ʮ����ת�����ƺ���  //////////////////////////////
* ����������num  ��Ҫת����ʮ����
bin[] ���ת��������Ƶ�����
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
* �������ܣ���һ�δ��ļ��Լ��Թ�������������г�ʼ��
* ����������fileadd Ŀ���ļ��Ĵ�ŵ�ַ
*/
bool InitFromFile(string fileadd)
{

	fstream infile(fileadd.c_str(), ios::binary | ios::in);
	//if (!infile) { cout << "error!" << endl; return 0; }
	int table[256];           //table[256]�±��0��255һһ��ӦASCIIֵ
	int i, j;
	int len = 0, num = 0;
	unsigned char ch;

	for (i = 0; i<256; i++) { table[i] = 0; maplist[i] = -1; }          //��ʼ��table��maplist����

	int readlen = 0;
	char buffer[bufferlen];          //���ö�ȡ������,�ӿ��ȡ�ٶ�
	do                               //ÿ�ζ�ȡbufferlen��С���ַ�
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

	//�����table�����з�������������������Ҷ�ӽ����
	for (i = 0; i<256; i++)
	{
		if (table[i] != 0) num++;
	}

	ht = new HTNode[2 * num - 1];     //������������
	hcd = new HCode[num];     //��������������

							  //�Թ�����������ht��maplist������и�ֵ
	for (i = 0, j = 0; i<256; i++)
	{
		if (table[i] != 0)
		{
			ht[j].data = i;       //����table�±�iΪ����Ӧ��ASCII��
			ht[j].weight = table[i];    //����table[i]ֵΪi��Ȩֵ
			maplist[i] = j;  //�����ַ�������������ӳ��
			j++;

		}
	}
	nodenum = num;
	textlen = len;
	//cout << "............�ļ�����:" << len << "��ȡ�����" << nodenum << endl;
	infile.clear();
	infile.close();
	return 1;
}

/*
* �������ܣ������������
* ����������ht ������������
*           n ��������Ҷ�ӽ����
*/
void HTCreat(HTNode ht[], int n)
{
	//cout << "............���������������" << endl;
	int i, k, lnode, rnode;
	int min1, min2;

	//��ʼ��������������ÿ��Ԫ�ص�parent,rchild,lchild
	for (i = 0; i<2 * n - 1; i++)
	{
		ht[i].parent = ht[i].rchild = ht[i].lchild = -1;

	}

	//�Թ������������Ҷ�ӽ�����������в���
	for (i = n; i<2 * n - 1; i++)
	{
		min1 = min2 = 2147483647;
		lnode = rnode = -1;

		//Ѱ�����н����Ȩֵ��С���������ĵ�ַ�±� lnode rnode
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

		//����С�������ĸ�ĸֵparent��ֵΪ������ĵ�ַ�±�
		ht[lnode].parent = i;
		ht[rnode].parent = i;

		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		//����ĸ���rchild,lchild��ֵΪ����С���ĵ�ַ�±�
		ht[i].lchild = lnode;
		ht[i].rchild = rnode;
	}
}

/*
* �������ܣ��������������
* ��������: ht ������������
*           hcd ��Ź���������
*           n �����������
*/

void HCCreat(HTNode ht[], HCode hcd[], int n)
{
	//cout << "............�������:" << endl;
	int i, p, c;
	HCode hc;                     //���й���������ʱ���Ǵ�Ҷ�ӽ�㿪ʼ���룬��󵽸����
	hc = new char[n];             //hc������Ϊ��תվ�����������
	int start;

	for (i = 0; i<n; i++)
	{
		//tmplen = 0;
		start = n - 1;
		hc[start] = '\0';
		c = i;
		p = ht[i].parent;             //p��i�ĸ�ĸ
		while (p != -1)
		{
			if (ht[p].lchild == c)  //���i ���丸ĸp ���ӽ��
			{
				hc[--start] = '0';
				//tmplen++;
			}
			else
			{
				hc[--start] = '1';   //���i ���丸ĸp �Һ��ӽ��
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
* �������ܣ�������ת����
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
* �������ܣ�������д���ļ���
* ����������hcd ��Ź�������������
*           fileadd ԭ�ļ���ַ�ַ���
*           filecode ��ѹ���ļ��ĵ�ַ�ַ���
*           filecode2 �������ļ�2�ĵ�ַ�ַ���
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

	//���ļ�filecode��д��ÿ������Ӧ�ñ���
	for (int i = 0; i<nodenum; i++)
	{
		outfile.write((char*)&ht[i].data, sizeof(char));
		outfile << ":";
		outfile << hcd[i] << endl;
	}

	//���ļ�filecode2��д�����������
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
* �������ܣ�ѹ������������andд���ļ�
* ����������hcd ��������������
*           fileadd ԭ�ļ�·���ַ���
*           fileadd2 ѹ���ļ�·���ַ���
*           fileadd3 ������ѹ���ļ�
*/
void ConvertFile(HCode hcd[], string fileadd, string fileadd2,string fileadd3)
{
	///////////////ѹ����д���ļ�����/////////////////////////////////////////////////
	fstream infile(fileadd.c_str(), ios::in | ios::binary);
	fstream outfile(fileadd2.c_str(), ios::out | ios::binary);
	fstream outfile3(fileadd3.c_str(), ios::out );
	//if (!infile) cout << "open file fail!" << endl;
	//if (!outfile) cout << "creat file fail!" << endl;
	//if (!outfile3) cout << "creat reducefile fail!" << endl;
	//unsigned
	char ch;
	/////////////д���������/////////////////////////////////////
	ch = nodenum;
	outfile.write(&ch, 1);        //д������
	ch = 8;
	outfile.write(&ch, 1);        //д�벹λ��(Ԥд��)
	codelen = 0;
	outfile.write((char *)&codelen, 4);  //д��ѹ������ļ��Ĺ����������ܳ���(Ԥд��)
	int h = 0;
	for (h = 0; h<nodenum; h++)
	{
		outfile.write((char*)&ht[h].data, sizeof(char));  //д��ڵ���Ϣ
		outfile.write((char*)&ht[h].weight, sizeof(int));  //д��Ȩֵ
	}

	char tmp[8];   //���û�����
	char outbuffer[bufferlen];    //����д�뻺����
	char *tmpcd;
	int i = 0, last = 0, j, k;    //last��¼tmp

	char inbuffer[bufferlen];
	int readlen = 0;
	h = 0;
    //����������ѹ����ʼ
	do
	{
		infile.read(inbuffer, bufferlen);
		readlen = infile.gcount();    //���ζ�ȡ���ֽ���
		tmpcd = hcd[maplist[(unsigned char)inbuffer[i]]];
		for (i = 0; i<readlen; )
		{

			for (j = last; j<8 && *tmpcd != '\0'; j++)
			{
				tmp[j] = *tmpcd;
				tmpcd++;
			}
			if (j == 8 && *tmpcd == '\0')    //tmp[8]ǡ������λ��ǡ�ö���һ���ַ�����
			{                                //�Ĺ���������
				last = 0;     //tmp[8]����
				i++;
				ch = BinTDec(tmp);
				outbuffer[h] = ch;    //���ַ�д��outbuffer[]
				h++;
				codelen++;   //ѹ���ļ����Ӷȼ�һ
				if (h == bufferlen)    //�����1024�ֽ�
				{
					outfile.write(outbuffer, bufferlen);   //д��ѹ���ļ�
					outfile3.write(outbuffer, bufferlen);  //д��ѹ���ļ�
					h = 0;    //���outbuffer[]
				}
				if (i<readlen) tmpcd = hcd[maplist[(unsigned char)inbuffer[i]]];
				else   //��һ��readlen
				{
					i = 0;
					break;
				}
			}
			else if (j<8 && *tmpcd == '\0')    //tmp[8]û����λ��ǡ�ö���һ���ַ���
			{                                  //���Ĺ���������
				last = j;
				i++;
				if (i<readlen) tmpcd = hcd[maplist[(unsigned char)inbuffer[i]]];
				else
				{
					i = 0;
					break;
				}
				/////����ѭ��////
			}
			else if (j == 8 && *tmpcd != '\0')    //tmp[8]ǡ������λ��û����һ���ַ���
			{                                     //���Ĺ���������
				last = 0;    //tmp[8]����
				ch = BinTDec(tmp);    //tmp[8]��Ķ�����תʮ����
				outbuffer[h] = ch;    
				h++;
				codelen++;   //ѹ���ļ����Ӷȼ�һ
				if (h == bufferlen)
				{
					outfile.write(outbuffer, bufferlen);    //д��ѹ���ļ�
					outfile3.write(outbuffer, bufferlen);    //д��ѹ���ļ�
					h = 0;
				}
			}
		}
	} while (readlen == bufferlen);

	////////////�ж��費��Ҫ����///////////////////////////
	if (j == 8)  //����Ҫ����
	{
		outfile.write(outbuffer, h);    //д��ѹ���ļ�
		outfile3.write(outbuffer, h);    //д��ѹ���ļ�
	}
	else if (j<8)  //����
	{
		for (k = j; k<8; k++)
		{
			tmp[k] = '0';    //��0
		}
		ch = BinTDec(tmp);

		outbuffer[h] = ch;
		h++;
		outfile.write(outbuffer, h);    //д��ѹ���ļ�
		outfile3.write(outbuffer, h);    //д��ѹ���ļ�
		codelen++;   //ѹ���ļ����Ӷȼ�һ
	}
	ch = 8 - j;
	rearnum = 8 - j;
	//cout << "............��λ��:" << rearnum << endl;
	outfile.seekp(1, ios::beg);
	outfile.write(&ch, 1);       //д�������Ĳ�λ��
	//cout << "............ѹ�����ļ��Ĺ��������볤��:" << codelen << endl;
	outfile.seekp(2, ios::beg);
	outfile.write((char*)&codelen, 4);  //д��������ѹ������ļ��Ĺ����������ܳ��ȳ���

	outfile.close();
	infile.close();
}


///////////////////////////////���ļ���ѹ/////////////////////////////////////////////
void DecompressionFile(string fileadd2, string fileadd3)
{
	//cout<<"............��ѹ��������ļ�����:"<<endl;
	fstream infile(fileadd2.c_str(), ios::in | ios::binary);
	fstream outfile(fileadd3.c_str(), ios::out | ios::binary);
	cout << endl;
	/////////////////������������������/////////////
	int h = 0;
	char buffer[bufferlen];      //�����ļ��Ļ�����
	char outbuffer[bufferlen];   //д���ļ��Ļ�����

	infile.read(buffer, 1);
	nodenum = (unsigned char)*buffer;//�������������
	if (nodenum == 0) nodenum = 256;
	cout << "............�����������������:" << nodenum << endl;

	infile.read(buffer, 1);
	rearnum = (unsigned char)*buffer;
	cout << "............�������������벹λ��:" << rearnum << endl;

	infile.read((char*)&codelen, 4);
	cout << "............����ѹ������ļ��Ĺ����������ܳ���:" << codelen << endl;

	//cout<<"  ����������������...."<<endl;
	ht = new HTNode[2 * nodenum - 1];
	hcd = new HCode[nodenum];
	//hcdlen=new int[nodenum];
	for (h = 0; h<nodenum; h++)
	{
		infile.read(&ht[h].data, 1);
		infile.read((char*)&ht[h].weight, 4);
	}
	//////���߹�������///////
	HTCreat(ht, nodenum);
	//////�������������/////
	HCCreat(ht, hcd, nodenum);
	///////////////////////////////////////////////

	///////////////////////��ѹ�������ѹ�ļ�////////////////////////
	char *buffertmp = new char;
	int bin[8], j = 0, i = 0;
	int coderead = 0;       //��¼�Զȵĳ���,�����жϺ�ʱ�ﵽ�ļ����һ�ֽ�(��codelen�Ƚ�)
	int readlen = 0;
	int child = 0;
	int last = 2 * nodenum - 2; //��ѹʱ��¼�ϴ�ָʾ����λ��
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
			if (coderead == codelen)   //�ﵽ�ļ�β
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
			else             //û�ﵽ�ļ�β
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

	InitFromFile(fileadd);     //���ļ��г�ʼ����������
	HTCreat(ht, nodenum);       //�����������
	HCCreat(ht, hcd, nodenum);   //�������������
	ShowFile(hcd, fileadd, filecode, filecode2);     //չ�ֲ�д���ļ�
	ConvertFile(hcd, fileadd, fileadd2, filecode3);   //ѹ����д���ļ�                //+
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


