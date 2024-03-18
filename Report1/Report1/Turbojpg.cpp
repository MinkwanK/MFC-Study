#include "pch.h"
#include "Turbojpg.h"


#pragma comment(lib, "turbojpeg.lib")
#define WIDTHBYTES(bits) (((bits)+31)/32*4); //이미지 가로 바이트 길이는 4바이트의 배수 (?)
CTubojpg::CTubojpg(void)
{
	m_jpegDecompressor = tjInitDecompress();
	m_jpegCompressor = tjInitCompress();
}

CTubojpg::~CTubojpg(void)
{
	tjDestroy(m_jpegDecompressor);
	tjDestroy(m_jpegCompressor);
}


	//TB_JPEC_HEADER_INFO 변수에 분리한 헤더정보 기입
	BOOL CTubojpg::GetJpegHeaderInfo(CString path, TB_JPEG_HEADER_INFO& headerInfo)
	{
		BOOL bRet = FALSE;
		do
		{
			if (NULL == m_jpegDecompressor)
				break;

			CFile fJpgFile;
			bRet = fJpgFile.Open(path, CFile::modeRead | CFile::typeBinary);
			if (!bRet)
			{
				//WRITE_LOGE(_T("%s fail to open jpg file\n"), __FUNCTION__);
				break;
			}
			headerInfo.iJpegSize = (int)fJpgFile.GetLength();
			headerInfo.pJpegData = new BYTE[headerInfo.iJpegSize];
			fJpgFile.Read(headerInfo.pJpegData, headerInfo.iJpegSize);
			fJpgFile.Close();

			//jpeg 서브샘플링 값이 저장됨.
			int jpegSubsamp = 0;
			int iRet = tjDecompressHeader2(m_jpegDecompressor, headerInfo.pJpegData, headerInfo.iJpegSize,
				&headerInfo.iWidth, &headerInfo.iHeight, &jpegSubsamp);
			TRACE(_T("JPEG Width: %d\n"), headerInfo.iWidth);
			TRACE(_T("JPEG Height: %d\n"), headerInfo.iHeight);
			TRACE(_T("JPEG Subsampling: %d\n"), jpegSubsamp);
			bRet = TRUE;
		} while (0);

		return bRet;
	}

BOOL CTubojpg::DecodeImage(LPBYTE pJpgBuffer, int iJpgSize, LPBYTE pOutBuffer, int& iWidth, int& iHeight, int target_format)
{
	BOOL bRet = FALSE;
	do
	{
		if (NULL == m_jpegDecompressor)
			break;

		//이미지의 색상 구성
		int jpegSubsamp;
		int iRet = tjDecompressHeader2(m_jpegDecompressor, pJpgBuffer, iJpgSize, &iWidth, &iHeight, &jpegSubsamp);
		//int target_format = TJPF_RGB;
		//target_format: 디코딩된 이미지의 목표 포맷. 출력 이미지의 색상 공간 및 비트수를 지정함.
		if (0 != tjDecompress2(m_jpegDecompressor, pJpgBuffer, iJpgSize, pOutBuffer, iWidth, 0/*pitch*/, iHeight,
			target_format, TJFLAG_FASTDCT))
		{
			TRACE("Decoding error : %s\n", tjGetErrorStr());
			break;
		}

		bRet = TRUE;
	} while (0);

	return bRet;
}

BOOL CTubojpg::EncodeImage(LPBYTE pImage, int iWidth, int iHeight, LPBYTE* pOutImage, int& iJpgSize,
	int iJpgQuality, int pixelFormat)
{
	BOOL bRet = FALSE;
	do
	{
		if (NULL == m_jpegCompressor)
			break;

		int iPitches = TJPAD(iWidth * tjPixelSize[pixelFormat]);
		int iBytes = (pixelFormat == TJPF_GRAY) ? 1 : 3; 
		//pixelFormat은 원본 이미지의 픽셀 포맷, TJSAMP_420은 압축형식, TJFLAG_ACCURATEDCT 더 정확힌 dct 알고리즘 사용
		if (0 != tjCompress2(m_jpegCompressor, pImage, iWidth, 0, iHeight, pixelFormat, pOutImage,
			(unsigned long*)&iJpgSize, TJSAMP_420 , iJpgQuality, TJFLAG_ACCURATEDCT))
		{
			break;
		}


		bRet = TRUE;
	} while (0);

	return bRet;
}
