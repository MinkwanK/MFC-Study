
// BmpReadingView.cpp: CBmpReadingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "BmpReading.h"
#endif

#include "BmpReadingDoc.h"
#include "BmpReadingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBmpReadingView

IMPLEMENT_DYNCREATE(CBmpReadingView, CView)

BEGIN_MESSAGE_MAP(CBmpReadingView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBmpReadingView 생성/소멸

CBmpReadingView::CBmpReadingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbReserved = 0;
	}

}

CBmpReadingView::~CBmpReadingView()
{
	if (BmInfo)delete BmInfo;
}

BOOL CBmpReadingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CBmpReadingView 그리기

void CBmpReadingView::OnDraw(CDC* pDC)
{
#define WIDTHBYTES(bits) (((bits)+31)/32*4); //이미지 가로 바이트 길이는 4바이트의 배수 (?)
	int height;
	int width;
	int rwsize;

	CBmpReadingDoc* pDoc = GetDocument();
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
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);
		BmInfo->bmiHeader = pDoc->dibHi;
		SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
	}


}

void CBmpReadingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBmpReadingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBmpReadingView 진단

#ifdef _DEBUG
void CBmpReadingView::AssertValid() const
{
	CView::AssertValid();
}

void CBmpReadingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBmpReadingDoc* CBmpReadingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBmpReadingDoc)));
	return (CBmpReadingDoc*)m_pDocument;
}
#endif //_DEBUG


// CBmpReadingView 메시지 처리기
