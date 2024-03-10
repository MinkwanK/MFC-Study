
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFC_Report1.h"
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
	ON_WM_CREATE()
	ON_BN_CLICKED(101,OnButtonClicked)
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

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*
	버튼을 만들어서 중간에 배치하고 싶다. 근데 TRACE를 찍어보니 OnCreate에서 윈도우의 크기가 결정되지 않는 것 같다.
	책이나 인터넷에선 버튼을 보통 OnCreate() 함수 안에서 만든다고 한다.
	*/
	CRect rect;
	GetClientRect(&rect);
	TRACE(_T("x 중심값 %d\n"), rect.CenterPoint().x);
	TRACE(_T("y 중심값 %d\n"), rect.CenterPoint().y);
	m_pushButton.Create(_T("푸시 버튼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0,0,200,200),this,101);

	return 0;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	CRect rect;
	GetClientRect(&rect);

	dc.SetBkColor(RGB(255, 255, 0));
	dc.DrawText(CString("안녕하세요."),&rect,DT_LEFT);
	dc.SetBkColor(RGB(0, 0, 0));
	dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText(CString("반갑습니다."), &rect, DT_RIGHT);
	dc.SetBkColor(RGB(0, 255, 0));
	dc.SetTextColor(RGB(0, 0, 0));
	dc.DrawText(CString("건강하세요"), &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	
	
}





void CChildView::OnButtonClicked()
{
	MessageBox(_T("버튼 클릭!!"));
}
