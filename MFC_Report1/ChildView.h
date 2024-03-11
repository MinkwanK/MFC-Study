
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
	CButton m_pushButton;
	CButton m_radio1;
	CButton m_radio2;
	CButton m_group1;
	CButton m_check1;

	BOOL canDraw;
	BOOL drawMode;
	int x1, y1, x2, y2;
	char shape;
	COLORREF color;

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnButtonClicked();
	afx_msg void OnColorRed();
	afx_msg void OnColorGreen();
	afx_msg void OnColorBlue();
	afx_msg void OnShapeCircle();
	afx_msg void OnShapeSquare();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnColorBlack();
	afx_msg void OnUpdateColorBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBlue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorGreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorRed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShapeCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShapeSquare(CCmdUI* pCmdUI);
	void OnCheckClicked();
	afx_msg void OnSaveBmp();
};

