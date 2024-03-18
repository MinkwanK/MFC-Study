#pragma once

#include "../Include/turbojpg/turbojpeg.h"


typedef struct _TB_JPEG_HEADER_INFO_
{
	int iWidth;
	int iHeight;
	int iJpegSize;
	BYTE* pJpegData;
}TB_JPEG_HEADER_INFO, * TB_JPEG_HEADER_INFO_PTR;

class CTubojpg
{
public:
	CTubojpg(void);
	~CTubojpg(void);

	BOOL DecodeImage(LPBYTE pJpgBuffer, int iJpgSize, LPBYTE pOutBuffer, int& iWidth, int& iHeight,
		int target_format = TJPF_RGB);
	BOOL CTubojpg::EncodeImage(LPBYTE pImage, int iWidth, int iHeight, LPBYTE* pOutImage, int& iJpgSize,
		int iJpgQuality, int pixelFormat);
	BOOL GetJpegHeaderInfo(CString szFileName, TB_JPEG_HEADER_INFO& headerInfo);
private:
	tjhandle m_jpegDecompressor = NULL;
	tjhandle m_jpegCompressor = NULL;
	int m_iWidth = 0;
	int m_iHeight = 0;
};
