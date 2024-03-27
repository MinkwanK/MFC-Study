#include "pch.h"
#include "EnforceDisplay.h"


BEGIN_MESSAGE_MAP(CEnforceDisplay, CDisplayStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CEnforceDisplay::CEnforceDisplay(void)
{
	if(m_csEnforceFile.GetLength() > 0)
	{
		ENFORCEMENT_DATA_PTR pEnforceData = m_enforceFile.GetEnforcementData();
	}
}

CEnforceDisplay::~CEnforceDisplay(void)
{
}

void CEnforceDisplay::UpdateDisplayInfo()
{
}

void CEnforceDisplay::OnPaint()
{
}
