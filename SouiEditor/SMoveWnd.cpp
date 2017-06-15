#include "stdafx.h"
#include "SMoveWnd.h"
#include "CNewGuid.h"
#include "control/SMessageBox.h"

#define  POINT_SIZE      4     //Ԫ���϶����С

#define HORZ_LT 0
#define VERT_LT 1

#define HORZ 2
#define VERT 3

#define UP     0
#define DOWN   1
#define LEFT   2
#define RIGHT  3


namespace SOUI
{
	SMoveWnd::SMoveWnd()
	{
		m_hLUpRDown = GETRESPROVIDER->LoadCursor(L"sizenwse");
		m_hAll = GETRESPROVIDER->LoadCursor(L"sizeall");
		m_hNormal = GETRESPROVIDER->LoadCursor(m_style.m_strCursor);
		m_downWindow = 0;
		m_bFocusable = TRUE;
		m_pRealWnd = NULL;
		m_bDrawFocusRect = FALSE;
		m_StateMove = 0;
	}

	SMoveWnd::~SMoveWnd()
	{
	}

	void SOUI::SMoveWnd::OnPaint(IRenderTarget *pRT)
	{
		__super::OnPaint(pRT);

		//���ｫʵ�ʿؼ��͸�����ʵ�ʿؼ�����Ĳ��ֿؼ���λ�ø���Ϊһ����
		if (m_pRealWnd == m_Desiner->m_pRealWndRoot)
		{
			CRect rectR;
			CRect rectRP;
			m_pRealWnd->GetWindowRect(rectR);
			m_pRealWnd->GetParent()->GetWindowRect(rectRP);

			SouiLayoutParam *pMoveWndLayout = GetLayoutParamT<SouiLayoutParam>();
			SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pMoveWndLayout->GetRawData();
			//pSouiLayoutParamStruct->pos[0].nPos = 20;
			//pSouiLayoutParamStruct->pos[1].nPos = 20;
			//pSouiLayoutParamStruct->pos[2].nPos = rectR.right - rectR.left;
			//pSouiLayoutParamStruct->pos[3].nPos = rectR.bottom - rectR.top;

			//pMoveWndLayout->SetSpecifiedSize(Horz, rectR.right - rectR.left);
			//pMoveWndLayout->SetSpecifiedSize(Vert, rectR.bottom - rectR.top);
			pSouiLayoutParamStruct->posLeft.nPos.fSize = 20;
			pSouiLayoutParamStruct->posTop.nPos.fSize = 20;
			pSouiLayoutParamStruct->posRight.nPos.fSize = rectR.right - rectR.left;
			pSouiLayoutParamStruct->posBottom.nPos.fSize = rectR.bottom - rectR.top;

			SLayoutSize LayoutSize = pMoveWndLayout->GetSpecifiedSize(Horz);
			LayoutSize.fSize = rectR.right - rectR.left;
			pMoveWndLayout->SetSpecifiedSize(Horz, LayoutSize);

			LayoutSize = pMoveWndLayout->GetSpecifiedSize(Vert);
			LayoutSize.fSize = rectR.bottom - rectR.top;
			pMoveWndLayout->SetSpecifiedSize(Vert, LayoutSize);
			//pMoveWndLayout->SetSpecifiedSize(Horz, rectR.right - rectR.left);
			//pMoveWndLayout->SetSpecifiedSize(Vert, rectR.bottom - rectR.top);

			CRect rect;
			GetWindowRect(rect);

			if (!rect.EqualRect(rectR))
			{
				GetParent()->RequestRelayout();
				GetParent()->UpdateLayout();
			}
		}
		else
		{
			//����MoveWnd��λ�ú�RealWndһ��
			CRect rectR;
			CRect rectRP;
			m_pRealWnd->GetWindowRect(rectR);
			m_pRealWnd->GetParent()->GetWindowRect(rectRP);

			SouiLayoutParam *pMoveWndLayout = GetLayoutParamT<SouiLayoutParam>();
			SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pMoveWndLayout->GetRawData();
			//pSouiLayoutParamStruct->pos[0].nPos = rectR.left - rectRP.left;
			//pSouiLayoutParamStruct->pos[1].nPos = rectR.top - rectRP.top;
			//pSouiLayoutParamStruct->pos[2].nPos = rectR.right - rectR.left;
			//pSouiLayoutParamStruct->pos[3].nPos = rectR.bottom - rectR.top;

			//pMoveWndLayout->SetSpecifiedSize(Horz, rectR.right - rectR.left);
			//pMoveWndLayout->SetSpecifiedSize(Vert, rectR.bottom - rectR.top);

			pSouiLayoutParamStruct->posLeft.nPos.fSize = rectR.left - rectRP.left;
			pSouiLayoutParamStruct->posTop.nPos.fSize = rectR.top - rectRP.top;
			pSouiLayoutParamStruct->posRight.nPos.fSize = rectR.right - rectR.left;
			pSouiLayoutParamStruct->posBottom.nPos.fSize = rectR.bottom - rectR.top;

			SLayoutSize LayoutSize = pMoveWndLayout->GetSpecifiedSize(Horz);
			LayoutSize.fSize = rectR.right - rectR.left;
			pMoveWndLayout->SetSpecifiedSize(Horz, LayoutSize);

			LayoutSize = pMoveWndLayout->GetSpecifiedSize(Vert);
			LayoutSize.fSize = rectR.bottom - rectR.top;
			pMoveWndLayout->SetSpecifiedSize(Vert, LayoutSize);

			CRect rect;
			GetWindowRect(rect);

			if (!rect.EqualRect(rectR))
			{
				GetParent()->RequestRelayout();
				GetParent()->UpdateLayout();
			}
		}

		CRect rect;
		GetWindowRect(rect);

		SPainter painter;
		BeforePaint(pRT, painter);

		AdjustRect();

		int n = POINT_SIZE / 2;
		//rect.DeflateRect(n,n,n,n);

		CAutoRefPtr<IPen> pen, oldpen;

		if (IsSelect())
		{
			pRT->CreatePen(PS_SOLID, RGBA(255, 0, 0, 255), 2, &pen);
			pRT->SelectObject(pen, (IRenderObj**)&oldpen);
			pRT->DrawRectangle(m_rcPos1);
			pRT->DrawRectangle(m_rcPos2);
			pRT->DrawRectangle(m_rcPos3);
			pRT->DrawRectangle(m_rcPos4);
			pRT->DrawRectangle(m_rcPos5);
			pRT->DrawRectangle(m_rcPos6);
			pRT->DrawRectangle(m_rcPos7);
			pRT->DrawRectangle(m_rcPos8);

			pRT->DrawRectangle(rect);
			pRT->SelectObject(oldpen);
		}
		else
		{
			pRT->CreatePen(PS_SOLID, RGBA(172, 172, 172, 255), 1, &pen);
			pRT->SelectObject(pen, (IRenderObj**)&oldpen);

			pRT->DrawRectangle(rect);
			pRT->SelectObject(oldpen);
		}

		AfterPaint(pRT, painter);
	}

