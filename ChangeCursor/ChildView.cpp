
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "ChangeCursor.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	dc.SelectStockObject(LTGRAY_BRUSH);
	dc.Ellipse(10, 10, 400, 100);
}



BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point); //스크린 좌표를 클라이언트 좌표로 변환
		CRgn rgn;

		rgn.CreateEllipticRgn(10, 10, 400, 100);
		if (rgn.PtInRegion(point))
		{
			::SetCursor(AfxGetApp()->LoadCursorW(IDC_CURSOR1));
			
		}
		else
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		}
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
