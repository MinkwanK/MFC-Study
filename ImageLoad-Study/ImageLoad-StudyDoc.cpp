
// ImageLoad-StudyDoc.cpp: CImageLoadStudyDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageLoad-Study.h"
#endif

#include "ImageLoad-StudyDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageLoadStudyDoc

IMPLEMENT_DYNCREATE(CImageLoadStudyDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageLoadStudyDoc, CDocument)
END_MESSAGE_MAP()


// CImageLoadStudyDoc 생성/소멸

CImageLoadStudyDoc::CImageLoadStudyDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageLoadStudyDoc::~CImageLoadStudyDoc()
{
}

BOOL CImageLoadStudyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

BOOL CImageLoadStudyDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	TRACE(_T("Opening Document"));
	CFile hFile;

	int height;
	int width;

	//파일을 이진형식으로 열겠다. 파일의 데이터가 텍스트든 이진형식이든 상관없이 데이터를 읽고 쓸 수 있다.
	hFile.Open(lpszPathName, CFile::modeRead | CFile::typeBinary); //파일 열기

	CString fileExteinsion = GetFileExtension(lpszPathName);
	AfxMessageBox(fileExteinsion);

	if (fileExteinsion == "bmp")
	{
		TRACE(_T("File size : %d", hFile.GetLength()));
		//파일을 읽을 때마다 파일 포인터가 움직인다.
		hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));
		hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));
		if (dibHi.biBitCount != 8 &&  dibHi.biBitCount != 24)
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

			TRACE(_T("Img Size: %d"), ImgSize);
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
		loadMode = 'b';

	}
	else if (fileExteinsion == "jpg")
	{
		loadMode = 'j';
		jpgSize = hFile.GetLength();
		jpgData = new BYTE[jpgSize];
		TRACE(_T("size : %u", jpgSize));
		hFile.Read(jpgData, jpgSize);
		hFile.Close();
	}
	return TRUE;
}


CString CImageLoadStudyDoc::GetFileExtension(const CString& filePath)
{
	int lastIndex = filePath.ReverseFind('.');
	if (lastIndex != -1)
	{
		//다음에 나오는 문자열을 확장자로 반환
		return filePath.Mid(lastIndex + 1);
	}
	else
	{
		//확장자가 없는 경우 빈 문자열 반환
		return CString();
	}
}


// CImageLoadStudyDoc serialization

BOOL CImageLoadStudyDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFile hFile;
	CString fileExtension = GetFileExtension(lpszPathName);
	if (!hFile.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return FALSE;
	if (fileExtension == "bmp")
	{
		AfxMessageBox(_T("bmp"));
		hFile.Write(&dibHf, sizeof(BITMAPFILEHEADER));
		hFile.Write(&dibHi, sizeof(BITMAPINFOHEADER));

		if (dibHi.biBitCount == 8)
			hFile.Write(palRGB, sizeof(RGBQUAD) * 256);
		hFile.Write(m_InImg, dibHi.biSizeImage);
		hFile.Close();

	}
	else if(fileExtension == "jpg")
	{
		hFile.Write(jpgData, jpgSize);
		hFile.Close();
	}

	TRACE(_T("OnSaveDocument: %s",lpszPathName));
	TRACE(_T("OnSaveDocument"));
	

	return TRUE;
}

void CImageLoadStudyDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageLoadStudyDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CImageLoadStudyDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageLoadStudyDoc::SetSearchContent(const CString& value)
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

// CImageLoadStudyDoc 진단

#ifdef _DEBUG
void CImageLoadStudyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageLoadStudyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageLoadStudyDoc 명령
