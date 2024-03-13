
// BmpReading2View.cpp: CBmpReading2View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "BmpReading2.h"
#endif

#include "BmpReading2Doc.h"
#include "BmpReading2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBmpReading2View

IMPLEMENT_DYNCREATE(CBmpReading2View, CView)

BEGIN_MESSAGE_MAP(CBmpReading2View, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SAVE_BMP, &CBmpReading2View::OnSaveBmp)
END_MESSAGE_MAP()

// CBmpReading2View 생성/소멸

CBmpReading2View::CBmpReading2View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbReserved = 0;
	}

}

CBmpReading2View::~CBmpReading2View()
{
	if (BmInfo)delete BmInfo;
}

BOOL CBmpReading2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CBmpReading2View 그리기

void CBmpReading2View::OnDraw(CDC* pDC)
{
#define WIDTHBYTES(bits) (((bits)+31)/32*4); //이미지 가로 바이트 길이는 4바이트의 배수 (?)

	
	/*
	비트맵 이미지의 픽셀 데이터는 바이트 단위로 저장된다. 각 행은 4바이트 길이의 배수로 정렬되어야 한다.
	(가로 비트 수)+4바이트 길이의-1을 하면 무조건 1 이상은 나온다.
	여기에 / 4바이트 비트 길이를 하고 * 4바이트 길이를 하면 4바이트 길이의 배수로 정렬이 된다.
	
	*/
	int height;
	int width;
	int rwsize;
	
	CBmpReading2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_InImg == NULL)
		return;

	if (pDoc->dibHi.biBitCount == 24)
	{
		height = pDoc->dibHi.biHeight;
		width = pDoc->dibHi.biWidth;

		//이게 무슨말이지?
		//diBitCount 는 픽셀당 비트수
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);
		BmInfo->bmiHeader = pDoc->dibHi;
		//비트맵 이미지를 디바이스 컨텍스트에 그리는 데 사용하는 API.
		//그릴 대상의 핸들, 시작할 위치 x,y,그려질 이미지의 폭과 높이, 원본 이미지에서 그릴 부분의 시작 위치를 지정하는 x,y좌표,
		//SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
		//출력할 디바이스 핸들, 시작 좌표 X,Y, W,H,비트맵 이미지에서 출력을 시작할 위치 x,y, 출력할 비트맵 이미지의 픽셀 데이터, BitmapInfo 구조체 가리키는 포인터, 출력할 이미지의 색상 정보	, 출력 연산 코드
		StretchDIBits(pDC->GetSafeHdc(), 800, 0, 1000, 1000, 0, 0, pDoc->dibHi.biWidth, pDoc->dibHi.biHeight, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	//8비트
	/*
	* 8비트 비트맵에서는 각 픽셀이 1 바이트(8비트)로 표현됨. 각 바이트는 색상 인덱스를 나타냄. (해당 픽셀이 어떤 색상 인덱스를 가리키는지)
	* 24비트 비트맵은 한 픽셀에 각각의 색상 채널 R,G,B 모두 들어감
	*/
	else
	{
		int index;
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);
		//위는 정렬된 가로 행 크기
		TRACE("rwsize: %d ", rwsize);
		for (int i = 0; i < pDoc->dibHi.biHeight; i++)
		{
			for (int j = 0; j < pDoc->dibHi.biWidth; j++)
			{
				index = pDoc->m_InImg[i * rwsize + j];
				BYTE R = pDoc->palRGB[index].rgbRed;
				BYTE G = pDoc->palRGB[index].rgbGreen;
				BYTE B = pDoc->palRGB[index].rgbBlue;
				//pDC->SetPixel(j, i, RGB(R, G, B));
			
				//비트맵 이미지의 픽셀 좌표는 하단에서 상단으로 증가하기 때문에	-1을 해서 좌표 변환
				//RGB는 픽셀 색깔
				//pDC->SetPixel(j, pDoc->dibHi.biHeight - i - 1, RGB(R, G, B));

				//최근접 이웃 보간법으로 크기 늘리기 가능
				for (int y = 0; y < 4; y++) {
					for (int x = 0; x < 4; x++) {
						pDC->SetPixel(j * 4 + x + 800, (pDoc->dibHi.biHeight - i - 1) * 4 + y, RGB(R, G, B));
					}
				}
			}
		}
	
	}

}


