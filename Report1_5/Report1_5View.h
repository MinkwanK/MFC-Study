
// Report1_5View.h: CReport15View 클래스의 인터페이스
//

#pragma once


class CReport15View : public CView
{
protected: // serialization에서만 만들어집니다.
	CReport15View() noexcept;
	DECLARE_DYNCREATE(CReport15View)

// 특성입니다.
public:
	CReport15Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CReport15View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Report1_5View.cpp의 디버그 버전
inline CReport15Doc* CReport15View::GetDocument() const
   { return reinterpret_cast<CReport15Doc*>(m_pDocument); }
#endif

