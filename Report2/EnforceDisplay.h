#pragma once
#include "DisplayStatic.h"
#include "EnforceFile.h"

class CEnforceDisplay :public CDisplayStatic
{
public:
	CEnforceDisplay(void);
	~CEnforceDisplay(void);

	virtual void UpdateDisplayInfo();

protected:
	CEnforceFile m_enforceFile;
	IMAGE_INFO m_lprOrgImage;
	IMAGE_INFO m_lprCropImage;
	IMAGE_INFO m_trkImage[MAX_BACKGROUND_IMAGE_NUM];
	BOOL m_bSetData = FALSE;



public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};