
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Report1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIDTHBYTES(bits) (((bits)+31)/32*4); //이미지 가로 바이트 길이는 4바이트의 배수 (?)
#define STRIDE(W) W + (W % 4 != 0 ? 4 - W % 4 : 0);
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	delete[] m_outJpgBuffer;
	delete[] m_paddingAddedBuffer;
	delete[] m_jpegHeaderInfo.pJpegData;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_LOAD_BMP, &CChildView::OnLoadBmp)
	ON_COMMAND(ID_LOAD_JPEG, &CChildView::OnLoadJpeg)
	ON_COMMAND(ID_SAVE_BMP, &CChildView::OnSaveBmp)
	ON_COMMAND(ID_SAVE_JPEG, &CChildView::OnSaveJpeg)
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

	CRect rect;
	GetClientRect(&rect);

	/*
	비트맵 이미지의 픽셀 데이터는 바이트 단위로 저장된다. 각 행은 4바이트 길이의 배수로 정렬되어야 한다.
	(가로 비트 수)+4바이트 길이의-1을 하면 무조건 1 이상은 나온다.
	여기에 / 4바이트 비트 길이를 하고 * 4바이트 길이를 하면 4바이트 길이의 배수로 정렬이 된다.

	*/

	if (loadMode == 'b')
	{
		int height;
		int width;
		int rwsize;
		//비트맵 영상 헤더
		BITMAPINFO* BmInfo = new BITMAPINFO();
		if (dibHi.biBitCount == 24)
		{
			height = dibHi.biHeight;
			width = dibHi.biWidth;
			BmInfo->bmiHeader = dibHi;
			int rwsize = WIDTHBYTES(dibHi.biBitCount * dibHi.biWidth);
			StretchDIBits(dc.GetSafeHdc(), 800, 0, 1000, 1000, 0, 0, dibHi.biWidth, dibHi.biHeight, m_InImg, BmInfo, DIB_RGB_COLORS, SRCCOPY);

		}
		else
		{
			int index;
			
			rwsize = WIDTHBYTES(dibHi.biBitCount * dibHi.biWidth);
			//위는 정렬된 가로 행 크기
			TRACE("rwsize: %d ", rwsize);
			for (int i = 0; i < dibHi.biHeight; i++)
			{
				for (int j = 0; j < dibHi.biWidth; j++)
				{
					index = m_InImg[i * rwsize + j];
					BYTE R = palRGB[index].rgbRed;
					BYTE G = palRGB[index].rgbGreen;
					BYTE B = palRGB[index].rgbBlue;
					//pDC->SetPixel(j, i, RGB(R, G, B));

					//비트맵 이미지의 픽셀 좌표는 하단에서 상단으로 증가하기 때문에	-1을 해서 좌표 변환
					//RGB는 픽셀 색깔
					//pDC->SetPixel(j, pDoc->dibHi.biHeight - i - 1, RGB(R, G, B));

					//최근접 이웃 보간법으로 크기 늘리기 가능
					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++) {
							dc.SetPixel(j * 4 + x + 800, (dibHi.biHeight - i - 1) * 4 + y, RGB(R, G, B));
						}
					}
				}
			}

		}
		TRACE(_T("BMP File Paint Complete \n"));

	}
	else if (loadMode == 'j')
	{
		//패딩 구하기
		int paddingAddedWidth = STRIDE(m_jpegHeaderInfo.iWidth);
		int padding = paddingAddedWidth - m_jpegHeaderInfo.iWidth;
		CString paddingMessage;
		paddingMessage.Format(_T("paddingMessage: %d"), paddingAddedWidth);
		AfxMessageBox(paddingMessage);

		//새로운 버퍼에 패딩이 적용된 사이즈 할당
		int paddingAddedSize = m_decodedSize + (padding*3*m_jpegHeaderInfo.iHeight);
		 m_paddingAddedBuffer = new BYTE[paddingAddedSize];
	
		//버퍼 기록 위치를 바꿔줄 바이터 포인터
		int bytePointer = 0;		
		//새로운 패딩 추가 버퍼에 데이터 옮기기
		for (int i = 0; i < m_jpegHeaderInfo.iHeight; i++)
		{
			for (int j = 0; j < m_jpegHeaderInfo.iWidth*3; j++)
			{
				m_paddingAddedBuffer[bytePointer++] = m_outJpgBuffer[i * m_jpegHeaderInfo.iWidth * 3  + j];
				
			}

			for (int k = 0; k < padding*3; k++)
			{
				m_paddingAddedBuffer[bytePointer++] = 0;
			}
		}
	
		m_jpegHeaderInfo.iWidth = paddingAddedWidth;
		BITMAPINFO* bitInfo = NULL;
		bitInfo = new BITMAPINFO;
		bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitInfo->bmiHeader.biPlanes = 1;
		bitInfo->bmiHeader.biBitCount = 3 * 8;
		//이미지 압축 방식, BI_RGB는 비압축방식
		bitInfo->bmiHeader.biCompression = BI_RGB;
		bitInfo->bmiHeader.biWidth = m_jpegHeaderInfo.iWidth;
		bitInfo->bmiHeader.biHeight = -m_jpegHeaderInfo.iHeight;
		
		//비트맵을 실제 크기로 확대 또는 축소하여 그릴 때 각 픽셀을 단순히 복사
		SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);  // set iMode.
		if (StretchDIBits(dc.GetSafeHdc(), 0, 0, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, 0, 0,
			m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight,
			m_paddingAddedBuffer, bitInfo, DIB_RGB_COLORS, SRCCOPY) != GDI_ERROR)
		{
			TRACE(_T("StretchDIBits 성공"));
	
		}		
		TRACE(_T("JPG File Paint Complete \n"));

		delete bitInfo;

		
	}
	
	
	
}



