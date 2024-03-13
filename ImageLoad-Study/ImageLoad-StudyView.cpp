
// ImageLoad-StudyView.cpp: CImageLoadStudyView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageLoad-Study.h"
#endif

#include "ImageLoad-StudyDoc.h"
#include "ImageLoad-StudyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageLoadStudyView

IMPLEMENT_DYNCREATE(CImageLoadStudyView, CView)

BEGIN_MESSAGE_MAP(CImageLoadStudyView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CImageLoadStudyView 생성/소멸

CImageLoadStudyView::CImageLoadStudyView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbReserved = 0;
	}

}

CImageLoadStudyView::~CImageLoadStudyView()
{
}

BOOL CImageLoadStudyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CImageLoadStudyView 그리기

void CImageLoadStudyView::OnDraw(CDC* pDC)
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

	CImageLoadStudyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_InImg == NULL)
		return;

	if (pDoc->loadMode == 'b')
	{
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
	else if (pDoc->loadMode == 'j')
	{
		//메모리에 스트림 생성
		IStream* pStream = SHCreateMemStream(pDoc->jpgData, pDoc->jpgSize);
		

		Bitmap bmp(pStream);

		// Draw the image onto the device context
		Graphics graphics(pDC->GetSafeHdc());
		graphics.DrawImage(&bmp, 0,0,200,200);


		//Clean up
		pStream->Release();
		
	}

	
	
}


// CImageLoadStudyView 진단

#ifdef _DEBUG
void CImageLoadStudyView::AssertValid() const
{
	CView::AssertValid();
}

void CImageLoadStudyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageLoadStudyDoc* CImageLoadStudyView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageLoadStudyDoc)));
	return (CImageLoadStudyDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageLoadStudyView 메시지 처리기


void CImageLoadStudyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	dc.MoveTo(point.x, point.y);
	m_p.x = point.x;
	m_p.y = point.y;

	CView::OnLButtonDown(nFlags, point);
}


void CImageLoadStudyView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	if ((nFlags && MK_LBUTTON) == MK_LBUTTON) {
		dc.MoveTo(m_p.x, m_p.y);
		dc.LineTo(point.x, point.y);
		m_p.x = point.x;
		m_p.y = point.y;
	}
	


	CView::OnMouseMove(nFlags, point);
}
