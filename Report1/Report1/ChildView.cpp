
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
	m_InBmpImg = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	m_bitInfo = new BITMAPINFO;;
	m_outJpgBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	m_paddingAddedBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	m_jpegHeaderInfo.pJpegData = new BYTE[MAX_IMAGE_BUFFER_SIZE];
}

CChildView::~CChildView()
{
	if (m_bitInfo != nullptr)
	delete m_bitInfo;
	if (m_outJpgBuffer != nullptr)
	delete[] m_outJpgBuffer;
	if(m_paddingAddedBuffer!=nullptr)
	delete[] m_paddingAddedBuffer;
	if (m_jpegHeaderInfo.pJpegData != nullptr)
	delete[] m_jpegHeaderInfo.pJpegData;

	if (m_InBmpImg != nullptr)
		delete m_InBmpImg;

	OnTestEnd();

	
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_LOAD_BMP, &CChildView::OnLoadBmp)
	ON_COMMAND(ID_LOAD_JPEG, &CChildView::OnLoadJpeg)
	ON_COMMAND(ID_SAVE_BMP, &CChildView::OnSaveBmp)
	ON_COMMAND(ID_SAVE_JPEG, &CChildView::OnSaveJpeg)
	ON_WM_TIMER()
	ON_COMMAND(ID_TEST_END, &CChildView::OnTestEnd)
	ON_COMMAND(ID_TEST_IMAGE, &CChildView::OnTestImage)
	ON_COMMAND(ID_TEST_SIMPLE, &CChildView::OnTestSimple)
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

	if (m_loadMode == 'b')
	{
		if (m_dibHi.biBitCount == 24)
		{
			StretchDIBits(dc.GetSafeHdc(), 800, 0, 1000, 1000, 0, 0, m_dibHi.biWidth, m_dibHi.biHeight, m_InBmpImg, m_bitInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if(m_dibHi.biBitCount == 8)
		{
			int index;
			int rwsize = WIDTHBYTES(m_dibHi.biBitCount * m_dibHi.biWidth);
			//위는 정렬된 가로 행 크기
			TRACE("rwsize: %d ", rwsize);
			for (int i = 0; i < m_dibHi.biHeight; i++)
			{
				for (int j = 0; j < m_dibHi.biWidth; j++)
				{
					index = m_InBmpImg[i * rwsize + j];
					BYTE R = m_palRGB[index].rgbRed;
					BYTE G = m_palRGB[index].rgbGreen;
					BYTE B = m_palRGB[index].rgbBlue;
					//pDC->SetPixel(j, i, RGB(R, G, B));

					//비트맵 이미지의 픽셀 좌표는 하단에서 상단으로 증가하기 때문에	-1을 해서 좌표 변환
					//RGB는 픽셀 색깔
					//pDC->SetPixel(j, pDoc->dibHi.biHeight - i - 1, RGB(R, G, B));

					//최근접 이웃 보간법으로 크기 늘리기 가능
					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++) {
							dc.SetPixel(j * 4 + x + 800, (m_dibHi.biHeight - i - 1) * 4 + y, RGB(R, G, B));
						}
					}
				}
			}
		}
		TRACE(_T("BMP File Paint Complete \n"));

	}
	else if (m_loadMode == 'j')
	{
		//비트맵을 실제 크기로 확대 또는 축소하여 그릴 때 각 픽셀을 단순히 복사
		SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);  // set iMode.
		if (StretchDIBits(dc.GetSafeHdc(), 0, 0, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, 0, 0,
			m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight,
			m_paddingAddedBuffer, m_bitInfo, DIB_RGB_COLORS, SRCCOPY) != GDI_ERROR)
		{
			TRACE(_T("StretchDIBits 성공"));
	
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
		cFile.Read(&m_dibhf, sizeof(BITMAPFILEHEADER));
		cFile.Read(&m_dibHi, sizeof(BITMAPINFOHEADER));

		if (m_dibHi.biBitCount != 8 && m_dibHi.biBitCount != 24)
		{
			AfxMessageBox(_T("need gray/true color"));
			return;
		}

		
		if (m_dibHi.biBitCount == 8)
		{
			cFile.Read(m_palRGB, sizeof(RGBQUAD) * 256);
		}

		int ImgSize;
		if (m_dibHi.biBitCount == 8)
		{
			ImgSize = cFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
		}
	
		else if (m_dibHi.biBitCount == 24)
		{
			ImgSize = cFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		}

		//m_InBmpImg = new unsigned char[ImgSize];

		cFile.Read(m_InBmpImg, ImgSize);
		cFile.Close();
		m_loadMode = 'b';
		OnPrepareBmpImage();
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
	//m_outJpgBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT);
	CString path;
	
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		//m_jpgCodec.GetJpegHeaderInfo(path, m_jpegHeaderInfo);
		CFile cFile;
		cFile.Open(path, CFile::modeRead | CFile::typeBinary);

		m_jpegHeaderInfo.iJpegSize = cFile.GetLength();
		cFile.Read(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize);
		cFile.Close();
		//디코딩 수행.
		//디코딩 수행 결과물: Width, Height,디코딩 된 이미지 버퍼
		m_jpgCodec.DecodeImage(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize, m_outJpgBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, TJPF_BGR);
		m_decodedSize = m_jpegHeaderInfo.iWidth * m_jpegHeaderInfo.iHeight * 3;
		m_loadMode = 'j';

		//Stride 적용 이미지 버퍼 생성 및 복사 수행, BITMAPINFO 초기화
		OnPrepareJpegImage();
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

	hFile.Write(&m_dibhf, sizeof(BITMAPFILEHEADER));
	hFile.Write(&m_dibHi, sizeof(BITMAPINFOHEADER));

	if (m_dibHi.biBitCount == 8)
		hFile.Write(m_palRGB, sizeof(RGBQUAD) * 256);

	hFile.Write(m_InBmpImg, m_dibHi.biSizeImage);
	hFile.Close();

	
	TRACE(_T("OnSaveDocument"));
	



}


