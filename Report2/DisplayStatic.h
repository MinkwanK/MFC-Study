#pragma once


class CDisplayStatic :public CStatic
{
public:
	CDisplayStatic();
	virtual ~CDisplayStatic();

	virtual void UpdateDisplayInfo();
	void SetEnforceFile(CString& csEnforceFile) {m_csEnforceFile = csEnforceFile;}
	CString GetEnforceFile() { return m_csEnforceFile; }

public:
	static BITMAPINFO m_bih;
	static LPBYTE m_pImage;

protected:
	CWnd* m_pOwner;

protected:
	CString m_csEnforceFile;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
};