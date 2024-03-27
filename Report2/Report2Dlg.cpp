
// Report2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Report2.h"
#include "Report2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STRIDE(W) W + (W % 4 != 0 ? 4 - W % 4 : 0);
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CReport2Dlg 대화 상자



CReport2Dlg::CReport2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REPORT2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_outJpgBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	m_jpegHeaderInfo.pJpegData = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	m_paddingAddedBuffer = new BYTE[MAX_IMAGE_BUFFER_SIZE];
	m_bitInfo = new BITMAPINFO;

	m_font.CreatePointFont(240, _T("Arial"));
	m_iDrawIndex = -1;

}

CReport2Dlg::~CReport2Dlg()
{
	delete[] m_outJpgBuffer;
	delete[] m_jpegHeaderInfo.pJpegData;
	delete[] m_paddingAddedBuffer;
	delete m_bitInfo;
}

void CReport2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editLoadPath);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_PictureControl);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_editSavePath);
	DDX_Control(pDX, IDC_BUTTON_COORDINATE, m_btnCoordinate);
}

BEGIN_MESSAGE_MAP(CReport2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CReport2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_DIR, &CReport2Dlg::OnBnClickedButtonLoadDir)
//	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CReport2Dlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDIR, &CReport2Dlg::OnBnClickedButtonSavedir)
//	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CReport2Dlg::OnBnClickedButtonSave)
ON_BN_CLICKED(IDC_BUTTON_COORDINATE, &CReport2Dlg::OnBnClickedButtonCoordinate)
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CReport2Dlg 메시지 처리기

BOOL CReport2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InItContrl();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CReport2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CReport2Dlg::OnPaint()
{

	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	//윈도우 최소화 여부 확인
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);

	
	}
	else
	{
		TRACE(_T("\nPaint 끝\n"));
	}

	
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CReport2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CReport2Dlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CReport2Dlg::InItContrl()
{
	m_editLoadPath.SetWindowTextW(_T("C:\\Users\\user\\Desktop\\MFC-Study\\photo"));
	m_editSavePath.SetWindowTextW(_T("C:\\Users\\user\\Desktop\\MFC-Study\\photo\\photo-copy\\"));


	m_sSaveDirPath = "C:\\Users\\user\\Desktop\\MFC-Study\\photo\\photo-copy\\";
}

//특정 디렉토리를 100밀리초 간격으로 검색 
//파일이 있으면 파일을 읽어서 화면에 출력
static UINT Thread1(LPVOID pParam)
{
	CFileFind find;
	CReport2Dlg* pDlg = (CReport2Dlg*)AfxGetApp()->m_pMainWnd;
	BOOL blsFile = FALSE;
	CString sSearchPath = pDlg->m_sLoadDirPath;
	int iTotal = 0;
	int iPastTotal = 0;

	//문자열의 오른쪽에서 한 개의 문자 반환 (검색어 설정)
	//폴더 경로 끝에 \가 빠져있으면 추가해준다.
	if (_T("\\") == sSearchPath.Right(1))
	{
		sSearchPath.Format(_T("%s*.G0001"), sSearchPath);
	}
	else
	{
		sSearchPath.Format(_T("%s\\*.G0001"), sSearchPath);
		pDlg->m_sLoadDirPath += "\\";
	}


	//지정한 디렉터리에서 파일을 찾도록 설정 (100ms 주기로 검색)
	//파일 이름을 StringArray에 넣기.
	while (true)
	{	
		Sleep(50);
		iPastTotal = iTotal;
		iTotal = 0;
		blsFile = find.FindFile(sSearchPath);

		if (pDlg->m_sArrayFileName.GetSize() > 0)
		{
			pDlg->m_sArrayFileName.RemoveAll();
		}
		while (blsFile)
		{
			blsFile = find.FindNextFileW();
			if (!find.IsDots() && !find.IsDirectory())
			{
				iTotal++;
				pDlg->m_sArrayFileName.Add(find.GetFileName());
			}
			
		}

		//탐색한 파일의 이름들을 모두 CStringArray에 넣었으니 이것을 하나하나 꺼내보자!
		if (iTotal != iPastTotal && iTotal > 0)
		{
			pDlg->LoadFile();
		}

	}

	return TRUE;
}

void CReport2Dlg::LoadFile()
{
	//폴더 내 파일경로 이름 가져오기
	for (int i = 0; m_sArrayFileName.GetSize(); i++)
	{
		CString sFullPath = m_sLoadDirPath + m_sArrayFileName[i];
		if (m_enforceFile.LoadEnforceFile(sFullPath))
		{
			m_enforceDisp.SetEnforceFile(sFullPath);

		}
		
	}

	

}



