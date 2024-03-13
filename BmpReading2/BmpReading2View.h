
// BmpReading2View.h: CBmpReading2View 클래스의 인터페이스
//

#pragma once


class CBmpReading2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CBmpReading2View() noexcept;
	DECLARE_DYNCREATE(CBmpReading2View)

// 특성입니다.
public:
	CBmpReading2Doc* GetDocument() const;

// 작업입니다.
public:
	CButton m_jpgButton;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	BITMAPINFO* BmInfo; //비트맵 영상 헤드
	CPoint m_p;
	virtual ~CBmpReading2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSaveBmp();
};

#ifndef _DEBUG  // BmpReading2View.cpp의 디버그 버전
inline CBmpReading2Doc* CBmpReading2View::GetDocument() const
   { return reinterpret_cast<CBmpReading2Doc*>(m_pDocument); }
#endif

