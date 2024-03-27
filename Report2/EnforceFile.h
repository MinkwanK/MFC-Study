#pragma once

#include "CommonDefine.h"


typedef enum  _ENFORCE_FILE_CONVERT_TYPE_
{
	efctViolation = 0,		//위반정보 파일명
	efctImageCar,			//차량 이미지 파일명
	efctImageLpr,			//번호판 이미지 파일명
	efctImageTrk,			//트래킹 이미지 파일명
	efctCount,				

}ENFORCE_FILE_CONVERT_TYPE;

class CEnforceFile
{
public:
	CEnforceFile();
	~CEnforceFile();

	ENFORCEMENT_DATA_PTR GetEnforcementData();
	
private:
	ENFORCEMENT_DATA m_enforceData;
	CString m_csFileName = _T("");
	BOOL m_bLoadFile = FALSE;
	BOOL m_bTrkImage = FALSE;

private:
	void InitData();
	void ReleaseData();
public:
	BOOL CEnforceFile::LoadEnforceFile(CString& csFileName, int iRecursion=0);
};