void CReport2Dlg::OnBnClickedButtonLoadDir()
{
	/*
		초기 폴더 경로로 사용될 변수
		파일이 존재해야하는지를 나타내는 플래그
		부모 윈도우 핸들 포인터
		대화상자 옵션
	*/
	CFolderPickerDialog picker(m_sLoadDirPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (IDOK == picker.DoModal())
	{
		m_sLoadDirPath = picker.GetPathName();
		m_editLoadPath.SetWindowTextW(m_sLoadDirPath);
		AfxBeginThread(Thread1, this);
		
	}

}

void CReport2Dlg::OnBnClickedButtonSavedir()
{
	CFolderPickerDialog picker;
	if (IDOK == picker.DoModal())
	{
		m_sSaveDirPath = picker.GetPathName();

		if (_T("\\") != m_sSaveDirPath.Right(1))
		{
			m_sSaveDirPath.Format(_T("%s\\"), m_sSaveDirPath);
		}

		m_editSavePath.SetWindowTextW(m_sSaveDirPath);
	}
}

void CReport2Dlg::LoadJpegFile()
{
	CReport2Dlg* pDlg = (CReport2Dlg*)AfxGetApp()->m_pMainWnd;
	CFile cFile;
	CString sFilePath;


	for (int i = 0; i < pDlg->m_sArrayFileName.GetSize(); i++)
	{	
		Sleep(50);
		m_iDrawIndex = i;
		sFilePath = m_sLoadDirPath + m_sArrayFileName[i];
		if (cFile.Open(sFilePath, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("\n %d 번째 출력 \n"), i);
			m_jpegHeaderInfo.iJpegSize = cFile.GetLength();
			cFile.Read(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize);
			CFileStatus cFileStatus;
			cFile.GetStatus(cFileStatus);
			

			cFile.Close();
			
			m_jpgCodec.DecodeImage(m_jpegHeaderInfo.pJpegData, m_jpegHeaderInfo.iJpegSize, m_outJpgBuffer, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, TJPF_BGR);
			PrepareJpegImage(m_jpegHeaderInfo.iWidth * m_jpegHeaderInfo.iHeight * 3, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight);
		}

	}
}

void CReport2Dlg::PrepareJpegImage(int iDecodedJpegSize,int iJpegEncodedWidth, int iJpegEncodedHeight)
{
	//패딩 구하기
	int iPaddingAddedWidth = STRIDE(m_jpegHeaderInfo.iWidth);
	int iPadding = iPaddingAddedWidth - m_jpegHeaderInfo.iWidth;

	int iPaddingAddedSize = iDecodedJpegSize + (iPadding * 3 * m_jpegHeaderInfo.iHeight);
	int iBytePointer = 0;

	//새로운 패딩 추가 버퍼에 데이터 옮기기
	for (int i = 0; i < m_jpegHeaderInfo.iHeight; i++)
	{

		memcpy(m_paddingAddedBuffer + iBytePointer, m_outJpgBuffer + (i * m_jpegHeaderInfo.iWidth * 3), m_jpegHeaderInfo.iWidth * 3);
		iBytePointer += m_jpegHeaderInfo.iWidth * 3;

		for (int k = 0; k < iPadding * 3; k++)
		{
			m_paddingAddedBuffer[iBytePointer++] = 0;
		}

	
	}

	m_jpegHeaderInfo.iWidth = iPaddingAddedWidth;
	m_bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitInfo->bmiHeader.biPlanes = 1;
	m_bitInfo->bmiHeader.biBitCount = 3 * 8;
	//이미지 압축 방식, BI_RGB는 비압축방식
	m_bitInfo->bmiHeader.biCompression = BI_RGB;
	m_bitInfo->bmiHeader.biWidth = m_jpegHeaderInfo.iWidth;
	m_bitInfo->bmiHeader.biHeight = -m_jpegHeaderInfo.iHeight;


	CDC* memDC = new CDC;
	CDC* picDC = m_PictureControl.GetDC();
	CBitmap bmp, * pOldbmp;
	CRect picRect;
	m_PictureControl.GetClientRect(picRect);
	int iWidth = picRect.Width();
	int iHeight = picRect.Height();
	memDC->CreateCompatibleDC(picDC);
	bmp.CreateCompatibleBitmap(picDC, iWidth, iHeight);
	pOldbmp = memDC->SelectObject(&bmp);
	bmp.DeleteObject();
	pOldbmp->DeleteObject();
	StretchDIBits(memDC->GetSafeHdc(), 0, 0, iWidth, iHeight, 0, 0, m_jpegHeaderInfo.iWidth, m_jpegHeaderInfo.iHeight, m_paddingAddedBuffer, m_bitInfo, DIB_RGB_COLORS, SRCCOPY);


	memDC->SelectObject(m_font);
	DrawDataInfo(memDC, picRect);
	picDC->BitBlt(0, 0, iWidth, iHeight, memDC, 0, 0, SRCCOPY);
	SaveJpegImage(m_iDrawIndex, iJpegEncodedWidth, iJpegEncodedHeight);



	DeleteObject(memDC);
	DeleteObject(picDC);
	delete memDC;
	ReleaseDC(picDC);
	

	
}


//사진 헤더 정보 그리기 작업
void CReport2Dlg::DrawColorText(CDC* pDC, CRect& rcText, COLORREF clrText, COLORREF clrOutline, CString& csText, UINT uiFormat)
{
	CRect rcOutLine = rcText;
	pDC->SetTextColor(clrOutline);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if((1 == i) && (1 == j)) continue;

			rcOutLine = rcText;
			rcOutLine.OffsetRect(i, j);
			pDC->DrawText(csText, rcOutLine, uiFormat);
		}
	}

	pDC->SetTextColor(clrText);
	rcText.OffsetRect(1, 1);
	pDC->DrawText(csText, rcText, uiFormat);
}

