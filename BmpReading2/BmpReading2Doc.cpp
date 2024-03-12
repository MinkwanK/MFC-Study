
// BmpReading2Doc.cpp: CBmpReading2Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "BmpReading2.h"
#endif

#include "BmpReading2Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBmpReading2Doc

IMPLEMENT_DYNCREATE(CBmpReading2Doc, CDocument)

BEGIN_MESSAGE_MAP(CBmpReading2Doc, CDocument)

END_MESSAGE_MAP()


// CBmpReading2Doc 생성/소멸

CBmpReading2Doc::CBmpReading2Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CBmpReading2Doc::~CBmpReading2Doc()
{
	if (m_InImg)
		delete m_InImg;
	
}

BOOL CBmpReading2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CBmpReading2Doc serialization

void CBmpReading2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

BOOL CBmpReading2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	TRACE(_T("Opening Document"));
	CFile hFile;

	int height;
	int width;

	//파일을 이진형식으로 열겠다. 파일의 데이터가 텍스트든 이진형식이든 상관없이 데이터를 읽고 쓸 수 있다.
	hFile.Open(lpszPathName, CFile::modeRead | CFile::typeBinary); //파일 열기
	//파일을 읽을 때마다 파일 포인터가 움직인다.
	hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));

	if (dibHf.bfType != 0x4D42)
	{
		AfxMessageBox(_T("Not BMP File"));

		return FALSE;
	}

	hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));
	if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)
	{
		AfxMessageBox(_T("need gray/true color"));
		return FALSE;
	}
	if (dibHi.biBitCount == 8)
		hFile.Read(palRGB, sizeof(RGBQUAD) * 256); //2^8승

	//메모리 할당
	int ImgSize;
	if (dibHi.biBitCount == 8)
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
		//이미지의 크기는 파일 총 길이에서 헤드 두개, 팔레트의 사이즈를 제외한다.

		TRACE(_T("Img Size: %d"),ImgSize);
	}
	else if (dibHi.biBitCount == 24)
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	}

	m_InImg = new unsigned char[ImgSize];
	m_OutImg = new unsigned char[ImgSize];
	hFile.Read(m_InImg, ImgSize);
	hFile.Close();

	height = dibHi.biHeight;
	width = dibHi.biWidth;

	return TRUE;

}

BOOL CBmpReading2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFile hFile;
	if (!hFile.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return FALSE;

	TRACE(_T("OnSaveDocument"));
	hFile.Write(&dibHf,sizeof(BITMAPFILEHEADER));
	hFile.Write(&dibHi, sizeof(BITMAPINFOHEADER));
	//비트수가 8이면 팔레트 데이터 쓰기
	if (dibHi.biBitCount == 8)
		hFile.Write(palRGB, sizeof(RGBQUAD) * 256);

	hFile.Write(m_InImg, dibHi.biSizeImage);
	hFile.Close();
	return TRUE;
}



#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CBmpReading2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CBmpReading2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBmpReading2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CBmpReading2Doc 진단

#ifdef _DEBUG
void CBmpReading2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBmpReading2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBmpReading2Doc 명령






