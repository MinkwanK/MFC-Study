
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

}

void CReport2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CReport2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CReport2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_DIR, &CReport2Dlg::OnBnClickedButtonDir)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CReport2Dlg::OnBnClickedButtonSearch)
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
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
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
	m_edit.SetWindowTextW(_T("경로 지정"));
}


void CReport2Dlg::OnBnClickedButtonDir()
{
	/*
		초기 폴더 경로로 사용될 변수
		파일이 존재해야하는지를 나타내는 플래그
		부모 윈도우 핸들 포인터
		대화상자 옵션
	*/
	CFolderPickerDialog picker(m_sDirPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (IDOK == picker.DoModal())
	{
		m_sDirPath = picker.GetPathName();
		m_edit.SetWindowTextW(m_sDirPath);
		
	}

}




void CReport2Dlg::FindImageFiles()
{
	
}

static UINT Thread1(LPVOID pParam)
{
	CFileFind find;
	CReport2Dlg* pDlg = (CReport2Dlg*)AfxGetApp()->m_pMainWnd;
	BOOL blsFile = FALSE;
	CString sSearchPath = pDlg->m_sDirPath;
	
	
	//문자열의 오른쪽에서 한 개의 문자 반환
	if (_T("\\") == sSearchPath.Right(1))
	{
		sSearchPath.Format(_T("%s*.jpg"), sSearchPath);
	}
	else
	{
		sSearchPath.Format(_T("%s\\*.jpg"), sSearchPath);
	}
	TRACE(sSearchPath);
	//지정한 디렉터리에서 파일을 찾도록 설정. 파일을 찾으면 TRUE 반환 없으면 FALSE 반환
	blsFile = find.FindFile(sSearchPath);
	if (blsFile)
	{
		TRACE("\nFile Find\n");
	}
	else {
		TRACE("\n File not found \n");
	}
	while (blsFile)
	{
		blsFile = find.FindNextFileW();
		CString fileName;
		fileName.Format(_T("\n FileName: %s \n"), find.GetFileName());
		TRACE(fileName);
		//find에서 현재 검색된 파일이.(현재 디렉터리) 또는 ..(상위 디렉터리)
		//현재 검색된 파일이 디렉터리인지?
		if (!find.IsDots() && !find.IsDirectory())
		{
			pDlg->m_listSearchedImg.Add(find.GetFileName());
		}
		
	}

	return TRUE;
}

//검색 버튼 클릭하면 이미지 탐색 스레드 시작
void CReport2Dlg::OnBnClickedButtonSearch()
{
	AfxBeginThread(Thread1, this);
}
