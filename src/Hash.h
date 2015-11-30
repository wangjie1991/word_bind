#ifndef _HASH_STRING_
#define _HASH_STRING_

#define MAXTABLELEN 1024    // Ĭ�Ϲ�ϣ�������С
//////////////////////////////////////////////////////////////////////////
// ��ϣ��������
typedef struct _HASHTABLE
{
	unsigned long nHashA;
	unsigned long nHashB;
	int  nValue;
	bool bExists;
}HASHTABLE, *PHASHTABLE;

class StringHash
{
public:
	StringHash(const long nTableLength = MAXTABLELEN);
	~StringHash(void);
private:
	unsigned long cryptTable[0x500];
	unsigned long m_tablelength;    // ��ϣ��������
	HASHTABLE *m_HashIndexTable;
private:
	void InitCryptTable();    // �Թ�ϣ������Ԥ����
	unsigned long HashString(const char *lpszString, unsigned long dwHashType); // ��ȡ��ϣֵ
public:
	bool Add(char* url,int id);
	int Find(char* url);    // ���url�Ƿ�hash��
	void OutputTable();
};

#endif
