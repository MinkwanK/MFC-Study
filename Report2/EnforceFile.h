#pragma once

#include "CommonDefine.h"


typedef enum  _ENFORCE_FILE_CONVERT_TYPE_
{
	efctViolation = 0,		//�������� ���ϸ�
	efctImageCar,			//���� �̹��� ���ϸ�
	efctImageLpr,			//��ȣ�� �̹��� ���ϸ�
	efctImageTrk,			//Ʈ��ŷ �̹��� ���ϸ�
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