void CChildView::OnSaveJpeg()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT);
	CFile hFile;
	CString path;
	LPBYTE encodedJpgBuffer = nullptr;

	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
	}

	if (hFile.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		//인코딩 시에 동적할당
		
		//인코딩하고 정해질 파일 사이즈
		m_jpegHeaderInfo.iJpegSize = 0;

		if
			(m_jpgCodec.EncodeImage(m_paddingAddedBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, &encodedJpgBuffer, m_jpegHeaderInfo.iJpegSize, 90, TJPF_BGR))
		{
			TRACE(_T("\nEncoding Success\n"));
			TRACE(_T("\noutJpegSize: %d \n", outJpegSize));
			hFile.Write(encodedJpgBuffer, m_jpegHeaderInfo.iJpegSize);
		}
		else
		{
			TRACE(_T("\nEncoding failed\n"));
		}

		hFile.Close();
		TRACE(_T("OnSaveDocument"));
	}
	

}


void CChildView::OnPrepareJpegImage()
{
	//패딩 구하기
	int paddingAddedWidth = STRIDE(m_jpegHeaderInfo.iWidth);
	int padding = paddingAddedWidth - m_jpegHeaderInfo.iWidth;

	//CString paddingMessage;
	//paddingMessage.Format(_T("paddingMessage: %d"), paddingAddedWidth);
	//AfxMessageBox(paddingMessage);

	//새로운 버퍼에 패딩이 적용된 사이즈 할당
	int paddingAddedSize = m_decodedSize + (padding * 3 * m_jpegHeaderInfo.iHeight);
	//m_paddingAddedBuffer = new BYTE[paddingAddedSize];

	//버퍼 기록 위치를 바꿔줄 바이터 포인터
	int bytePointer = 0;
	
	//새로운 패딩 추가 버퍼에 데이터 옮기기
	for (int i = 0; i < m_jpegHeaderInfo.iHeight; i++)
	{
		/*for (int j = 0; j < m_jpegHeaderInfo.iWidth * 3; j++)
		{
			m_paddingAddedBuffer[bytePointer++] = m_outJpgBuffer[i * m_jpegHeaderInfo.iWidth * 3 + j];

		}*/
	
		memcpy(m_paddingAddedBuffer + bytePointer, m_outJpgBuffer + (i * m_jpegHeaderInfo.iWidth * 3), m_jpegHeaderInfo.iWidth*3);
		bytePointer += m_jpegHeaderInfo.iWidth*3;

		for (int k = 0; k < padding * 3; k++)
		{
			m_paddingAddedBuffer[bytePointer++] = 0;
		}
	}

	m_jpegHeaderInfo.iWidth = paddingAddedWidth;
	m_bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitInfo->bmiHeader.biPlanes = 1;
	m_bitInfo->bmiHeader.biBitCount = 3 * 8;
	//이미지 압축 방식, BI_RGB는 비압축방식
	m_bitInfo->bmiHeader.biCompression = BI_RGB;
	m_bitInfo->bmiHeader.biWidth = m_jpegHeaderInfo.iWidth;
	m_bitInfo->bmiHeader.biHeight = -m_jpegHeaderInfo.iHeight;
}


