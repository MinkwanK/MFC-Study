
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFC_Report1_2.h"
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
	/*
	디바이스 컨텍스트(Device Context 또는 DC)는 Windows OS에서 그래픽 출력에 사용되는 추상화된 개념.
	DC는 그림을 그리거나 텍스트를 출력하고, 그래픽 객체를 조작하며, 화면에 비트맵을 그리는 등의 그래픽 작업을 할 수 있는 인터페이스 제공

	DC는 특정한 디바이스에 대한 출력 처리 (화면, 프린터, 글카)

	DC는 GDI가 생성하고 관리하는 데이터 구조체, 출력과 관련된 다양한 정보를 담고 있음.

	1. 응용 프로그램이 OS에 DC 요청
	2. 요청 받은 OS의 GDI가 내부적으로 DC를 만든 후, DC를 가리키는 핸들을 응용 프로그램에 전달
	3. 응용 프로그램은 DC의 속성 변경 가능. DC 핸들을 윈도우 API 함수에 전달하여 출력 요청.(->장치 독립적) 요청은 다시 OS GDI에 전달.
	4. GDI가 DC에 포함된 정보를 토대로 다양한 상황을 고려하여 출력. (->장치 의존적). 장치별 출력을 위해 장치 드라이버 사용




	TextOut() 함수는 문자열을 출력할 (x,y) 좌표를 사용함.
	DrawText() 함수는 문자열을 감싸는 직사각형의 좌표(RECT 구조체) 사용
	*/
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	//SIZE에서 파생한 클래스, C가 클래스라는 의미?
	CRect rect;

	//클라이언트 영역의 크기를 가져와서 rect에 저장. 클라이언트 영역이란 윈도우 내에서 작업을 수행하는 영역
	GetClientRect(&rect);
	dc.SetBkColor(RGB(255, 255, 0));
	//dc.TextOutW(50, 50, CString(_T("안녕하세요")));
	dc.DrawText(CString("DrawText 연습"), &rect, 0);
	dc.SetBkColor(RGB(255, 255, 255));
	dc.DrawText(CString("반갑습니다."), &rect, DT_RIGHT | DT_SINGLELINE);
	dc.SetBkColor(RGB(0, 255, 0));
	dc.DrawText(CString("건강하세요"), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//과제 2
	CImage image;
	//HResult 는 함수 호출의 반환값을 저장한다. 함수 반횐된 값을 통해 성공 및 실패를 알 수 있다.
	HRESULT hResult = image.Load(_T("dog.jpg"));

	if (hResult != S_OK)
	{
		AfxMessageBox(_T("이미지를 불러올 수 없습니다."));
	}
	//매개변수: 출력할 디바이스 컨텍스트. m_hDC는 핸들을 의미함.
	image.Draw(dc.m_hDC, 1200, 0);



}