//이미지 파일 불러오기
void CChildView::OnLoadBmp()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT);
	CFile cFile;
	CString path;

	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		cFile.Open(path, CFile::modeRead|CFile::typeBinary);
		int fileSize = cFile.GetLength();
		cFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));
		cFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));

		if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)
		{
			AfxMessageBox(_T("need gray/true color"));
			return;
		}

		
		if (dibHi.biBitCount == 8)
		{
			cFile.Read(palRGB, sizeof(RGBQUAD) * 256);
		}

		int ImgSize;
		if (dibHi.biBitCount == 8)
		{
			ImgSize = cFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
		}
	
		else if (dibHi.biBitCount == 24)
		{
			ImgSize = cFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		}
		m_InImg = new unsigned char[ImgSize];

		cFile.Read(m_InImg, ImgSize);
		cFile.Close();
		loadMode = 'b';
		TRACE(_T("BMP File Load and Read Complete \n"));
		Invalidate();
	}

	
	
}


//Jpeg 불러오기 (디코딩 수행)
void CChildView::OnLoadJpeg()
{
	m_jpegHeaderInfo.iHeight = 0;
	m_jpegHeaderInfo.iJpegSize = 0;
	m_jpegHeaderInfo.iWidth = 0;
	m_jpegHeaderInfo.pJpegData = NULL;
	m_outJpgBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT);
	CString path;
	
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		//m_jpgCodec.GetJpegHeaderInfo(path, m_jpegHeaderInfo);
		CFile cFile;
		cFile.Open(path, CFile::modeRead | CFile::typeBinary);

		m_jpegHeaderInfo.iJpegSize = cFile.GetLength();
		m_jpegHeaderInfo.pJpegData = new BYTE[m_jpegHeaderInfo.iJpegSize];
		cFile.Read(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize);

		//디코딩 수행.
		//디코딩 수행 결과물: Width, Height,디코딩 된 이미지 버퍼
		m_jpgCodec.DecodeImage(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize, m_outJpgBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, TJPF_BGR);
		m_decodedSize = m_jpegHeaderInfo.iWidth * m_jpegHeaderInfo.iHeight * 3;
		loadMode = 'j';
	
		Invalidate();
	}

	
}


void CChildView::OnSaveBmp()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT);
	CFile hFile;

	CString path;

	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
	}

	hFile.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	hFile.Write(&dibHf, sizeof(BITMAPFILEHEADER));
	hFile.Write(&dibHi, sizeof(BITMAPINFOHEADER));

	if (dibHi.biBitCount == 8)
		hFile.Write(palRGB, sizeof(RGBQUAD) * 256);

	hFile.Write(m_InImg, dibHi.biSizeImage);
	hFile.Close();

	
	TRACE(_T("OnSaveDocument"));
	



}


void CChildView::OnSaveJpeg()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT);
	CFile hFile;

	CString path;

	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
	}

	hFile.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	//인코딩 시에 동적할당
	LPBYTE encodedJpgBuffer = NULL;
	//인코딩하고 정해질 파일 사이즈
	m_jpegHeaderInfo.iJpegSize = 0;
	
	if
		(m_jpgCodec.EncodeImage(m_paddingAddedBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, &encodedJpgBuffer,m_jpegHeaderInfo.iJpegSize, 90, TJPF_BGR))
	{
		TRACE(_T("\nEncoding Success\n"));
		TRACE(_T("\n %d \n", outJpegSize));
		hFile.Write(encodedJpgBuffer, m_jpegHeaderInfo.iJpegSize);

		
	}
	else
	{
		TRACE(_T("\nEncoding failed\n"));
	}

	hFile.Close();	
	TRACE(_T("OnSaveDocument"));


}
