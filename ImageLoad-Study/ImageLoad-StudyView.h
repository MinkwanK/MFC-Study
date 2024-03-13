
// ImageLoad-StudyView.h: CImageLoadStudyView 클래스의 인터페이스
//

#pragma once


class CImageLoadStudyView : public CView
{
protected: // serialization에서만 만들어집니다.
	CImageLoadStudyView() noexcept;
	DECLARE_DYNCREATE(CImageLoadStudyView)

// 특성입니다.
public:
	CImageLoadStudyDoc* GetDocument() const;
	BITMAPINFO* BmInfo; //비트맵 영상 헤드
// 작업입니다.
public:

// 재정의입니다.
public:
	CPoint m_p;
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CImageLoadStudyView();
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
};

#ifndef _DEBUG  // ImageLoad-StudyView.cpp의 디버그 버전
inline CImageLoadStudyDoc* CImageLoadStudyView::GetDocument() const
   { return reinterpret_cast<CImageLoadStudyDoc*>(m_pDocument); }
#endif

