
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
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	//delete m_outJpgBuffer;
	//delete m_jpegHeaderInfo.pJpegData;
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
		//iColor는 어떻게 가져오지??
		BITMAPINFO* bitInfo = NULL;
		bitInfo = new BITMAPINFO;
		bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitInfo->bmiHeader.biPlanes = 1;
		bitInfo->bmiHeader.biBitCount = 3 * 8;
		//이미지 압축 방식, BI_RGB는 비압축방식
		bitInfo->bmiHeader.biCompression = BI_RGB;
		bitInfo->bmiHeader.biWidth = m_jpegHeaderInfo.iWidth;
		bitInfo->bmiHeader.biHeight = -m_jpegHeaderInfo.iHeight;
	


		//pixel format 을 정하는 이유는 디코딩된 이미지를 어떤 형식으로 처리하고 사용할지
		//if (m_jpgCodec.DecodeImage(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize, m_outJpgBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, TJPF_RGB))
		//{
			LONG rwSize = WIDTHBYTES(m_jpegHeaderInfo.iWidth * 24);
			//디코딩한 이미지의 가로를 4바이트의 배수로 맞춰줘야 한다. 비트맵으로 출력해야하니까.
			m_jpegHeaderInfo.iWidth = rwSize;
			//패딩 비트 추가
			BYTE* paddingOutBuffer = new BYTE[m_jpegHeaderInfo.iJpegSize + rwSize - m_jpegHeaderInfo.iWidth];
			memcpy(paddingOutBuffer, m_outJpgBuffer, m_jpegHeaderInfo.iJpegSize);
			bitInfo->bmiHeader.biWidth = rwSize;
			
			SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);  // set iMode.
			if (StretchDIBits(dc.GetSafeHdc(), 0, 0, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, 0, 0,
				m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight,
				m_outJpgBuffer, bitInfo, DIB_RGB_COLORS, SRCCOPY) != GDI_ERROR)
			{
				TRACE(_T("StretchDIBits 성공"));

				CString message;
				message.Format(_T("m_jPeg Width: %d m_jPeg Height: %d"), m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight);
				AfxMessageBox(message);
				message.Format(_T("bitInfo->bmiHeader.biWidth: %d bitInfo->bmiHeader.biHeight: %d"), bitInfo->bmiHeader.biWidth, bitInfo->bmiHeader.biHeight);


				
			}		
		TRACE(_T("JPG File Paint Complete \n"));

		
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

		m_jpgCodec.DecodeImage(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize, m_outJpgBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, 0);
		TRACE(_T("JPG File Load and Read Complete \n"));
		loadMode = 'j';
		CString message;
		message.Format(_T("Width: %d Height: %d"), m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight);
		AfxMessageBox(message);
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
	LPBYTE m_encodedJpgBuffer = NULL;
	//인코딩하고 정해질 파일 사이즈
	m_jpegHeaderInfo.iJpegSize = 0;
	
	if
		(m_jpgCodec.EncodeImage(m_outJpgBuffer, 590, m_jpegHeaderInfo.iHeight, &m_encodedJpgBuffer,m_jpegHeaderInfo.iJpegSize, 90, TJPF_RGB))
	{
		TRACE(_T("\nEncoding Success\n"));
		TRACE(_T("\n %d \n", outJpegSize));
		hFile.Write(m_encodedJpgBuffer, m_jpegHeaderInfo.iJpegSize);

		//delete m_encodedJpgBuffer;
	}
	else
	{
		TRACE(_T("\nEncoding failed\n"));
	}

	hFile.Close();


	TRACE(_T("OnSaveDocument"));


}
