
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Report1_4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	drawMode = false;
	shape = 'r';
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COLOR_GREEN, &CChildView::OnColorGreen)
	ON_COMMAND(ID_COLOR_RED, &CChildView::OnColorRed)
	ON_COMMAND(ID_COLOR_BLUE, &CChildView::OnColorBlue)
	ON_COMMAND(ID_COLOR_BLACK, &CChildView::OnColorBlack)
	ON_COMMAND(ID_SHAPE_RECT, &CChildView::OnShapeRect)
	ON_COMMAND(ID_SHAPE_CIRCLE, &CChildView::OnShapeCircle)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CChildView::OnUpdateColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CChildView::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CChildView::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CChildView::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_CIRCLE, &CChildView::OnUpdateShapeCircle)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_RECT, &CChildView::OnUpdateShapeRect)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SAVE, &CChildView::OnUpdateButtonSave)
	ON_COMMAND(ID_BUTTON_SAVE, &CChildView::OnButtonSave)
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
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	drawMode = TRUE;
	x1 = x2 = point.x;
	y1 = y2 = point.y;

	//CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	CBrush brush;
	brush.CreateSolidBrush(color);

	dc.SelectObject(brush);
	dc.SetROP2(R2_COPYPEN);
	x2 = point.x;
	y2 = point.y;
	if (shape == 'r')
	{
		dc.Rectangle(x1, y1, x2, y2);
	}
	else
	{
		dc.Ellipse(x1, y1, x2, y2);
	}

	drawMode = FALSE;



	//CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (drawMode)
	{
		CClientDC dc(this);
		dc.SelectStockObject(NULL_BRUSH);

		if (shape == 'r')
		{
			dc.SetROP2(R2_NOT);
			dc.Rectangle(x1, y1, x2, y2);
			dc.SetROP2(R2_NOT);
			x2 = point.x;
			y2 = point.y;
			dc.Rectangle(x1, y1, x2, y2);
		}
		else if(shape =='c')
		{
			dc.SetROP2(R2_NOT);
			dc.Ellipse(x1, y1, x2, y2);
			dc.SetROP2(R2_NOT);
			x2 = point.x;
			y2 = point.y;
			dc.Ellipse(x1, y1, x2, y2);
		}

	}

	//CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnColorGreen()
{
	color = RGB(0, 255, 0);
	
}


void CChildView::OnColorRed()
{
	color = RGB(255,0,0);

}


void CChildView::OnColorBlue()
{
	color = RGB(0, 0, 255);
	
}


void CChildView::OnColorBlack()
{
	color = RGB(0,0, 0);
}


void CChildView::OnShapeRect()
{
	shape = 'r';
}


void CChildView::OnShapeCircle()
{
	shape = 'c';
}


void CChildView::OnUpdateColorBlack(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(0, 0, 0));
}


void CChildView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(255, 0, 0));
}


void CChildView::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(0, 255, 0));
}


void CChildView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(0, 0, 255));
}


void CChildView::OnUpdateShapeCircle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(shape == 'c');
}


void CChildView::OnUpdateShapeRect(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(shape == 'r');
}


void CChildView::OnUpdateButtonSave(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void CChildView::OnButtonSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	Gdiplus::ImageFormatJPEG
}
