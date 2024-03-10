
// Simple.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Simple.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleApp

/*
윈도우 메시지와 메시지 핸들러 연결

*/
BEGIN_MESSAGE_MAP(CSimpleApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSimpleApp::OnAppAbout)
END_MESSAGE_MAP()


// CSimpleApp 생성

CSimpleApp::CSimpleApp() noexcept
{

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Simple.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CSimpleApp 개체입니다.
//전역 응용 프로그램 객체 선언. theApp은 응용 프로그램 마법사가 생성한 코드 전체에서 유일한 전역 객체
CSimpleApp theApp;


// CSimpleApp 초기화
//응용 프로그램 호출 시 자동으로 호출되는 가상함수. 응용 프로그램 실행 시 필요한 각종 초기화 코드 넣기.
BOOL CSimpleApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.

	//프로그램과 관련된 정보를 특정 레지스트리 위치에 저장하게 하는 함수. 
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));


	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 애플리케이션의 주 창 개체로 설정합니다.

	//프레임 윈도우 객체를 동적으로 생성한 후 객체의 주소값을 CWinThread::m_pMainWnd 멤버 변수에 저장.
	//CWinThread는 CWinApp의 베이스 클래스이므로 CWinThread의 멤버변수에 접근 가능
	//프레임 윈도우 객체는 만들어졌지만 운영체제 수준의 실제 프레임 윈도우가 생성된 것은 아님.
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 프레임을 만들어 리소스와 함께 로드합니다.
	//실제 프레임 윈도우 생성 이 윈도우를 앞에서 생성한 프레임 윈도우 객체와 연결. 윈도우 생성 시 리소스 정보가 필요한데 IDR_MAINFRAME(여러개의 리소스를 대표하는 ID)에 들어가 있다.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);





	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	//생성된 프레임 윈도우 화면에 표시
	pFrame->ShowWindow(SW_SHOW);
	//프레임 윈도우가 WM_PAINT 함수를 즉시 처리
	pFrame->UpdateWindow();

	//초기화 성공 시 TRUE 리턴
	//FALSE 리턴하면 프로그램 종료
	return TRUE;
}


//종료 가상 함수. 응용 프로그램이 끝날 때 실행해야 하는 각종 청소 코드를 넣는다.
int CSimpleApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	return CWinApp::ExitInstance();
}

// CSimpleApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

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

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CSimpleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSimpleApp 메시지 처리기