	void SMoveWnd::OnLButtonDown(UINT nFlags, CPoint pt)
	{
		if (m_Desiner->m_nState == 1)//����Ǵ����ؼ�״̬
		{
			//��갴��ʱ�����ؼ�
			NewWnd(pt);
			m_Desiner->CreatePropGrid(m_Desiner->m_xmlNode.name());
			m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
			m_Desiner->AddCodeToEditor(NULL);
			OnLButtonUp(nFlags, pt);

			return;
		}

		if (m_pRealWnd != m_Desiner->m_pRealWndRoot)
		{
			SStringT s;
			s.Format(_T("%d"), m_pRealWnd->GetUserData());
			m_Desiner->SetCurrentCtrl(m_Desiner->FindNodeByAttr(m_Desiner->m_CurrentLayoutNode, L"data", s), this);
			m_Desiner->CreatePropGrid(m_Desiner->m_xmlNode.name());
			m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
		}
		else
		{
			m_Desiner->SetCurrentCtrl(m_Desiner->m_CurrentLayoutNode, this);
			m_Desiner->CreatePropGrid(_T("hostwnd"));
			m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
		}
		m_Desiner->AddCodeToEditor(NULL);

		SWindow::OnLButtonDown(nFlags, pt);

		Oldx = pt.x;
		Oldy = pt.y;

		if (m_rcPos8.PtInRect(pt))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			m_downWindow = 8;
		}
		else
		{
			if (m_rcPos1.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
				m_downWindow = 1;
			}
			else
				if (m_rcPos2.PtInRect(pt))
				{
					SetCursor(LoadCursor(NULL, IDC_SIZENS));
					m_downWindow = 2;
				}
				else
					if (m_rcPos3.PtInRect(pt))
					{
						//SetCursor(LoadCursor(NULL,IDC_SIZEWE));
						m_downWindow = 3;
					}
					else
						if (m_rcPos4.PtInRect(pt))
						{
							SetCursor(LoadCursor(NULL, IDC_SIZEWE));
							m_downWindow = 4;
						}
						else if (m_rcPos5.PtInRect(pt))
						{
							SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
							m_downWindow = 5;
						}
						else if (m_rcPos6.PtInRect(pt))
						{
							SetCursor(LoadCursor(NULL, IDC_SIZENS));
							m_downWindow = 6;
						}
						else if (m_rcCenter.PtInRect(pt))
						{
							//SetCursor(LoadCursor(NULL,IDC_SIZEALL));
							m_downWindow = 9;
						}
		}
	}

	void SMoveWnd::OnLButtonUp(UINT nFlags, CPoint pt)
	{
		SWindow::OnLButtonUp(nFlags, pt);

		//���ؼ���λ�ø��µ�xml�ڵ�;
		if (m_StateMove)
		{
			//if (m_pRealWnd != m_Desiner->m_pRealWndRoot)
			//{
			m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
			m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
			m_Desiner->AddCodeToEditor(NULL);
			m_StateMove = 1;
			//}
		}

		m_downWindow = 0;
	}

	void SMoveWnd::OnMouseMove(UINT nFlags, CPoint pt)
	{
		if (!IsSelect())
		{
			return;
		}

		if (0 == m_downWindow) //��ǰ�ؼ�û�б�����
		{
			//�����ǰֻѡ��һ���ؼ�����Ϊ��ǰ�ؼ�
			//if( mainWnd.m_designerView.m_CurSelCtrlList.size() == 1 && mainWnd.m_designerView.m_CurSelCtrlList[0]==this)
			//{
			//�ı���
			if (m_rcPos1.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			}
			else if (m_rcPos2.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENS));
			}
			else if (m_rcPos3.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			}
			else  if (m_rcPos4.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			}
			else if (m_rcPos5.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			}
			else if (m_rcPos6.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENS));
			}
			else if (m_rcPos7.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			}
			else  if (m_rcPos8.PtInRect(pt))
			{
				SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			}
			//else  if(m_rcCenter.PtInRect(pt) && (m_Desiner->m_nState == 0)) //�м�λ�ã����϶��ؼ�
			//{
			//	SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			//}

			//}
		}
		else//�ؼ��������϶���С��λ�õ����
		{
			int x = pt.x - Oldx;
			int y = pt.y - Oldy;
			int x1;
			int y1;


			BOOL bx = FALSE;
			BOOL by = FALSE;

			if (abs(pt.x - Oldx) >= 8 || abs(pt.y - Oldy) >= 8)
			{
				x1 = abs(pt.x - Oldx);
				x1 = x1 / 8;

				y1 = abs(pt.y - Oldy);
				y1 = y1 / 8;

				if (abs(pt.x - Oldx) >= 8)
				{
					if (x > 0)
					{
						x = 8;
					}
					else
					{
						x = -8;
					}

					bx = TRUE;
				}
				else
				{
					x = 0;
				}

				if (abs(pt.y - Oldy) >= 8)
				{
					if (y > 0)
					{
						y = 8;
					}
					else
					{
						y = -8;
					}

					by = TRUE;
				}
				else
				{
					y = 0;
				}
			}
			else
			{
				return;
			}

			switch (m_downWindow)
			{
				/*��߿�*/
			case 8:
			{
				if (bx)
				{
					if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
					{
						return; //���Բ��ֲ����϶���߿�
					}
					MoveWndSizeLT(x*x1, HORZ_LT);
				}
			}
			break;;

			/*���Ͻ�*/
			case 1:
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					return; //���Բ��ֲ����϶����Ͻ�
				}

				if (bx)
				{
					MoveWndSizeLT(x*x1, HORZ_LT);
				}
				if (by)
				{
					MoveWndSizeLT(y*y1, VERT_LT);
				}
			}
			break;

			/*�ϱ߿�*/
			case 2:
			{
				if (by)
				{
					if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
					{
						return; //���Բ��ֲ����϶��ϱ߿�
					}
					MoveWndSizeLT(y*y1, VERT_LT);
				}
			}
			break;

			/*���Ͻ�*/
			case 3:
				return;

				/*�ұ߿�*/
			case 4:
			{
				if (bx)
				{
					if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
					{
						MoveWndSize_Linear(x*x1, Horz);
					}
					else
					{
						MoveWndSize(x*x1, HORZ);
					}
				}
			}
			break;

			/*���½�*/
			case 5:
			{
				if (bx)
				{
					if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
					{
						MoveWndSize_Linear(x*x1, Horz);
					}
					else
					{
						MoveWndSize(x*x1, HORZ);
					}
				}
				if (by)
				{
					if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
					{
						MoveWndSize_Linear(y*y1, Vert);
					}
					else
					{
						MoveWndSize(y*y1, VERT);
					}
				}
			}
			break;

			/*�±߿�*/
			case 6:
			{
				if (by)
				{
					if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
					{
						MoveWndSize_Linear(y*y1, Vert);
					}
					else
					{
						MoveWndSize(y*y1, VERT);
					}
				}
			}
			break;

			/*���½�*/
			case 7:
				return;

				/*�м��϶�*/
			case 9:
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					return; //���Բ��ֲ����϶�
				}
				if (bx)
				{
					MoveWndHorz(x*x1);
				}
				if (by)
				{
					MoveWndVert(y*y1);
				}
			}
			break;

			default:
				return;
			}


			Oldx = Oldx + x*x1;
			Oldy = Oldy + y*y1;
			m_pRealWnd->GetParent()->RequestRelayout();
			m_pRealWnd->GetParent()->UpdateLayout();

			GetParent()->RequestRelayout();
			GetParent()->UpdateLayout();

			m_StateMove = 1;
			m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
			m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);

			GetParent()->Invalidate(); //ˢ�¸�����
		}
	}


	void SMoveWnd::AdjustRect()
	{
		CRect rect;
		GetWindowRect(rect);





		m_rcPos1.left = rect.left;
		m_rcPos1.top = rect.top;
		m_rcPos1.right = rect.left + POINT_SIZE;
		m_rcPos1.bottom = rect.top + POINT_SIZE;

		m_rcPos2.left = rect.left + (rect.right - rect.left) / 2 - POINT_SIZE / 2;
		m_rcPos2.top = rect.top;
		m_rcPos2.right = m_rcPos2.left + POINT_SIZE;
		m_rcPos2.bottom = rect.top + POINT_SIZE;

		m_rcPos3.left = rect.right - POINT_SIZE;
		m_rcPos3.top = rect.top;
		m_rcPos3.right = rect.right;
		m_rcPos3.bottom = rect.top + POINT_SIZE;

		m_rcPos4.left = rect.right - POINT_SIZE;
		m_rcPos4.top = rect.top + (rect.bottom - rect.top) / 2 - POINT_SIZE / 2;
		m_rcPos4.right = rect.right;
		m_rcPos4.bottom = m_rcPos4.top + POINT_SIZE;

		m_rcPos5.left = rect.right - POINT_SIZE;
		m_rcPos5.top = rect.bottom - POINT_SIZE;
		m_rcPos5.right = rect.right;
		m_rcPos5.bottom = rect.bottom;

		m_rcPos6.left = m_rcPos2.left;
		m_rcPos6.top = rect.bottom - POINT_SIZE;
		m_rcPos6.right = m_rcPos2.right;
		m_rcPos6.bottom = rect.bottom;

		m_rcPos7.left = rect.left;
		m_rcPos7.top = rect.bottom - POINT_SIZE;
		m_rcPos7.right = rect.left + POINT_SIZE;
		m_rcPos7.bottom = rect.bottom;

		m_rcPos8.left = rect.left;
		m_rcPos8.top = m_rcPos4.top;
		m_rcPos8.right = rect.left + POINT_SIZE;
		m_rcPos8.bottom = m_rcPos4.bottom;

		m_rcCenter.left = rect.left + POINT_SIZE;
		m_rcCenter.top = rect.top + POINT_SIZE;
		m_rcCenter.right = rect.right - POINT_SIZE;
		m_rcCenter.bottom = rect.bottom - POINT_SIZE;

	}

	BOOL SMoveWnd::IsSelect()
	{
		if (m_Desiner == NULL)
			return FALSE;

		if (m_Desiner->m_CurSelCtrl == this)
		{
			return TRUE;
		}

		return FALSE;
	}


	void SMoveWnd::MoveWndHorz(int x)
	{
		//ˮƽ�ƶ�
		if (m_pRealWnd == m_Desiner->m_pRealWndRoot)
		{
			return;
		}

		ILayoutParam *pSouiLayoutParam = GetLayoutParam();
		SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pSouiLayoutParam->GetRawData();

		ILayoutParam *pSouiLayoutParam1 = m_pRealWnd->GetLayoutParam();
		SouiLayoutParamStruct *pSouiLayoutParamStruct1 = (SouiLayoutParamStruct*)pSouiLayoutParam1->GetRawData();


		//��margin�����
		SwndStyle &style = m_pRealWnd->GetParent()->GetStyle();
		int nMargin = 0;

		CRect rcMargin = style.GetMargin();

		nMargin = rcMargin.left;

		//�����϶���left����С��0
		if (pSouiLayoutParamStruct->posLeft.nPos.fSize + x - nMargin < 0 && x < 0)
		{
			return;
		}


		//�����϶���right���ܴ��ڸ��ؼ���right
		if (x > 0)
		{
			CRect r;
			GetParent()->GetWindowRect(r);

			if (pSouiLayoutParamStruct->posLeft.nPos.fSize + pSouiLayoutParam->GetSpecifiedSize(Horz).fSize + x + rcMargin.right > r.right - r.left)
			{
				return;
			}
		}



		if (pSouiLayoutParamStruct1->nCount == 2) //������������
		{
			if (pSouiLayoutParamStruct1->posLeft.cMinus == -1)// ����1Ϊ���������
			{
				if (pSouiLayoutParamStruct1->posLeft.nPos.fSize - x < 0)
				{
					return;
				}

				pSouiLayoutParamStruct->posLeft.nPos.fSize = pSouiLayoutParamStruct->posLeft.nPos.fSize + x;
				pSouiLayoutParamStruct1->posLeft.nPos.fSize = pSouiLayoutParamStruct1->posLeft.nPos.fSize - x;

			}
			else//����1Ϊ����
			{
				if (pSouiLayoutParamStruct1->posRight.cMinus == -1)
				{
					if (pSouiLayoutParamStruct1->posRight.nPos.fSize - x < 0)
					{
						return;
					}
				}

				pSouiLayoutParamStruct->posLeft.nPos.fSize = pSouiLayoutParamStruct->posLeft.nPos.fSize + x;
				pSouiLayoutParamStruct1->posLeft.nPos.fSize = pSouiLayoutParamStruct1->posLeft.nPos.fSize + x;
			}
		}
		else if (pSouiLayoutParamStruct1->nCount == 4)
		{


			int nPosTop, nPosTop1;
			int nPosButtom, nPosButtom1;



			/************************* �ƶ�top�� **************************************/

			if (pSouiLayoutParamStruct1->posLeft.cMinus == -1)// ����1Ϊ���������
			{
				if (pSouiLayoutParamStruct1->posLeft.nPos.fSize - x < 0)
				{
					return;
				}

				nPosTop = pSouiLayoutParamStruct->posLeft.nPos.fSize + x; //layout->pos[1].nPos = layout->pos[1].nPos + x;   
				nPosTop1 = pSouiLayoutParamStruct1->posLeft.nPos.fSize - x; //layout1->pos[1].nPos = layout1->pos[1].nPos - x;

			}
			else//����1Ϊ����
			{
				if (pSouiLayoutParamStruct1->posRight.cMinus == -1)
				{
					if (pSouiLayoutParamStruct1->posRight.nPos.fSize - x < 0)
					{
						return;
					}
				}

				nPosTop = pSouiLayoutParamStruct->posLeft.nPos.fSize + x;    //layout->pos[1].nPos = layout->pos[1].nPos + x;
				nPosTop1 = pSouiLayoutParamStruct1->posLeft.nPos.fSize + x;  //layout1->pos[1].nPos = layout1->pos[1].nPos + x;
			}

			/************************* �ƶ� top **************************************/




			/************************* �ƶ� buttom  **************************************/
			if (pSouiLayoutParamStruct1->posRight.pit == PIT_SIZE)  //100, 100 ,@5, @5�������
			{
				//

				pSouiLayoutParamStruct->posLeft.nPos.fSize = nPosTop;
				pSouiLayoutParamStruct1->posLeft.nPos.fSize = nPosTop1;

			}
			else
			{
				if (pSouiLayoutParamStruct1->posRight.cMinus == -1)// ����1Ϊ���������
				{
					if (pSouiLayoutParamStruct1->posRight.nPos.fSize - x < 0)
					{
						return;
					}

					nPosButtom = pSouiLayoutParamStruct->posRight.nPos.fSize + x;  //layout->pos[3].nPos = layout->pos[3].nPos + x;
					nPosButtom1 = pSouiLayoutParamStruct1->posRight.nPos.fSize - x; //layout1->pos[3].nPos = layout1->pos[3].nPos - x;

				}
				else//����1Ϊ����
				{
					nPosButtom = pSouiLayoutParamStruct->posRight.nPos.fSize + x; //layout->pos[3].nPos = layout->pos[3].nPos + x;
					nPosButtom1 = pSouiLayoutParamStruct1->posRight.nPos.fSize + x; //layout1->pos[3].nPos = layout1->pos[3].nPos + x;
				}


				pSouiLayoutParamStruct->posLeft.nPos.fSize = nPosTop;
				pSouiLayoutParamStruct1->posLeft.nPos.fSize = nPosTop1;
				pSouiLayoutParamStruct->posRight.nPos.fSize = nPosButtom;
				pSouiLayoutParamStruct1->posRight.nPos.fSize = nPosButtom1;

			}
			/************************* �ƶ� buttom **************************************/



		}


	}
	void SMoveWnd::MoveWndVert(int x)
	{
		//��ֱ�ƶ�
		if (m_pRealWnd == m_Desiner->m_pRealWndRoot)
		{
			return;
		}

		SouiLayoutParam *pSouiLayoutParam = GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pSouiLayoutParam->GetRawData();

		SouiLayoutParam *pSouiLayoutParam1 = m_pRealWnd->GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct1 = (SouiLayoutParamStruct*)pSouiLayoutParam1->GetRawData();

		//��margin�����
		SwndStyle &style = m_pRealWnd->GetParent()->GetStyle();
		int nMargin = 0;

		CRect rcMargin = style.GetMargin();
		nMargin = rcMargin.top;

		//�����϶���top����С��0
		if (pSouiLayoutParamStruct->posTop.nPos.fSize + x - nMargin < 0 && x < 0)
		{
			return;
		}

		//�����϶���bottom���ܴ��ڸ��ؼ���bottom
		if (x > 0)
		{
			CRect r;
			GetParent()->GetWindowRect(r);

			if (pSouiLayoutParamStruct->posTop.nPos.fSize + pSouiLayoutParam->GetSpecifiedSize(Vert).fSize + x + rcMargin.bottom > r.bottom - r.top)
			{
				return;
			}
		}

		if (pSouiLayoutParamStruct1->nCount == 2) //������������
		{
			if (pSouiLayoutParamStruct1->posTop.cMinus == -1)// ����1Ϊ���������
			{
				if (pSouiLayoutParamStruct1->posTop.nPos.fSize - x < 0)
				{
					return;
				}

				pSouiLayoutParamStruct->posTop.nPos.fSize = pSouiLayoutParamStruct->posTop.nPos.fSize + x;
				pSouiLayoutParamStruct1->posTop.nPos.fSize = pSouiLayoutParamStruct1->posTop.nPos.fSize - x;

			}
			else//����1Ϊ����
			{
				if (pSouiLayoutParamStruct1->posBottom.cMinus == -1)
				{
					if (pSouiLayoutParamStruct1->posBottom.nPos.fSize - x < 0)
					{
						return;
					}
				}

				pSouiLayoutParamStruct->posTop.nPos.fSize = pSouiLayoutParamStruct->posTop.nPos.fSize + x;
				pSouiLayoutParamStruct1->posTop.nPos.fSize = pSouiLayoutParamStruct1->posTop.nPos.fSize + x;
			}
		}
		else if (pSouiLayoutParamStruct1->nCount == 4)
		{
			int nPosTop, nPosTop1;
			int nPosButtom, nPosButtom1;
			/************************* �ƶ�top�� **************************************/

			if (pSouiLayoutParamStruct1->posTop.cMinus == -1)// ����1Ϊ���������
			{
				if (pSouiLayoutParamStruct1->posTop.nPos.fSize - x < 0)
				{
					return;
				}

				nPosTop = pSouiLayoutParamStruct->posTop.nPos.fSize + x; //layout->pos[1].nPos = layout->pos[1].nPos + x;   
				nPosTop1 = pSouiLayoutParamStruct1->posTop.nPos.fSize - x; //layout1->pos[1].nPos = layout1->pos[1].nPos - x;
			}
			else//����1Ϊ����
			{
				if (pSouiLayoutParamStruct1->posBottom.cMinus == -1)
				{
					if (pSouiLayoutParamStruct1->posBottom.nPos.fSize - x < 0)
					{
						return;
					}
				}

				nPosTop = pSouiLayoutParamStruct->posTop.nPos.fSize + x;    //layout->pos[1].nPos = layout->pos[1].nPos + x;
				nPosTop1 = pSouiLayoutParamStruct1->posTop.nPos.fSize + x;  //layout1->pos[1].nPos = layout1->pos[1].nPos + x;
			}

			/************************* �ƶ� top **************************************/

			/************************* �ƶ� buttom  **************************************/
			if (pSouiLayoutParamStruct1->posBottom.pit == PIT_SIZE)  //100, 100 ,@5, @5�������
			{
				//
				pSouiLayoutParamStruct->posTop.nPos.fSize = nPosTop;
				pSouiLayoutParamStruct1->posTop.nPos.fSize = nPosTop1;
			}
			else
			{
				if (pSouiLayoutParamStruct1->posBottom.cMinus == -1)// ����1Ϊ���������
				{
					if (pSouiLayoutParamStruct1->posBottom.nPos.fSize - x < 0)
					{
						return;
					}

					nPosButtom = pSouiLayoutParamStruct->posBottom.nPos.fSize + x;  //layout->pos[3].nPos = layout->pos[3].nPos + x;
					nPosButtom1 = pSouiLayoutParamStruct1->posBottom.nPos.fSize - x; //layout1->pos[3].nPos = layout1->pos[3].nPos - x;
				}
				else//����1Ϊ����
				{
					nPosButtom = pSouiLayoutParamStruct->posBottom.nPos.fSize + x; //layout->pos[3].nPos = layout->pos[3].nPos + x;
					nPosButtom1 = pSouiLayoutParamStruct1->posBottom.nPos.fSize + x; //layout1->pos[3].nPos = layout1->pos[3].nPos + x;
				}

				pSouiLayoutParamStruct->posTop.nPos.fSize = nPosTop;
				pSouiLayoutParamStruct1->posTop.nPos.fSize = nPosTop1;
				pSouiLayoutParamStruct->posBottom.nPos.fSize = nPosButtom;
				pSouiLayoutParamStruct1->posBottom.nPos.fSize = nPosButtom1;
			}
			/************************* �ƶ� buttom **************************************/
		}
	}

	void SMoveWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_bCtrlShift)
		{
			m_Desiner->ShowMovWndChild(TRUE, (SMoveWnd*)this->GetWindow(GSW_FIRSTCHILD));
			m_bCtrlShift = FALSE;
		}
	}

	void SMoveWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (m_pRealWnd == m_Desiner->m_pRealWndRoot)
		{
			m_bMsgHandled = FALSE;
			return;
		}

		//�������·����        ѡ��ǰ����һ���ؼ�
		//esc              ѡ�и��ؼ�
		//ctrl  + �����   ���������ƶ�1�����λ��
		//Shift + �����   �����ؼ��ĳߴ�
		//delete           ɾ����ǰ�ؼ�
		//CTRL + SHIFT ���ؼ����ӿؼ���ס�ˣ���ʱ�����޷��ƶ��ؼ��ģ���Ϊ������ôѡ��ֻ��ѡ���ӿؼ�����ʱ��סctrl+ shift���������ӿؼ���ʹ�ؼ�����ѡ�дӶ��ƶ�

		m_bMsgHandled = FALSE;

		BOOL bShift = (::GetKeyState(VK_SHIFT) < 0);
		BOOL bCtrl = (::GetKeyState(VK_CONTROL) < 0);

		if (bShift&&bCtrl)
		{
			m_Desiner->ShowMovWndChild(FALSE, (SMoveWnd*)this->GetWindow(GSW_FIRSTCHILD));
			m_bCtrlShift = TRUE;
			return;
		}
		else
		{
			if (m_bCtrlShift)
			{
				m_Desiner->ShowMovWndChild(TRUE, (SMoveWnd*)this->GetWindow(GSW_FIRSTCHILD));
				m_bCtrlShift = FALSE;
			}
		}

		switch (nChar)
		{
		case VK_UP:
			if (bCtrl)
			{

				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					return; //���Բ��ֲ����ƶ�
				}
				MoveWndVert(-1);
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����

			}
			else if (bShift)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					MoveWndSize_Linear(-1, Vert);
				}
				else
				{
					MoveWndSize(-1, VERT);
				}
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����

			}
			else
			{
				SMap<SWindow*, SMoveWnd*>::CPair *p = m_Desiner->m_mapMoveRealWnd.Lookup(m_pRealWnd->GetWindow(GSW_PREVSIBLING));
				if (p)
				{
					SMoveWnd *pMovWnd = p->m_value;
					pMovWnd->Click(0, CPoint(0, 0));
				}

			}
			break;

		case VK_LEFT:
			if (bCtrl)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					return;
				}
				else
				{
					MoveWndHorz(-1);
				}
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����
			}
			else if (bShift)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					MoveWndSize_Linear(-1, Horz);
				}
				else
				{
					MoveWndSize(-1, HORZ);
				}
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����
			}
			else
			{
				SMap<SWindow*, SMoveWnd*>::CPair *p = m_Desiner->m_mapMoveRealWnd.Lookup(m_pRealWnd->GetWindow(GSW_PREVSIBLING));
				if (p)
				{
					SMoveWnd *pMovWnd = p->m_value;
					pMovWnd->Click(0, CPoint(0, 0));
				}
			}
			break;

		case VK_DOWN:
			if (bCtrl)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					return;
				}
				else
				{
					MoveWndVert(1);
				}
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����
			}
			else if (bShift)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					MoveWndSize_Linear(1, Vert);
				}
				else
				{
					MoveWndSize(1, VERT);
				}
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����
			}
			else
			{
				SMap<SWindow*, SMoveWnd*>::CPair *p = m_Desiner->m_mapMoveRealWnd.Lookup(m_pRealWnd->GetWindow(GSW_NEXTSIBLING));
				if (p)
				{
					SMoveWnd *pMovWnd = p->m_value;
					pMovWnd->Click(0, CPoint(0, 0));
				}
			}
			break;

		case VK_RIGHT:
			if (bCtrl)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					return;
				}
				MoveWndHorz(1);
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����
			}
			else if (bShift)
			{
				if (m_pRealWnd->GetLayoutParam()->IsClass(SLinearLayoutParam::GetClassName()))
				{
					MoveWndSize_Linear(1, Horz);
				}
				else
				{
					MoveWndSize(1, HORZ);
				}
				m_bMsgHandled = TRUE;

				m_pRealWnd->GetParent()->RequestRelayout();
				m_pRealWnd->GetParent()->UpdateChildrenPosition();

				GetParent()->RequestRelayout();
				GetParent()->UpdateChildrenPosition();
				m_Desiner->UpdatePosToXmlNode(m_pRealWnd, this);
				m_Desiner->UpdatePropGrid(m_Desiner->m_xmlNode);
				GetParent()->Invalidate(); //ˢ�¸�����
			}
			else
			{
				SMap<SWindow*, SMoveWnd*>::CPair *p = m_Desiner->m_mapMoveRealWnd.Lookup(m_pRealWnd->GetWindow(GSW_NEXTSIBLING));
				if (p)
				{
					SMoveWnd *pMovWnd = p->m_value;
					pMovWnd->Click(0, CPoint(0, 0));
				}
			}
			break;

		case VK_ESCAPE:
		{
			SMap<SWindow*, SMoveWnd*>::CPair *p = m_Desiner->m_mapMoveRealWnd.Lookup(m_pRealWnd->GetWindow(GSW_PARENT));
			if (p)
			{
				SMoveWnd *pMovWnd = p->m_value;
				pMovWnd->Click(0, CPoint(0, 0));
			}
			m_bMsgHandled = TRUE;
		}
		break;

		case VK_DELETE:
		{
			int n = SMessageBox(NULL, _T("ȷ��Ҫɾ����"), _T("��ʾ"), MB_OKCANCEL);
			if (n == IDOK)
			{
				m_Desiner->DeleteCtrl();
			}
			m_bMsgHandled = TRUE;
		}
		break;

		default:
			m_bMsgHandled = FALSE;
		}


	}


	void SMoveWnd::MoveWndSizeLT(int x, int PosN)
	{
		if (m_pRealWnd == m_Desiner->m_pRealWndRoot)
		{
			return;
		}

		SouiLayoutParam *pSouiLayoutParam = GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pSouiLayoutParam->GetRawData();

		SouiLayoutParam *pSouiLayoutParam1 = m_pRealWnd->GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct1 = (SouiLayoutParamStruct*)pSouiLayoutParam1->GetRawData();


		CRect rcReal, rcRealParent;
		m_pRealWnd->GetWindowRect(rcReal);
		m_pRealWnd->GetParent()->GetWindowRect(rcRealParent);

		//��margin�����
		SwndStyle &style = m_pRealWnd->GetParent()->GetStyle();
		int nMargin = 0;

		CRect rcMargin = style.GetMargin();
		if (PosN == HORZ_LT)
		{
			nMargin = rcMargin.left;
		}
		else
		{
			nMargin = rcMargin.top;
		}

		//�������϶���С��left��top����С��0
		/*if (pSouiLayoutParamStruct->pos[PosN].nPos + x - nMargin < 0 && x < 0)*/
		if (GetLayoutSize(pSouiLayoutParamStruct, PosN) + x - nMargin < 0 && x < 0)
		{
			return;
		}

		//�����϶���С,left ���ܴ��� right
		if (pSouiLayoutParam->GetSpecifiedSize(Horz).fSize - x < 1 && PosN == 0 && x > 0)
		{
			return;
		}

		//�����϶���С,top ���ܴ��� buttom
		if (pSouiLayoutParam->GetSpecifiedSize(Vert).fSize - x < 1 && PosN == 1 && x > 0)
		{
			return;
		}

		/*pSouiLayoutParamStruct->pos[PosN].nPos = pSouiLayoutParamStruct->pos[PosN].nPos + x;*/
		SetLayoutSize(pSouiLayoutParamStruct, PosN, GetLayoutSize(pSouiLayoutParamStruct, PosN) + x);
		if (PosN == 0)
		{
			SLayoutSize LayoutSize = pSouiLayoutParam->GetSpecifiedSize(Horz);
			LayoutSize.fSize = LayoutSize.fSize - x;
			pSouiLayoutParam->SetSpecifiedSize(Horz, LayoutSize);
			/*pSouiLayoutParam->SetSpecifiedSize(Horz, pSouiLayoutParam->GetSpecifiedSize(Horz) - x);*/
		}
		else
		{
			/*pSouiLayoutParam->SetSpecifiedSize(Vert, pSouiLayoutParam->GetSpecifiedSize(Vert) - x);*/
			SLayoutSize LayoutSize = pSouiLayoutParam->GetSpecifiedSize(Vert);
			LayoutSize.fSize = LayoutSize.fSize - x;
			pSouiLayoutParam->SetSpecifiedSize(Vert, LayoutSize);
		}

		if (pSouiLayoutParamStruct1->nCount == 2) //ֻ����������㣬�Զ������С
		{
			if (pSouiLayoutParam1->IsSpecifiedSize(Both))//��sizeָ����С��ʱ��
			{

				/*pSouiLayoutParamStruct1->pos[PosN].nPos = pSouiLayoutParamStruct1->pos[PosN].nPos + x;*/
				SetLayoutSize(pSouiLayoutParamStruct1, PosN, GetLayoutSize(pSouiLayoutParamStruct1, PosN) + x);
				if (PosN == 0)
				{   //��
					/*pSouiLayoutParam1->SetSpecifiedSize(Horz, pSouiLayoutParam1->GetSpecifiedSize(Horz) - x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Horz);
					LayoutSize.fSize = LayoutSize.fSize - x;
					pSouiLayoutParam1->SetSpecifiedSize(Horz, LayoutSize);
				}
				else
				{   //��
					/*pSouiLayoutParam1->SetSpecifiedSize(Vert, pSouiLayoutParam1->GetSpecifiedSize(Vert) - x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Vert);
					LayoutSize.fSize = LayoutSize.fSize - x;
					pSouiLayoutParam1->SetSpecifiedSize(Vert, LayoutSize);
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			//-100, -100, @20,@20
			if (GetPosInfo(pSouiLayoutParamStruct1, PosN).cMinus == -1)// ����Ϊ���������
			{

				//pSouiLayoutParamStruct1->pos[PosN].nPos = pSouiLayoutParamStruct1->pos[PosN].nPos - x;
				SetLayoutSize(pSouiLayoutParamStruct1, PosN, GetLayoutSize(pSouiLayoutParamStruct1, PosN) - x);
			}
			else
			{
				/*pSouiLayoutParamStruct1->pos[PosN].nPos = pSouiLayoutParamStruct1->pos[PosN].nPos + x;*/
				SetLayoutSize(pSouiLayoutParamStruct1, PosN, GetLayoutSize(pSouiLayoutParamStruct1, PosN) + x);
			}

			if (GetPosInfo(pSouiLayoutParamStruct1, PosN + 2).pit == PIT_SIZE)  //@5�������
			{

				/*pSouiLayoutParamStruct1->pos[PosN + 2].nPos = pSouiLayoutParamStruct1->pos[PosN + 2].nPos - x;*/
				SetLayoutSize(pSouiLayoutParamStruct1, PosN + 2, GetLayoutSize(pSouiLayoutParamStruct1, PosN + 2) - x);
				if (PosN == 0)
				{
					/*pSouiLayoutParam1->SetSpecifiedSize(Horz, pSouiLayoutParam1->GetSpecifiedSize(Horz) - x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Horz);
					LayoutSize.fSize = LayoutSize.fSize - x;
					pSouiLayoutParam1->SetSpecifiedSize(Horz, LayoutSize);
				}
				else
				{
					/*pSouiLayoutParam1->SetSpecifiedSize(Vert, pSouiLayoutParam1->GetSpecifiedSize(Vert) - x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Vert);
					LayoutSize.fSize = LayoutSize.fSize - x;
					pSouiLayoutParam1->SetSpecifiedSize(Vert, LayoutSize);
				}


			}
			else  //�������
			{
				//80,100,50,80 ���������top < buttom  right < reft����ʱ������ 
				//
				//layout1->pos[PosN].nPos = layout1->pos[PosN].nPos + x;
			}
		}
	}

	void SMoveWnd::MoveWndSize(int x, int PosN)
	{
		//SwndLayout *layout = GetLayout();
		//SwndLayout *layout1 = m_pRealWnd->GetLayout();

		SouiLayoutParam *pSouiLayoutParam = GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pSouiLayoutParam->GetRawData();

		SouiLayoutParam *pSouiLayoutParam1 = m_pRealWnd->GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct1 = (SouiLayoutParamStruct*)pSouiLayoutParam1->GetRawData();

		CRect rcReal, rcRealParent, rcMovParent;
		m_pRealWnd->GetWindowRect(rcReal);
		m_pRealWnd->GetParent()->GetWindowRect(rcRealParent);
		GetParent()->GetWindowRect(rcMovParent);

		//��margin�����
		SwndStyle &style = m_pRealWnd->GetParent()->GetStyle();
		int nMargin = 0;

		CRect rcMargin = style.GetMargin();
		if (PosN == HORZ)
		{
			nMargin = rcMargin.right;
		}
		else
		{
			nMargin = rcMargin.bottom;
		}

		//���϶����ܳ������ؼ����ұ߾�
		if (PosN == 2 && x > 0)
		{
			if (GetLayoutSize(pSouiLayoutParamStruct, PosN - 2) + pSouiLayoutParam->GetSpecifiedSize(Horz).fSize + x + nMargin > rcMovParent.right - rcMovParent.left)
			{
				return;
			}
		}

		//���϶����ܳ������ؼ����±߾�
		if (PosN == 3 && x > 0)
		{
			if (GetLayoutSize(pSouiLayoutParamStruct, PosN - 2) + pSouiLayoutParam->GetSpecifiedSize(Vert).fSize + x + nMargin > rcMovParent.bottom - rcMovParent.top)
			{
				return;
			}
		}

		//��margin�����  
		SwndStyle &style1 = m_pRealWnd->GetStyle();
		int nMarginLeft = 0, nMarginTop = 0, nMarginBottom = 0, nMarginRight = 0;

		rcMargin = style1.GetMargin();

		if (PosN == HORZ)
		{
			nMarginRight = rcMargin.right;
			nMarginLeft = rcMargin.left;
		}
		else
		{
			nMarginBottom = rcMargin.bottom;
			nMarginTop = rcMargin.top;
		}

		//�����϶���С,left ���ܴ��� right
		if (pSouiLayoutParam->GetSpecifiedSize(Horz).fSize + x - (nMarginRight + nMarginLeft) < 1 && PosN == 2 && x < 0)
		{
			return;
		}

		//�����϶���С,top ���ܴ��� buttom
		if (pSouiLayoutParam->GetSpecifiedSize(Vert).fSize + x - ((nMarginBottom + nMarginTop)) < 1 && PosN == 3 && x < 0)
		{
			return;
		}

		/*pSouiLayoutParamStruct->pos[PosN].nPos = pSouiLayoutParamStruct->pos[PosN].nPos + x;*/
		SetLayoutSize(pSouiLayoutParamStruct, PosN, GetLayoutSize(pSouiLayoutParamStruct, PosN) + x);

		if (PosN == 2)
		{
			/*pSouiLayoutParam->SetSpecifiedSize(Horz, pSouiLayoutParam->GetSpecifiedSize(Horz) + x);*/
			SLayoutSize LayoutSize = pSouiLayoutParam->GetSpecifiedSize(Horz);
			LayoutSize.fSize = LayoutSize.fSize + x;
			pSouiLayoutParam->SetSpecifiedSize(Horz, LayoutSize);
		}
		else
		{
			/*pSouiLayoutParam->SetSpecifiedSize(Vert, pSouiLayoutParam->GetSpecifiedSize(Vert) + x);*/
			SLayoutSize LayoutSize = pSouiLayoutParam->GetSpecifiedSize(Vert);
			LayoutSize.fSize = LayoutSize.fSize + x;
			pSouiLayoutParam->SetSpecifiedSize(Vert, LayoutSize);
		}

		if (pSouiLayoutParamStruct1->nCount == 0)
		{
			if (pSouiLayoutParam1->IsSpecifiedSize(Both))//��sizeָ����С��ʱ��
			{
				if (PosN == 2)
				{
					/*pSouiLayoutParam1->SetSpecifiedSize(Horz, pSouiLayoutParam1->GetSpecifiedSize(Horz) + x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Horz);
					LayoutSize.fSize = LayoutSize.fSize + x;
					pSouiLayoutParam1->SetSpecifiedSize(Horz, LayoutSize);
				}
				else
				{
					/*pSouiLayoutParam1->SetSpecifiedSize(Vert, pSouiLayoutParam1->GetSpecifiedSize(Vert) + x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Vert);
					LayoutSize.fSize = LayoutSize.fSize + x;
					pSouiLayoutParam1->SetSpecifiedSize(Vert, LayoutSize);
				}
			}
		}
		else if (pSouiLayoutParamStruct1->nCount == 2) //ֻ����������㣬�Զ������С
		{
			if (pSouiLayoutParam1->IsSpecifiedSize(Both))//��sizeָ����С��ʱ��
			{

				/*pSouiLayoutParamStruct->pos[PosN].nPos = pSouiLayoutParamStruct->pos[PosN].nPos + x;*/
				SetLayoutSize(pSouiLayoutParamStruct, PosN, GetLayoutSize(pSouiLayoutParamStruct, PosN) + x);
				if (PosN == 2)
				{   //��
					/*pSouiLayoutParam1->SetSpecifiedSize(Horz, pSouiLayoutParam1->GetSpecifiedSize(Horz) + x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Horz);
					LayoutSize.fSize = LayoutSize.fSize + x;
					pSouiLayoutParam1->SetSpecifiedSize(Horz, LayoutSize);
				}
				else
				{   //��
					/*pSouiLayoutParam1->SetSpecifiedSize(Vert, pSouiLayoutParam1->GetSpecifiedSize(Vert) + x);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Vert);
					LayoutSize.fSize = LayoutSize.fSize + x;
					pSouiLayoutParam1->SetSpecifiedSize(Vert, LayoutSize);
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			//5,3,-5,-7
			//5,3,@5,@7

			if (GetPosInfo(pSouiLayoutParamStruct1, PosN).cMinus == -1)// ����3Ϊ���������
			{
				/*pSouiLayoutParamStruct1->pos[PosN].nPos = pSouiLayoutParamStruct1->pos[PosN].nPos - x;*/
				SetLayoutSize(pSouiLayoutParamStruct1, PosN, GetLayoutSize(pSouiLayoutParamStruct1, PosN) - x);
			}
			else if (GetPosInfo(pSouiLayoutParamStruct1, PosN).pit == PIT_SIZE)  //@5�������
			{
				//layout->pos[PosN].nPos = layout->pos[PosN].nPos + x;
				/*pSouiLayoutParamStruct1->pos[PosN].nPos = pSouiLayoutParamStruct1->pos[PosN].nPos + x;*/
				SetLayoutSize(pSouiLayoutParamStruct1, PosN, GetLayoutSize(pSouiLayoutParamStruct1, PosN) + x);
				if (PosN == 2)
				{
					/*pSouiLayoutParam1->SetSpecifiedSize(Horz, pSouiLayoutParamStruct1->pos[PosN].nPos);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Horz);
					LayoutSize.fSize = GetLayoutSize(pSouiLayoutParamStruct1, PosN);
					pSouiLayoutParam1->SetSpecifiedSize(Horz, LayoutSize);
				}
				else
				{
					/*pSouiLayoutParam1->SetSpecifiedSize(Vert, pSouiLayoutParamStruct1->pos[PosN].nPos);*/
					SLayoutSize LayoutSize = pSouiLayoutParam1->GetSpecifiedSize(Vert);
					LayoutSize.fSize = GetLayoutSize(pSouiLayoutParamStruct1, PosN);
					pSouiLayoutParam1->SetSpecifiedSize(Vert, LayoutSize);
				}
			}
			else  //�������
			{
				//80,100,50,80 ���������top < buttom  right < reft����ʱ������ 

				//layout->pos[PosN].nPos = layout->pos[PosN].nPos + x;
				/*pSouiLayoutParamStruct1->pos[PosN].nPos = pSouiLayoutParamStruct1->pos[PosN].nPos + x;*/
				SetLayoutSize(pSouiLayoutParamStruct1, PosN, GetLayoutSize(pSouiLayoutParamStruct1, PosN) + x);
			}

		}
	}

	void SMoveWnd::MoveWndSize_Linear(int x, ORIENTATION orientation)
	{
		SouiLayoutParam *pSouiLayoutParam = GetLayoutParamT<SouiLayoutParam>();
		SouiLayoutParamStruct *pSouiLayoutParamStruct = (SouiLayoutParamStruct*)pSouiLayoutParam->GetRawData();

		SLinearLayoutParam *pLinearLayoutParam = m_pRealWnd->GetLayoutParamT<SLinearLayoutParam>();
		SLinearLayoutParamStruct *pSLinearLayoutParamStruct = (SLinearLayoutParamStruct*)pLinearLayoutParam->GetRawData();

		CRect rcReal, rcRealParent, rcMovParent;
		m_pRealWnd->GetWindowRect(rcReal);
		m_pRealWnd->GetParent()->GetWindowRect(rcRealParent);
		GetParent()->GetWindowRect(rcMovParent);


		//�����ϳ����ؼ��߽�
		if (orientation == Horz)  //�����������ұߵı߿�
		{
			if (pLinearLayoutParam->IsMatchParent(Horz))  //ƥ�丸����ʱ���ܸı��С
			{
				return;
			}

			if (pSLinearLayoutParamStruct->width.fSize + x > 0)
			{
				pSLinearLayoutParamStruct->width.fSize = pSLinearLayoutParamStruct->width.fSize + x;
				pSouiLayoutParamStruct->posRight.nPos.fSize = pSouiLayoutParamStruct->posRight.nPos.fSize + x;
			}
		}
		else  ////�����������±ߵı߿�
		{
			if (pLinearLayoutParam->IsMatchParent(Vert))  //ƥ�丸����ʱ���ܸı��С
			{
				return;
			}

			if (pSLinearLayoutParamStruct->height.fSize + x > 0)
			{
				pSLinearLayoutParamStruct->height.fSize = pSLinearLayoutParamStruct->height.fSize + x;
				pSouiLayoutParamStruct->posBottom.nPos.fSize = pSouiLayoutParamStruct->posBottom.nPos.fSize + x;
			}
		}
	}


	void SMoveWnd::NewWnd(CPoint pt)
	{
		m_Desiner->NewWnd(pt, this);
	}

	void SMoveWnd::Click(UINT nFlags, CPoint pt)
	{
		OnLButtonDown(nFlags, pt);
		OnLButtonUp(nFlags, pt);
	}

	float SMoveWnd::GetLayoutSize(SouiLayoutParamStruct *pSouiLayoutParam, int PosN)
	{
		switch (PosN)
		{
		case 0:
			return pSouiLayoutParam->posLeft.nPos.fSize;
		case 1:
			return pSouiLayoutParam->posTop.nPos.fSize;
		case 2:
			return pSouiLayoutParam->posRight.nPos.fSize;
		case 3:
			return pSouiLayoutParam->posBottom.nPos.fSize;
		default:
			return 0.f;
		}
	}

	void SMoveWnd::SetLayoutSize(SouiLayoutParamStruct *pSouiLayoutParam, int PosN, float value)
	{
		switch (PosN)
		{
		case 0:
			pSouiLayoutParam->posLeft.nPos.fSize = value;
			break;
		case 1:
			pSouiLayoutParam->posTop.nPos.fSize = value;
			break;
		case 2:
			pSouiLayoutParam->posRight.nPos.fSize = value;
			break;
		case 3:
		default:
			pSouiLayoutParam->posBottom.nPos.fSize = value;
			break;
		}
	}


	POS_INFO SMoveWnd::GetPosInfo(SouiLayoutParamStruct *pSouiLayoutParam, int PosN)
	{
		switch (PosN)
		{
		case 0:
			return pSouiLayoutParam->posLeft;
		case 1:
			return pSouiLayoutParam->posTop;
		case 2:
			return pSouiLayoutParam->posRight;
		case 3:
		default:
			return pSouiLayoutParam->posBottom;
		}
	}

}//namesplace soui