void LoadJpgFile()
{
	int ImageWidth = 640;
	int ImageHeight = 480;

	int decodedBufferSize = ImageWidth * ImageHeight * 3;
	void* DecodedBuffer = malloc(decodedBufferSize);


}
// CBmpReading2View 진단

#ifdef _DEBUG
void CBmpReading2View::AssertValid() const
{
	CView::AssertValid();
}

void CBmpReading2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBmpReading2Doc* CBmpReading2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBmpReading2Doc)));
	return (CBmpReading2Doc*)m_pDocument;
}
#endif //_DEBUG


// CBmpReading2View 메시지 처리기


void CBmpReading2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	dc.MoveTo(point.x, point.y);
	m_p.x = point.x;
	m_p.y = point.y;

}

void CBmpReading2View::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	if ((nFlags && MK_LBUTTON) == MK_LBUTTON)
	{
		dc.MoveTo(m_p.x, m_p.y);
		dc.LineTo(point.x, point.y);
		m_p.x = point.x;
		m_p.y = point.y;
	}
}


//save 하고 싶은데 잘 안됨 ㅜㅜ

//현재 화면 Rect 대로 캡쳐
//bmp파일로 저장
//CFile에 write하기
void CBmpReading2View::OnSaveBmp()
{
	//HDC h_screen_dc = ::GetDC(NULL);
	//CRect rect;
	//GetClientRect(&rect);
	//int width = rect.Width();
	//int height = rect.Height();

	//BITMAPINFO dib_define;
	//dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//dib_define.bmiHeader.biWidth = width;
	//dib_define.bmiHeader.biHeight = height;
	//dib_define.bmiHeader.biPlanes = 1;
	//dib_define.bmiHeader.biBitCount = 24;
	//dib_define.bmiHeader.biCompression = BI_RGB;
	//dib_define.bmiHeader.biSizeImage = (((width * 24 + 31) & ~31) >> 3) * height;
	//dib_define.bmiHeader.biXPelsPerMeter = 0;
	//dib_define.bmiHeader.biYPelsPerMeter = 0;
	//dib_define.bmiHeader.biClrImportant = 0;
	//dib_define.bmiHeader.biClrUsed = 0;


	////DIB의 내부 이미지 비트 패턴을 참조할 포인터 변수
	//BYTE* p_image_data = NULL;

	////DIB 생성
	//HBITMAP h_bitmap = ::CreateDIBSection(h_screen_dc, &dib_define, DIB_RGB_COLORS, (void**)&p_image_data, 0, 0);

	////가상 DC 생성 
	//HDC h_memory_dc = ::CreateCompatibleDC(h_screen_dc);

	////가상 DC에 이미지를 추출할 비트맵 연결
	//HBITMAP h_old_bitmap = (HBITMAP)::SelectObject(h_memory_dc, h_bitmap);

	////현재 스크린 화면 캡쳐
	//::BitBlt(h_memory_dc, 0, 0, width, height, h_screen_dc, 0, 0, SRCCOPY);

	////본래의 비트맵으로 복구
	//::SelectObject(h_memory_dc, h_old_bitmap);

	////가상 DC 제거
	//DeleteDC(h_memory_dc);

	////DIB 파일의 헤더 내용 구성
	//BITMAPFILEHEADER dib_format_layout;
	//ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	//dib_format_layout.bfType = *(WORD*)"BM";
	//dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage; dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//// DIB 파일을 생성한다.
	//FILE *p_file = fopen("image.bmp", "wb");if(p_file != NULL){    fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);    fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);    fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);    fclose(p_file);}

	//// 사용했던 비트맵과 DC 를 해제한다.if(NULL != h_bitmap) DeleteObject(h_bitmap);if(NULL != h_screen_dc) ::ReleaseDC(NULL, h_screen_dc);

	//// 캡쳐를 하기 위해서 감춘 화면을 다시 보여준다.ShowWindow(SW_SHOW);

	
}
