#include "pch.h"
#include "EnforceFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEnforceFile::CEnforceFile()
{
	InitData();
}

CEnforceFile::~CEnforceFile()
{
	ReleaseData();
}

ENFORCEMENT_DATA_PTR CEnforceFile::GetEnforcementData()
{
	if (!m_bLoadFile) return NULL;

	return &m_enforceData;
}

void CEnforceFile::InitData()
{
	ZeroMemory(&m_enforceData, sizeof(m_enforceData));
	m_enforceData.imgInfo.pImage = NULL;
	m_bLoadFile = FALSE;
	m_bTrkImage = FALSE;
	m_csFileName = _T("");
}

void CEnforceFile::ReleaseData()
{
	m_bLoadFile = FALSE;
	m_bTrkImage = FALSE;
	m_csFileName = _T("");

	SAFE_DELETE_ARRAY(m_enforceData.imgInfo.pImage);
	
	for (int i = 0; i < MAX_BACKGROUND_IMAGE_NUM; i++)
	{
		SAFE_DELETE_ARRAY(m_enforceData.trckingInfo.imgInfo[i].pImage);
	}
}


/*

원본이미지, 잘린이미지, 단속판이미지, 추적이미지를 순서대로 파일에서 읽는다.
*/
BOOL CEnforceFile::LoadEnforceFile(CString& csFileName, int iRecursion /* = 0*/)
{
	BOOL bRet = FALSE;
	do
	{
		if (3 <= iRecursion)
		{
			TRACE(_T("Fail to open File Name: %s \n"), csFileName);
			break;
		}

		CFile fileTarget;
		if (!fileTarget.Open(csFileName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("Fail to open File: %s \n"), csFileName);
			break;
		}

		ULONGLONG iFileSize = fileTarget.GetLength();
		ULONGLONG iRemainSize = iFileSize;
		const int iHeaderSize = sizeof(ENFORCEMENT_DATA);

		if (iFileSize <= iHeaderSize)
		{
			fileTarget.Close();
			break;
		}

		ReleaseData();
		m_csFileName = csFileName;
		fileTarget.Read(&m_enforceData, iHeaderSize);
		iRemainSize -= iHeaderSize;

		//여기서 문제 발생 
		ULONGLONG iTotalSize = (m_enforceData.imgInfo.iSize + m_enforceData.plateImgInfo.iSize);

		if (iRemainSize < iTotalSize)
		{
			TRACE(_T("lpr file size if wrong(%s)\n"), csFileName);
			fileTarget.Close();
			break;
		}

		m_enforceData.imgInfo.pImage = new BYTE[m_enforceData.imgInfo.iSize];
		fileTarget.Read(m_enforceData.imgInfo.pImage, m_enforceData.imgInfo.iSize);
		iRemainSize -= m_enforceData.imgInfo.iSize;
		
		if (m_enforceData.iFileStep == (int)ENFORCE_FILE_STEP::ENFILE_STEP_RET_LPR)
		{
			m_enforceData.cropImgInfo.pImage = new BYTE[m_enforceData.cropImgInfo.iSize];
			fileTarget.Read(m_enforceData.cropImgInfo.pImage, m_enforceData.cropImgInfo.iSize);
			iRemainSize -= m_enforceData.cropImgInfo.iSize;
			m_enforceData.plateImgInfo.pImage = nullptr;
			if ((m_enforceData.plateImgInfo.iSize > 0) && (iRemainSize >= m_enforceData.plateImgInfo.iSize))
			{
				m_enforceData.plateImgInfo.pImage = new BYTE[m_enforceData.plateImgInfo.iSize];
				fileTarget.Read(m_enforceData.plateImgInfo.pImage, m_enforceData.plateImgInfo.iSize);
			}
		}
		else if (m_enforceData.iFileStep == (int)ENFORCE_FILE_STEP::ENFILE_STEP_FINISH)
		{
			m_enforceData.cropImgInfo.pImage = new BYTE[m_enforceData.cropImgInfo.iSize];
			fileTarget.Read(m_enforceData.cropImgInfo.pImage, m_enforceData.cropImgInfo.iSize); 
			iRemainSize -= m_enforceData.cropImgInfo.iSize;
			m_enforceData.plateImgInfo.pImage = nullptr;

			if ((m_enforceData.plateImgInfo.iSize > 0) && (iRemainSize >=
				m_enforceData.plateImgInfo.iSize))
			{
				m_enforceData.plateImgInfo.pImage = new BYTE[m_enforceData.plateImgInfo.iSize];

				fileTarget.Read(m_enforceData.plateImgInfo.pImage,
					m_enforceData.plateImgInfo.iSize);
				iRemainSize -= m_enforceData.plateImgInfo.iSize;
			}

			//단속모드
			if (tvmtSignal & m_enforceData.dwEnforceMode)
			{
				for (int i = 0; i < MAX_BACKGROUND_IMAGE_NUM; i++)
				{
					int iFileSize = m_enforceData.trckingInfo.imgInfo[i].iSize;
					m_enforceData.trckingInfo.imgInfo[i].pImage = nullptr;

					if ((iFileSize > 0) && (iRemainSize >= iFileSize))
					{
						m_enforceData.trckingInfo.imgInfo[i].pImage = new BYTE[m_enforceData.trckingInfo.imgInfo[i].iSize];

						//CFile::Read의 Return Value는 버퍼로 전송된 바이트 수이다.
						if (0 == fileTarget.Read(m_enforceData.trckingInfo.imgInfo[i].pImage, iFileSize))
						{
							break;
						}
						iRemainSize -= iFileSize;
					}

				}
			}
			//단속모드 미설정시??
			else
			{
				for (int i = 0; i < MAX_BACKGROUND_IMAGE_NUM; i++)
				{
					m_enforceData.trckingInfo.imgInfo[i].pImage = nullptr;
				}
			}
		}
		//번호판 인식이 안됐으면
		else
		{
			for (int i = 0; i < MAX_BACKGROUND_IMAGE_NUM; i++)
			{
				m_enforceData.trckingInfo.imgInfo[i].pImage = nullptr;
			}
		}

		fileTarget.Close();
		bRet = m_bLoadFile = TRUE;



	} while (0); //while 0는 조건부 실행을 구현하는데 사용?

	return bRet;
}


