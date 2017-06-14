#pragma once

#include <core/simplewnd.h>

class CScintillaModule
{
public:
	CScintillaModule();
	~CScintillaModule();

	BOOL operator !() const
	{
		return m_hModule==NULL;
	}
protected:
	HINSTANCE m_hModule;
};

typedef void (*FN_CALLBACK)(int custom_msg);
namespace SOUI {
	class SDesignerView;
}

// CScintillaWnd
class CScintillaWnd : public CSimpleWnd
{
public:
	CScintillaWnd();
	virtual ~CScintillaWnd();
	BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND hParentWnd, UINT nID,HINSTANCE hInst);
	BOOL OpenFile(LPCTSTR lpFileName);
	void DoSave();

	void SetDirty(bool bDirty);

	BOOL SaveFile(LPCTSTR lpFileName);

	void BindDesignView(SDesignerView* pWnd);

	LPCTSTR GetOpenedFileName(){return m_strFileName;}
	void SetOpenedFileName(LPCTSTR pszFileName){m_strFileName=pszFileName;}

	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) {
		return SendMessage(Msg, wParam, lParam);
	}

	void SetOnDirtyWnd(SWindow* pWnd, int index);
	bool m_bDirty;		//指示文档是否已修改

protected:
	// 显示行号
	void UpdateLineNumberWidth(void);
	void InitScintillaWnd(void);
	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff,0xff,0xff), int size = 0, const char* face = NULL);
	void SetFold();
	// 设置XML的语法规则
	void SetXmlLexer(COLORREF bkColor);

	void findMatchingBracePos(int & braceAtCaret, int & braceOpposite);
	bool doMatch(); //匹配括号并加亮缩进向导;
	
	void GetRange(int start, int end, char* text);

	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

	BEGIN_MSG_MAP_EX(CScintillaWnd)
		MSG_OCM_NOTIFY(OnNotify)
		MSG_WM_KEYDOWN(OnKeyDown)
	END_MSG_MAP()

	SStringT m_strFileName;
	SDesignerView *m_pDesign;
	FN_CALLBACK m_fnCallback;

	SWindow* m_pDirtyWnd;
	int m_nPageIndex;
};
