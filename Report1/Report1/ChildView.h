
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include "../Include/turbojpg/turbojpeg.h"
#include "Turbojpg.h"

#define MAX_IMAGE_WIDTH				4600
#define MAX_IMAGE_HEIGHT			2500
#define MAX_IMAGE_BUFFER_SIZE		(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT*3)

typedef struct _IMAGE_INFO
{
	int iWidth;
	int iHeight;
	int iColor;
	int iSize;

}IMAGE_INFO, * IMAGE_INFO_PTR;
// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	unsigned char* m_InImg;  //bmp파일의 이미지 내용 저장
	BITMAPFILEHEADER dibHf; //비트맵 파일 헤드 구조체
	BITMAPINFOHEADER dibHi; //비트맵 영상 헤드 구조체
	RGBQUAD palRGB[256]; //팔레트 정보 구조체 배열

	BYTE* m_outJpgBuffer;
	UINT m_jpgSize;
	TB_JPEG_HEADER_INFO m_jpegHeaderInfo;
	CTubojpg m_jpgCodec; //jpgCodec
	char loadMode;

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
	afx_msg void OnLoadBmp();
	afx_msg void OnLoadJpeg();
	afx_msg void OnSaveBmp();
	afx_msg void OnSaveJpeg();
};

