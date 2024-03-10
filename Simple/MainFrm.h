
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ChildView.h"


//프레임 윈도우 기능을 가진 CFrameWnd 클래스를 상속 받음.
class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame() noexcept;
protected: 
	//이 매크로는 생성되는 객체를 대상으로 실행 시간 클래스 정보 기능을 사용할 수 있게 해준다.
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	//CFrameWnd 클래스에서 상속받은 가상함수 재정의
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	//CObject 클래스에서 상속받은 가상함수 재정의. 각각 타당성 점검기능, 디버깅을 위한 객체 상태 출력기능
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//뷰 객체 선언
	CChildView    m_wndView;

// 생성된 메시지 맵 함수
protected:
	//afx_msg로 시작하므로 메시지 핸들러
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

};


