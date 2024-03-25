
// Report2Dlg.h: 헤더 파일
//

#pragma once
#include "TurboJpg.h"
#define MAX_IMAGE_WIDTH				4600
#define MAX_IMAGE_HEIGHT			2500
#define MAX_IMAGE_BUFFER_SIZE		(MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT*3)


//.dat 데이터 헤더
typedef struct _Data_Header
{
	int iWidth;
	int iHeight;
	int iFileSize;
	SYSTEMTIME sysTime;
}DATA_HEADER;
// CReport2Dlg 대화 상자
class CReport2Dlg : public CDialogEx
{
// 생성입니다.
public:
	CReport2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CReport2Dlg();

// 대화 상자 데이터입니다.
public:
	//JPG 파일 탐색 경로
	CString m_sLoadDirPath;
	CString m_sSaveDirPath;
	CFont m_font;
	//Jpg 파일 경로 저장 Array
	CStringArray m_sArrayJpegName;
	
	TB_JPEG_HEADER_INFO m_jpegHeaderInfo;
	CTubojpg m_jpgCodec; 
	BITMAPINFO* m_bitInfo;
	SYSTEMTIME m_JpegTime;

	BYTE* m_outJpgBuffer;
	BYTE* m_paddingAddedBuffer;

	int m_iDrawIndex;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPORT2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonDir();
	CEdit m_editLoadPath;
	void InItContrl();
	void FindImageFiles();
	void LoadJpegFile();
	void PrepareJpegImage(int iDecodedJpegSize);
	CStatic m_PictureControl;
	void CReport2Dlg::DrawDataInfo(int index);
	CEdit m_editSavePath;
	afx_msg void OnBnClickedButtonSavedir();
	void DrawColorText(CDC* pDC, CRect& rcText, COLORREF clrText, COLORREF clrOutline, CString& csText, UINT uiFormat);
	void SaveJpegImage(int index);
};
