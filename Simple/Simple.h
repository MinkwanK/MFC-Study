
// Simple.h: Simple 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
//프로그램에서 리소스를 번호가 아닌 매크로 상수로 참조할 수 있게 해준다. 
//resource.h에 가면 생성한 비트맵의 리소스 번호를 볼 수 있다.


// CSimpleApp:
// 이 클래스의 구현에 대해서는 Simple.cpp을(를) 참조하세요.
//

//응용 프로그램 클래스는 CWinApp 클래스 상속받음. 
class CSimpleApp : public CWinApp
{
public:
	CSimpleApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.

public:
	//OnAppAbout 함수는 프로그램 실행 시 [도움말] -[프로젝트명 정보...]메뉴를 선택하면 자동으로 호출되는 메시지 핸들러
	//afx_msg는 해당 함수가 메시지 핸들러인지 쉽게 구분할 수 있게 해줌.
	afx_msg void OnAppAbout();
	//어떤 클래스에 메시지 핸들러가 한 개 이상 존재하면 DECLARE_MESSAGE_MAP() 매크로가 클래스 선언부에 있어야 함.
	DECLARE_MESSAGE_MAP()
};

extern CSimpleApp theApp;