//사진의 헤더 정보 화면에 출력
void CReport2Dlg::DrawDataInfo(CDC* memDC, CRect rcDisp)
{
	COLORREF crText = RGB(255, 255, 0);
	COLORREF crBack = RGB(0, 0, 0);
	int iTextHeight = 35;
	memDC->SetBkMode(TRANSPARENT); // 배경색을 투명하게 설정
	memDC->SetTextColor(crText);
	CString sText;
	CRect rcText = rcDisp;
	rcText.OffsetRect(5, 5);
	sText.Format(_T("Width:   %d"), m_jpegHeaderInfo.iWidth);
	DrawColorText(memDC, rcText, crText, crBack, sText, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rcText.OffsetRect(0, iTextHeight);
	sText.Format(_T("Height:  %d"), m_jpegHeaderInfo.iHeight);
	DrawColorText(memDC, rcText, crText, crBack, sText, DT_LEFT | DT_TOP | DT_SINGLELINE);
	rcText.OffsetRect(0, iTextHeight);
	sText.Format(_T("Size:    %d"), m_jpegHeaderInfo.iJpegSize);
	DrawColorText(memDC, rcText, crText, crBack, sText, DT_LEFT | DT_TOP | DT_SINGLELINE);

	CFile cFile;
	CFileStatus cFileStatus;
	cFile.Open(m_sLoadDirPath + m_sArrayFileName[m_iDrawIndex], CFile::modeRead | CFile::typeBinary);
	cFile.GetStatus(cFileStatus);
	cFile.Close();

	SYSTEMTIME  stJpgCreate;
	int iSize = sizeof(stJpgCreate);

	rcText.OffsetRect(0, iTextHeight);
	sText.Format(_T("Time: %04d.%02d.%02d.%02d.%02d.%02d"), cFileStatus.m_ctime.GetYear(), cFileStatus.m_ctime.GetMonth(), cFileStatus.m_ctime.GetDay(), cFileStatus.m_ctime.GetHour(), cFileStatus.m_ctime.GetMinute(), cFileStatus.m_ctime.GetSecond());
	DrawColorText(memDC, rcText, crText, crBack, sText, DT_LEFT | DT_TOP | DT_SINGLELINE);

	

}



void CReport2Dlg::SaveJpegImage(int index, int iJpegEncodedWidth, int iJpegEncodedHeight)
{
	DATA_HEADER dataHeader;
	GetLocalTime(&dataHeader.sysTime);
	CFile cFile;


	/*
	JPEG 파일 읽고 사이즈 알기.
	dataheader 만들어서 앞에 붙이기
	*/


	BYTE* encodedJpegBuffer;
	cFile.Open(m_sLoadDirPath + m_sArrayFileName[index], CFile::modeReadWrite | CFile::typeBinary);
	UINT iJpegSize = cFile.GetLength();
	encodedJpegBuffer = new BYTE[iJpegSize];
	dataHeader.iWidth = iJpegEncodedWidth;
	dataHeader.iHeight = iJpegEncodedHeight;
	dataHeader.iFileSize = iJpegSize;
	cFile.Write(&dataHeader, sizeof(dataHeader));
	cFile.Write(encodedJpegBuffer, iJpegSize);
	cFile.Close();
	CString sSaveFileName;


	sSaveFileName.Format(_T("%04d_%02d_%02d_%02d_%02d_%02d_%03d.dat"), dataHeader.sysTime.wYear, dataHeader.sysTime.wMonth, dataHeader.sysTime.wDay, dataHeader.sysTime.wHour, dataHeader.sysTime.wMinute, dataHeader.sysTime.wSecond, dataHeader.sysTime.wMilliseconds);

	MoveFile(m_sLoadDirPath+ m_sArrayFileName[index],m_sSaveDirPath+ sSaveFileName);

	delete[] encodedJpegBuffer;

}


void CReport2Dlg::OnBnClickedButtonCoordinate()
{
	CRect rc1, rc2,rc3,rc4;
	m_PictureControl.GetWindowRect(rc1);
	m_PictureControl.GetClientRect(rc2);
	
	TRACE("\nWindow Area Rect[PictureControl] L: %d T: %d R: %d B: %d \nClient Area Rect[PictureControl] L: %d T: %d R: %d B: %d \n\n", rc1.left, rc1.top, rc1.right, rc1.bottom, rc2.left, rc2.top, rc2.right, rc2.bottom);

	GetWindowRect(rc1);
	GetClientRect(rc2);
	TRACE("\nWindow Area Rect L: %d T: %d R: %d B: %d \nClient Area Rect: %d T: %d R: %d B: %d \n\n", rc1.left, rc1.top, rc1.right, rc1.bottom, rc2.left, rc2.top, rc2.right, rc2.bottom);
	/*int iFrameWidth = rc1.Width() - rc2.Width();
	int iFrameHeight = rc1.Height() - rc2.Height();

	TRACE("\n\nFrame Width: %d \nFrame Height: %d \n\n", iFrameWidth, iFrameHeight);
	TRACE("\n\nSystemMetrics EDGE_Y: %d\n\n", GetSystemMetrics(SM_CYEDGE));
	TRACE("\n\nSystemMetrics EDGE_X: %d\n\n", GetSystemMetrics(SM_CXEDGE));

	TRACE("\n\nSystemMetrics CXSIZEFRAME: %d\n\n", GetSystemMetrics(SM_CXSIZEFRAME));
	TRACE("\n\nSystemMetrics SM_CYSIZEFRAME: %d\n\n", GetSystemMetrics(SM_CYSIZEFRAME));

	TRACE("\n\nSystemMetrics CAPTION: %d\n\n", GetSystemMetrics(SM_CYCAPTION));
	TRACE("\n\nSystemMetrics SM_CYBORDER: %d\n\n", GetSystemMetrics(SM_CYBORDER));
	TRACE("\n\nSystemMetrics SM_CXBORDER: %d\n\n", GetSystemMetrics(SM_CXBORDER));
	WINDOWINFO* windowInfo = new WINDOWINFO;
	this->GetWindowInfo(windowInfo);
	TRACE("\n\nWindow Info [cxWindowBorders: %d cyWindowBorders: %d] \n\n", windowInfo->cxWindowBorders,windowInfo->cyWindowBorders);*/
//
//
//	// mCaptionHeight is the default size of the NC area at
//// the top of the window. If the window has a caption,
//// the size is calculated as the sum of:
////      SM_CYFRAME        - The thickness of the sizing border
////                          around a resizable window
////      SM_CXPADDEDBORDER - The amount of border padding
////                          for captioned windows
////      SM_CYCAPTION      - The height of the caption area
////
//
//	int height = (GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) +
//		GetSystemMetrics(SM_CXPADDEDBORDER));
//	TRACE("\n\n title bar height : %d \n\n", height);
//	int iEdge;     // 테투리 라인 굵기  
//	int iHeight;   // 실제 높이  		
//	// 테투리 사이즈를 구하고		
//	iEdge = GetSystemMetrics(SM_CYEDGE);
//	// 테투리 사이즈를 더한 높이가 실제 높이  
//	iHeight = GetSystemMetrics(SM_CYCAPTION) + iEdge;


		//m_btnCoordinate.GetWindowRect(rc1);
	//m_btnCoordinate.GetClientRect(rc2);

	//TRACE("\n\n 버튼 윈도우 좌표: %d %d %d %d 버튼 클라이언트 좌표: %d %d %d %d \n", rc1.left, rc1.top, rc1.right, rc1.bottom, rc2.left, rc2.top, rc2.right, rc2.bottom);
	

}


void CReport2Dlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	//TRACE("\n\n Point x: %d Point y: %d \n\n", point.x, point.y);

	CDialogEx::OnMouseMove(nFlags, point);
}

