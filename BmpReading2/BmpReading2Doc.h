﻿
// BmpReading2Doc.h: CBmpReading2Doc 클래스의 인터페이스
//


#pragma once


class CBmpReading2Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CBmpReading2Doc() noexcept;
	DECLARE_DYNCREATE(CBmpReading2Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	unsigned char* m_InImg;  //bmp파일의 이미지 내용 저장
	unsigned char* m_OutImg; //bmp 이미지 출력용 배열
	BITMAPFILEHEADER dibHf; //비트맵 파일 헤드 구조체
	BITMAPINFOHEADER dibHi; //비트맵 영상 헤드 구조체
	RGBQUAD palRGB[256]; //팔레트 정보 구조체 배열
	virtual ~CBmpReading2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);


};