void CChildView::OnPrepareBmpImage()
{
	CDC* dc = GetDC();
	int height;
	int width;
	int rwsize;
	//비트맵 영상 헤더
	if (m_dibHi.biBitCount == 24)
	{
		height = m_dibHi.biHeight;
		width = m_dibHi.biWidth;
		m_bitInfo->bmiHeader = m_dibHi;
	}
	
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		PrintTrace();
		break;
	case 2:
		OnLoadImageTest();
		break;

	}
	CWnd::OnTimer(nIDEvent);
}



void CChildView::OnTestSimple()
{
	SetTimer(1, 1000, NULL);
}


void CChildView::OnTestImage()
{
	SetTimer(2, 1000, NULL);
}

void CChildView::PrintTrace()
{
	TRACE(_T("\n타이머에 의해 작동된 안녕~\n"));
}


void CChildView::OnTestEnd()
{
	KillTimer(1);
	KillTimer(2);


	
}

//이미지 불러오기 테스트 -> 헤더 분리 수행 -> 디코딩 수행
void CChildView::OnLoadImageTest()
{
	TRACE(_T("\n 이미지 불러오기 테스트 \n"));
	m_jpegHeaderInfo.iHeight = 0;
	m_jpegHeaderInfo.iJpegSize = 0;
	m_jpegHeaderInfo.iWidth = 0;
	
	//m_outJpgBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	//memset(m_outJpgBuffer, 0, MAX_IMAGE_BUFFER_SIZE);

	if (m_dogTest > 10)
		m_dogTest = 1;
	CString path;
	path.Format(_T("C:\\Users\\user\\Desktop\\MFC-Study\\photo\\dog%d.jpg"),m_dogTest++);
	

	CFile cFile;
	cFile.Open(path, CFile::modeRead | CFile::typeBinary);

	m_jpegHeaderInfo.iJpegSize = cFile.GetLength();
	//m_jpegHeaderInfo.pJpegData = new BYTE[m_jpegHeaderInfo.iJpegSize];
	cFile.Read(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize);
	cFile.Close();

	//디코딩 수행.
	//디코딩 수행 결과물: Width, Height,디코딩 된 이미지 버퍼
	m_jpgCodec.DecodeImage(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize, m_outJpgBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, TJPF_BGR);
	m_decodedSize = m_jpegHeaderInfo.iWidth * m_jpegHeaderInfo.iHeight * 3;
	m_loadMode = 'j';

	//Stride 적용 이미지 버퍼 생성 및 복사 수행, BITMAPINFO 초기화
	OnPrepareJpegImage();
	Invalidate();
}
