
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once


// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	CButton m_button1;
	CButton m_radio1;
	CButton m_radio2;
	CButton m_group1;
// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//DECLARE_DYNCREATE(CChildView)
	DECLARE_MESSAGE_MAP()

public:
	void OnButtonClicked();
};

