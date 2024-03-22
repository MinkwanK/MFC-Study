
// Report2Dlg.h: 헤더 파일
//

#pragma once


// CReport2Dlg 대화 상자
class CReport2Dlg : public CDialogEx
{
// 생성입니다.
public:
	CReport2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
public:
	//JPG 파일 탐색 경로
	CString m_sDirPath;
	//Jpg 파일 경로 저장 Array
	CStringArray m_listSearchedImg;


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
	CEdit m_edit;
	void InItContrl();
	void FindImageFiles();
	
	afx_msg void OnBnClickedButtonSearch();
};
