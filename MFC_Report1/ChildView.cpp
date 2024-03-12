
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
	color = RGB(0, 0, 0);
	
	cFileDialogClicked = false;
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
	ON_COMMAND(ID_SAVE_JPG, &CChildView::OnSaveJpg)
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
	m_pushButton.Create(_T("이미지 불러오기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1200, 0, 1400, 100), this, 101);
	m_radio1.Create(_T("BMP 파일"), WS_CHILD | WS_VISIBLE | WS_GROUP|BS_AUTORADIOBUTTON, CRect(1200, 150, 1300, 200), this, 102);
	m_radio2.Create(_T("JPG 파일"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(1300, 150, 1400, 200), this, 103);
	m_group1.Create(_T("그룹 박스"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(1100, 100, 1500, 250), this, 104);
	//m_check1.Create(_T("그리기 체크박스"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(1200, 250, 1400, 300), this, 105);
	m_radio1.SetCheck(1);
	//m_check1.SetCheck(1);

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
	/*int checkState = m_check1.GetCheck();*/

	//if (checkState)
	//	canDraw = TRUE;
	//else
	//	canDraw = FALSE;
}



void CChildView::OnButtonClicked()
{
	CDC* pDC = GetDC();
	CString path;
	//NULL NULL은 기본 폴더경로 및 파일이름
	CFileDialog dlg(TRUE, NULL,NULL, OFN_OVERWRITEPROMPT, _T("jpg 파일 (*.jpg)|*.jpg|bmp 파일 (*.bmp)|*.bmp|모든 파일(*)|*||")); //true->불러오기 대화상자, false->저장 대화 상자
	CRect rect;
	GetClientRect(&rect);
	Invalidate();
	
	//DoModal() 함수를 호출하여 대화 상자를 표시한다. 사용자가 선택한 작업에 대한 결과를 반환
	//확인 버튼이 IDOK
	if (dlg.DoModal() == IDOK)
	{
		cFileDialogClicked = true;
		path = dlg.GetPathName(); //선택한 파일 경로 path에 저장
		CFile file;
		BITMAPFILEHEADER bmfh;
		DWORD dwFileSize, dwDibSize;
		BYTE* pDib = new BYTE[dwDibSize];
		
		file.Open(path, CFile::modeRead);
		dwFileSize = (DWORD)file.GetLength();
		//전체 사이즈에서 BITMAPFILEHEADER 사이즈를 빼면 DIB 사이즈가 된다.
		dwDibSize = dwFileSize - sizeof(BITMAPFILEHEADER);

		file.Read(&bmfh, sizeof(BITMAPINFOHEADER)); //fileheader 구조체 읽기
		file.Read(pDib, dwDibSize); //DIB 읽기
		file.Close();

		BITMAPINFOHEADER* pBmh = (BITMAPINFOHEADER*)pDib;
		int nWidth = pBmh->biWidth;
		int nHeight = pBmh->biHeight;
		int nBit = pBmh->biBitCount;
		BYTE* lpBits = NULL;

		if (nBit > 8)
		{
			lpBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER);
		}
		else
		{
			//lpBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << nBit);
		}

		//데이터 화면에 표현
		CClientDC dc(this);

		::StretchDIBits(dc.m_hDC, nWidth + 10, 0, nWidth, nHeight, 0, 0, nWidth,nHeight,lpBits, (LPBITMAPINFO)pDib, DIB_RGB_COLORS, SRCCOPY);

	
	}

	else { return; } //취소 선택한 경우 함수 종료

	




	ReleaseDC(pDC);
	




}

//LPCTSTR은 Long Pointer to a constant TSTRing
//유니코드와 멀티바이트 문자열 모두를 처리할 수 있는 포인터 형식
//L: Long Pointer를 의미
//P: Pointer를 의미. 포인터 형식임을 나타냄
//C: Constant를 의미. 포인터가 가리키는 문자열이 상수임을 나타냄
//TSTR: 유니코드와 멀티바이트 문자열을 동일하게 다루는 유니코드 문자셋에 맞는 문자열
//메모리 주소를 포함하고 있는 상수
HBITMAP dibLoadFromFile(LPCTSTR lpFileName)
{
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return NULL;

	BITMAPFILEHEADER bmfh;
	DWORD dwRead;
	ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwRead, NULL);

	DWORD dwInfoLength = bmfh.bfOffBits - sizeof(BITMAPINFOHEADER);
	LPBITMAPINFO lpbmi = (LPBITMAPINFO)new BYTE[dwInfoLength];
	ReadFile(hFile, lpbmi, dwInfoLength, &dwRead, NULL);

	HDC hDC = GetDC(GetDesktopWindow());

	LPVOID lpDIBits;
	HBITMAP hDIB = CreateDIBSection(hDC, lpbmi, DIB_RGB_COLORS, &lpDIBits, NULL, 0);
	ReleaseDC(GetDesktopWindow(), hDC);

	ReadFile(hFile, lpDIBits, lpbmi->bmiHeader.biSizeImage, &dwRead, NULL);
	CloseHandle(hFile);

	delete[] lpbmi;

	return hDIB;


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
	if (!cFileDialogClicked)
	{
		CRect rect;
		m_pushButton.GetWindowRect(&rect);
		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


		TRACE(_T("클릭"));

		drawMode = TRUE;
		x1 = x2 = point.x;
		y1 = y2 = point.y;
	}
		
	
	
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (drawMode && !cFileDialogClicked)
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


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!cFileDialogClicked) 
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
	}
	
	else
	{
		cFileDialogClicked = false;
		x1 = 0, x2 = 0, y1 = 0, y2 = 0;
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
	CDC* pDC = GetDC();
	CString path;
	CFileDialog dlg(FALSE,NULL,NULL); //true->불러오기 대화상자, false->저장 대화 상자



	//DoModal() 함수를 호출하여 대화 상자를 표시한다. 사용자가 선택한 작업에 대한 결과를 반환
	//확인 버튼이 IDOK
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName(); //선택한 파일 경로 path에 저장

	}

	else { return; } //취소 선택한 경우 함수 종료
	
	//가상 스크린의 폭과 높이를 얻음. 모니터의 크기가 아니라 모든 연결된 모니터들의 영역을 합친 화면 영역을 얻음
	//int iWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	//int iHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	
	

	//데스크톱 윈도우의 핸들을 전달. 전체 화면을 대상으로 한 그래픽 작업 수행에 적합
	CClientDC dc(this);
	CDC memDC;
	CBitmap bmp;
	//현재 클라이언트 크기 구하기
	CRect rect;
	GetClientRect(&rect);
	int iWidth = rect.Width();
	int iHeight = rect.Height();


	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, rect.left, rect.top, SRCCOPY);

	CImage image;
	image.Attach(bmp);
	image.Save(path+".bmp");
	MessageBox(_T("저장완료"));
	TRACE(_T("파일 저장 경로값 %s\n"), path);

}


