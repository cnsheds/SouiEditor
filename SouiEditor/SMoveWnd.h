#pragma once
#include "DesignerView.h"

namespace SOUI
{

	class SMoveWnd : public SUIWindow
	{
		SOUI_CLASS_NAME(SMoveWnd, L"movewnd")
	public:
		SMoveWnd(void);
		~SMoveWnd(void);
	protected:
		void OnLButtonDown(UINT nFlags,CPoint pt);

		void OnLButtonUp(UINT nFlags,CPoint pt);

		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnPaint(IRenderTarget *pRT);

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
		    MSG_WM_PAINT_EX(OnPaint)   
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_KEYUP(OnKeyUp)
		SOUI_MSG_MAP_END()

	public:
		void AdjustRect(); //����8�����λ��;
		BOOL IsSelect();   //�Ƿ�ѡ��
		void NewWnd(CPoint pt);
		void Click(UINT nFlags,CPoint pt);

        void MoveWndSize(int x, int PosN);   //�����ұ߿���±߿�
		void MoveWndSizeLT(int x, int PosN); //������߿���ϱ߿�

		void MoveWndSize_Linear(int x , ORIENTATION orientation); //���Բ��� �����ұ߿���±߿�

		void MoveWndHorz(int x);
		void MoveWndVert(int x);

		float GetLayoutSize(SouiLayoutParamStruct *pSouiLayoutParam, int PosN);
		void SetLayoutSize(SouiLayoutParamStruct *pSouiLayoutParam, int PosN, float value);

		POS_INFO GetPosInfo(SouiLayoutParamStruct *pSouiLayoutParam, int PosN);

	protected:

		//SOUI_ATTRS_BEGIN()
		//	SOUI_ATTRS_END()

		INT Oldx;
		INT Oldy;

		HCURSOR m_hLUpRDown;
		HCURSOR m_hAll;
		HCURSOR m_hNormal;

		//�ƶ�Ԫ��
		int      m_downIndex; //�϶�ѡ��Ԫ�� -1�� 0�ڿմ� 1���Ͻ� 2�� 3���Ͻ� 4�� 5���½� 6�� 7���½� 8�� 9�м�
		POINT    m_downPt;   //�������λ��
		CRect     m_rcPos1,m_rcPos2,m_rcPos3,m_rcPos4,m_rcPos5,m_rcPos6,m_rcPos7,m_rcPos8; //�˸��ǵ�,�����Ͻ�˳ʱ����ת

		CRect m_rcCenter; //�϶�λ��

		//�϶�����
		int m_downWindow;  // 0:δ����,4:�����ұ߿�,5.�������½�, 6:�����±߿�

		INT m_StateMove;	   //�ؼ���Сλ���Ƿ񱻸ı�;

	public:
		SDesignerView *m_Desiner;
		SUIWindow *m_pRealWnd;  //�����������ص�ʵ�ʴ���
		BOOL m_bCtrlShift;
	};

}
