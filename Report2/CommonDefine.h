#pragma once
#include <Windows.h>
#include "NetProtocolDefine.h"
#define MAX_LPR_STRING_LENGTH 32
#define MAX_TRACKING_POINT_NUM 100
#define MAX_BACKGROUND_IMAGE_NUM 8

#define SAFE_DELETE_ARRAY(p) {if(p!=NULL) delete[] p; p= NULL;}
typedef struct _IMAGE_INFO_
{
	int iWidth;
	int iHeight;
	int iColor;
	int iSize;
#ifdef _WIN64
	LPBYTE pImage;
#else
	LPBYTE pImage;
	int iDummy;
#endif //���Ǻ� �ڵ� ��� ������ ����
}IMAGE_INFO,*IMAGE_INFO_PTR;

enum class NUMBER_PLATE_TYPE
{
	LP_TYPE_UNKNOWN,
	LP_TYPE_GREEN_2FLOOR,	//���� ���2��
	LP_TYPE_YELLOW_2FLOOR,	//���� Ȳ��2��
	LP_TYPE_WHITE_EURO,		//������ ������ȣ��
	LP_TYPE_SPECIAL,		//Ư����(��)
	LP_TYPE_BIKE,			//�̷���
	LP_TYPE_MAX_NUM

};

enum class ENFORCE_FILE_STEP
{
	ENFILE_STEP_REQ_LPR,	//��ȣ�ν� ��û �ܰ�
	ENFILE_STEP_RET_LPR,	//��ȣ�ν� �Ϸ� �ܰ�
	ENFILE_STEP_FINISH		//�ܼ� Ȯ�� �ܰ�
};

typedef struct _TRACKING_CAR_INFO_
{
	int id;
	int iTrajectoryNum;
	POINT ptTrajectory[MAX_TRACKING_POINT_NUM];
	POINT ptSavePosition[MAX_BACKGROUND_IMAGE_NUM];	// ���� ���� ��ġ
	IMAGE_INFO imgInfo[MAX_BACKGROUND_IMAGE_NUM];

	SYSTEMTIME smSignalChange;			// ��ȣ ����ð�
	SYSTEMTIME smEntryTime;				// ex: 2003-06-07 19:08:09:nnn
	SYSTEMTIME smExitTime;				// ex: 2003-06-07 19:08:09:nnn
	int iViolationSignal;				// TEMS_SIGNAL_STATUS_TYPE
	int iCarDirection;					// 1: ����, 2: ��ȸ��, 3: ��ȸ��, 4: ����
	int iSignalType;					// 1: 3����(��Ȳ��), 2: 3����(��Ȳ��), 3: 4����(��Ȳ����), 4: 5����(��Ȳ������)

	char cReserved[32];
}TRACKING_CAR_INFO, * TRACKING_CAR_INFO_PTR;


typedef struct _ENFORCEMENT_DATA_
{
	int iFileStep;						//ENFORCE_FILE_STEP
	int id;								//detect object id
	DWORD dwEnforceMode;				//�ܼӸ��
	int iViolationType;					//TEMS_VIOLATION_CODE_TYPE
	int iFailType;						//�ܼ� ���� �ڵ�
	float fSpeed;						//�ӵ�
	int iCarType;						//VIOLATION_CAR_TYPE;
	int iLaneNum;						//���� ���� (1 Base..)
	SYSTEMTIME smTime;					//�ܼ� �ð�
	WORD wImageIndex;					//ĸ�� �ε���
	WORD wImageGap;						//�̹��� ĸ�� ����
	float fImageSpeed;
	POINT ptPlate[2];					//�ӵ����� ��ȣ�� �߽� ��ǥ
	int iDayNight;						//0: day, 1: night
	char szLprnum[MAX_LPR_STRING_LENGTH];
	RECT rcPlatePos;
	RECT rcCropPos;						// trigger point ���� cropping�� ��ġ
	RECT rcCropSave;					//�̹��� ����� cropping�� ��ġ
	POINT ptTrigger;					//��ü �̹��� ��ǥ���� Ʈ���Ű� �߻��� ��ǥ
	IMAGE_INFO imgInfo;					//���� �̹��� ����
	IMAGE_INFO plateImgInfo;			//��ȣ�� �̹��� ����
	IMAGE_INFO cropImgInfo;				//Cropping �̹��� ����
	BYTE btStopLineWaitCarNum;			//������ ���������
	BYTE btStopLineVioCarNum;			//������ ����������
	NUMBER_PLATE_TYPE ePlateType;		//��ȣ�� Ÿ��

	TRACKING_CAR_INFO trckingInfo;

	BYTE btHelmet;						//0: no helmet, 1: helmet
	char cReserved[127];


}ENFORCEMENT_DATA, * ENFORCEMENT_DATA_PTR;