void CChildView::OnSaveJpg()
{
	CDC* pDC = GetDC();
	CString path;
	CFileDialog dlg(FALSE, NULL, NULL); //true->불러오기 대화상자, false->저장 대화 상자



	//DoModal() 함수를 호출하여 대화 상자를 표시한다. 사용자가 선택한 작업에 대한 결과를 반환
	//확인 버튼이 IDOK
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName(); //선택한 파일 경로 path에 저장

	}

	else { return; } //취소 선택한 경우 함수 종료

	//가상 스크린의 폭과 높이를 얻음. 모니터의 크기가 아니라 모든 연결된 모니터들의 영역을 합친 화면 영역을 얻음
	//int iWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	//int iHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);



	//데스크톱 윈도우의 핸들을 전달. 전체 화면을 대상으로 한 그래픽 작업 수행에 적합
	CClientDC dc(this);
	CDC memDC;
	CBitmap bmp;
	//현재 클라이언트 크기 구하기
	CRect rect;
	GetClientRect(&rect);
	int iWidth = rect.Width();
	int iHeight = rect.Height();

	
	//생성할 비트맵과 호환되는 장치 컨텍스트, 비트맵의 너비, 비트맵의 높이
	//호환 비트맵의 핸들을 반환한다. 이 핸들을 사용하여 비트맵 조작 및 다른 DC에 비트맵 복삭 ㅏ가능하다.
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	//메모리 디바이스 컨텍스트 생성
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, rect.left, rect.top, SRCCOPY);

	CImage image;
	image.Attach(bmp);
	image.Save(path + ".jpg");
	MessageBox(_T("저장완료"));
	TRACE(_T("파일 저장 경로값 %s\n"), path);
}
