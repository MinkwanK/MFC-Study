#include "pch.h"
#include "DisplayStatic.h"
 

BITMAPINFO CDisplayStatic::m_bih;
LPBYTE CDisplayStatic::m_pImage = nullptr;

BEGIN_MESSAGE_MAP(CDisplayStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CDisplayStatic::CDisplayStatic()
{ 
	m_pOwner = NULL;
}

CDisplayStatic::~CDisplayStatic()
{
}

void CDisplayStatic::UpdateDisplayInfo()
{
	Invalidate();
}

void CDisplayStatic::OnPaint()
{
}

void CDisplayStatic::OnSize(UINT nType, int cx, int cy)
{
}

void CDisplayStatic::OnShowWindow(BOOL bShow, UINT nStatus)
{
}

void CDisplayStatic::OnDestroy()
{
}
