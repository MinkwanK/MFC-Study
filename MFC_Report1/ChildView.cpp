
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MFC_Report1.h"
#include "ChildView.h"
#include <afxwin.h>
#include <afxdisp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	canDraw = true;
	drawMode = false;
	shape = 'r';
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(101,OnButtonClicked)
	ON_BN_CLICKED(105,OnCheckClicked)
	ON_COMMAND(ID_COLOR_RED, &CChildView::OnColorRed)
	ON_COMMAND(ID_COLOR_GREEN, &CChildView::OnColorGreen)
	ON_COMMAND(ID_COLOR_BLUE, &CChildView::OnColorBlue)
	ON_COMMAND(ID_SHAPE_CIRCLE, &CChildView::OnShapeCircle)
	ON_COMMAND(ID_SHAPE_SQUARE, &CChildView::OnShapeSquare)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COLOR_BLACK, &CChildView::OnColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CChildView::OnUpdateColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CChildView::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CChildView::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CChildView::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_CIRCLE, &CChildView::OnUpdateShapeCircle)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_SQUARE, &CChildView::OnUpdateShapeSquare)
	ON_COMMAND(ID_SAVE_BMP, &CChildView::OnSaveBmp)
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

	//푸시 버튼 생성
	m_pushButton.Create(_T("이미지 불러오기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(500, 0, 1000, 100), this, 101);
	m_radio1.Create(_T("BMP 파일"), WS_CHILD | WS_VISIBLE | WS_GROUP|BS_AUTORADIOBUTTON, CRect(500, 150, 700, 200), this, 102);
	m_radio2.Create(_T("JPG 파일"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(800, 150, 1000, 200), this, 103);
	m_group1.Create(_T("그룹 박스"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(500, 100, 1000, 250), this, 104);
	m_check1.Create(_T("그리기 체크박스"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(1100, 100, 1300, 120), this, 105);
	m_radio1.SetCheck(1);
	m_check1.SetCheck(1);

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


void CChildView::OnCheckClicked()
{
	int checkState = m_check1.GetCheck();

	if (checkState)
		canDraw = TRUE;
	else
		canDraw = FALSE;
}



void CChildView::OnButtonClicked()
{
	
	CDC* pDC = GetDC();
	CString path;
	CFileDialog dlg(TRUE); //true->불러오기 대화상자, false->저장 대화 상자
	CRect rect;
	GetClientRect(&rect);
	Invalidate();
	//EnableWindow(FALSE); // 대화 상자의 유저 인터랙션 비활성화
	//DoModal() 함수를 호출하여 대화 상자를 표시한다. 사용자가 선택한 작업에 대한 결과를 반환
	//확인 버튼이 IDOK
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName(); //선택한 파일 경로 path에 저장
	
	}

	else { return; } //취소 선택한 경우 함수 종료

	//CImage는 이미지 처리 클래스
	//Load() 함수는 이미지를 메모리로 로드
	CImage image;
	HRESULT result = image.Load(path); //bmp 파일 로드

	if (FAILED(result))
	{
		MessageBox(_T("BMP 파일을 열 수 없습니다."), NULL, MB_OK | MB_ICONERROR);
		return;
	}
	int state_radio1 = m_radio1.GetCheck();
	int state_radio2 = m_radio2.GetCheck();

	if (state_radio1 == 1)
	{
		// 화면에 BMP 파일 그리기
		CBitmap bitmap;
		//Deatach() CImage 객체에서 이미지르 분리하고 제어권 상실
		//Attatch() 분리된 이미지를 CBitmap 객체에 첨부
		bitmap.Attach(image.Detach());
		BITMAP bmpinfo;

		//비트맵을 화면에 출력하려면 비트맵의 가로와 세로 크기를 알아야함.
		bitmap.GetBitmap(&bmpinfo);

		CDC dcmem;
		//메모리 디바이스 컨텍스트(메모리 일부를 화면처럼 다룰 수 있게 운영체제에서 제공하는 개념) 생성
		dcmem.CreateCompatibleDC(pDC);
		//비트맵을 메모리 디바이스 컨텍스트에 선택
		//메모리 DC에 비트맵 출력
		dcmem.SelectObject(&bitmap);
		//함수로 화면에 출력

	

		//한 디바이스 컨텍스트에서 다른 디바이스 컨텍스트로 비트맵 데이터 전송
		//SRCOOPY는 원본의 모든 픽셀을 복사하는 코드
		//마지막 인자는 픽셀값 조합 방식
	
		pDC->BitBlt(0, 200, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, SRCCOPY);
		ReleaseDC(&dcmem);
	
	}
	//jpg
	else
	{
		CImage image;
		HRESULT hResult = image.Load(path);

		if (hResult != S_OK)
		{
			AfxMessageBox(_T("이미지를 불러올 수 없습니다."));
		}
		
		image.Draw(pDC->m_hDC, 0, rect.CenterPoint().y - image.GetHeight() / 2);
	}




	ReleaseDC(pDC);





}


void CChildView::OnColorRed()
{
	color = RGB(255, 0, 0);
}


void CChildView::OnColorGreen()
{
	color = RGB(0, 255, 0);
}


void CChildView::OnColorBlue()
{
	color = RGB(0, 0, 255);
}


void CChildView::OnShapeCircle()
{
	shape = 'c';
}


void CChildView::OnShapeSquare()
{
	shape = 'r';
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (canDraw) {
		drawMode = TRUE;
		x1 = x2 = point.x;
		y1 = y2 = point.y;
	}
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (canDraw) {
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
	}
	
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (drawMode  && canDraw)
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
		else if (shape == 'c')
		{
			dc.SetROP2(R2_NOT);
			dc.Ellipse(x1, y1, x2, y2);
			dc.SetROP2(R2_NOT);
			x2 = point.x;
			y2 = point.y;
			dc.Ellipse(x1, y1, x2, y2);
		}

	}

}


void CChildView::OnColorBlack()
{
	color = RGB(0, 0, 0);
}


void CChildView::OnUpdateColorBlack(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(0, 0, 0));
}


void CChildView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(0, 0, 255));
}


void CChildView::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(0, 255, 0));
}


void CChildView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(color == RGB(255, 0, 0));
}


void CChildView::OnUpdateShapeCircle(CCmdUI* pCmdUI)
{
		pCmdUI->SetCheck(shape == 'c');
}


void CChildView::OnUpdateShapeSquare(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(shape == 'r');
}




void CChildView::OnSaveBmp()
{
	
	
	//가상 스크린의 폭과 높이를 얻음. 모니터의 크기가 아니라 모든 연결된 모니터들의 영역을 합친 화면 영역을 얻음
	int iWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int iHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);


	//데스크톱 윈도우의 핸들을 전달. 전체 화면을 대상으로 한 그래픽 작업 수행에 적합
	CClientDC clsScreenDC(GetDesktopWindow());
	CImage clsImage;
	HRESULT hr;



	// 전체 화면 캡처 이미지를 저장할 객체를 생성한다.

	if (clsImage.Create(iWidth, iHeight, 24))
	{
		HDC hDC = clsImage.GetDC();
		if (hDC)
		{

			// 전체 화면을 이미지 객체 DC 에 그린다.
			StretchBlt(hDC, 0, 0, iWidth, iHeight, clsScreenDC, 0, 0, iWidth, iHeight, SRCCOPY);
			clsImage.ReleaseDC();



			// 이미지 파일을 저장한다.

			hr = clsImage.Save(_T("c:\\temp\\screen.jpg"));
			if (FAILED(hr))
			{
				TRACE(_T("Save error\n"));
			}
		}
	}
	



	
}
