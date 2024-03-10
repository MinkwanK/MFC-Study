
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "DrawCircles.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_bDrawMode = FALSE;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
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
	SetCapture();
	m_bDrawMode = TRUE;

	m_x1 = m_x2 = point.x;
	m_y1 = m_y2 = point.y;

	//CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDrawMode)
	{
		//클라이언트 영역에 그리기 작업을 수행하기 위한 장치 컨텍스트
		CClientDC dc(this);
		//타원 윤곽을 그리기 위해 NULL_BRUSH 선택
		//SelectStockObject 함수는 장치 컨텍스트에서 기본 GDI 객체를 선택하는데 사용
		//NULL_BRUSH는 투명한 브러시
		dc.SelectStockObject(NULL_BRUSH);
		//이전에 그린 타원 지우기(색상을 XOR 연산)
		//dc의 래스터 연산 모드 설정. 그리기 작업 중 픽셀 색상 결정하는 방식 제어
		//R2_NOT은 현재 픽셀 색상의 보수를 사용
		dc.SetROP2(R2_NOT);
		dc.Ellipse(m_x1, m_y1, m_x2, m_y2);
		//새로운 타원 그리기
		dc.SetROP2(R2_NOT);
		m_x2 = point.x;
		m_y2 = point.y;
		//타워 그리기
		dc.Ellipse(m_x1, m_y1, m_x2, m_y2);
	}

	//CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	dc.SelectStockObject(NULL_BRUSH);
	//최종적인 타원 그리기
	//새로운 색상을 현재 펜의 색상으로 설정
	dc.SetROP2(R2_COPYPEN);
	m_x2 = point.x;
	m_y2 = point.y;
	dc.Ellipse(m_x1, m_y1, m_x2, m_y2);

	//그리기 모드 끝내기
	m_bDrawMode = FALSE;

	ReleaseCapture();
}
