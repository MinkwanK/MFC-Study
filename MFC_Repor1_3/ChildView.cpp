
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFC_Repor1_3.h"
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
	ON_BN_CLICKED(103,OnButtonClicked)
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
	CRect rect(5, 50, 500, 500);
	CRect r;
	GetClientRect(r);
	dc.Rectangle(rect);

	int state_radio1 = m_radio1.GetCheck();
	int state_radio2 = m_radio2.GetCheck();

	//bmp file
	if (state_radio1 == 1)
	{
		
		CBitmap bitmap;
		bitmap.LoadBitmapW(IDB_BITMAP1);
		BITMAP bmpinfo;
		bitmap.GetBitmap(&bmpinfo);

		//메모리 디바이스 컨텍스트를 맏는 후 비트맵을 선택해서 넣기
		CDC dcem;
		dcem.CreateCompatibleDC(&dc);
		dcem.SelectObject(&bitmap);

		dc.BitBlt(r.CenterPoint().x, r.CenterPoint().y, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcem, 0, 0, SRCCOPY);
	}
	else
	{
		
		CImage image;
		HRESULT hResult = image.Load(_T("dog.jpg"));

		if (hResult != S_OK)
		{
			AfxMessageBox(_T("이미지를 불러올 수 없습니다."));
		}
		
		image.Draw(dc.m_hDC, r.CenterPoint().x , r.CenterPoint().y);

	}

	CString str;
	str.Format(_T("버튼 상태: %d %d"), state_radio1, state_radio2);
	MessageBox(str, _T("버튼 결과"), MB_ICONINFORMATION);
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_button1.Create(_T("푸시 버튼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(120, 120, 350, 160), this, 103);
	m_radio1.Create(_T("bmp 파일"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(120, 170, 350, 200), this, 104);
	m_radio2.Create(_T("jpg 파일"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(120, 210, 350, 240), this, 105);
	m_group1.Create(_T("그룹 박스"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(100, 100, 400, 250), this, 106);

	m_radio1.SetCheck(1);

	return 0;
}


void CChildView::OnButtonClicked()
{
	Invalidate();
}
