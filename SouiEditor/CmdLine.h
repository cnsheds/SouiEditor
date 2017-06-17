
#pragma once

/*
	CCmdLine cmdLine(GetCommandLine());
	if (cmdLine.GetParamCount() > 0)
	{
		cmdLine.GetParam(0);	//0 �ŵ��ǽ���·��
		cmdLine.GetParam(1);	//��1��ʼ���ǲ���
	}
*/

class CCmdLine
{
private:
	enum {MAX_PARAM_COUNT = 25};//��ദ��25������
	LPTSTR m_szStrBuf;
	int m_nBufSize;
	LPTSTR m_szParams[MAX_PARAM_COUNT];
	int m_nParamCount;
	//���ߺ���,����������
	void ProcessCmdLine()
	{
		if(m_szStrBuf == NULL) return;
		
		bool blInQt = false;//�Ƿ���һ��������,"������״̬"��־
		bool blInParam = false;//�Ƿ���һ��������
		//ѭ���������Buf
		for (LPTSTR p = m_szStrBuf; *p != 0; p++)
		{
			switch(*p)
			{
			case _T(' '): //---------------------�ո�
				{
					//�������ŷ�Χ�ڵĿո�
					if(blInQt) break;
					if(blInParam)//����������
					{
						*p = _T('\0');
						blInParam = false;
					}
					break;
				}
			case _T('\"'): //---------------------����
				{
					//��ת"������״̬"��־
					blInQt = !blInQt;
					if(blInParam)//����������
					{
						*p = _T('\0');
						blInParam = false;
					}
					else  //һ��������ʼ��
					{
						//һ��������ʼ��
						blInParam = true;
						if(m_nParamCount >= MAX_PARAM_COUNT || _T('\0') == *(p + 1)) return;
						m_szParams[m_nParamCount++] = ++p;
					}
					break;
				}
			default:  //---------------------�����ַ�
				{
					if(!blInParam)
					{
						//һ��������ʼ��
						blInParam = true;
						if(m_nParamCount >= MAX_PARAM_COUNT) return;
						m_szParams[m_nParamCount++] = p;
					}
					break;
				}
			}
		}
	}
public:
	//���캯��
	CCmdLine(LPCTSTR szCmdLine)
		:m_szStrBuf(NULL)
		,m_nBufSize(0)
		,m_nParamCount(0)
	{
		//��ʼ��ָ������
		memset(m_szParams, 0, MAX_PARAM_COUNT * sizeof(TCHAR));
		int m_nBufSize = lstrlen(szCmdLine) + 1;
		//�����ڴ�,����ͨ���������ݽ������ַ���
		if(m_nBufSize > 1) m_szStrBuf = new TCHAR[m_nBufSize];
		lstrcpyn(m_szStrBuf, szCmdLine, m_nBufSize);
		m_szStrBuf[m_nBufSize - 1] = 0;//��֤�ı���������
		//����������
		ProcessCmdLine();
	}
	~CCmdLine()
	{
		delete[] m_szStrBuf;
	}
	//ȡ�������еĲ�������
	int GetParamCount(void) const
	{
		return m_nParamCount;
	}
	//ȡ��ĳ�������в���
	LPCTSTR GetParam(int nIndex)
	{
		if(nIndex >= MAX_PARAM_COUNT) return NULL;
		return m_szParams[nIndex];
	}
};

