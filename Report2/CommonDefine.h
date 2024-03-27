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
#endif //조건부 코드 블록 마무리 역할
}IMAGE_INFO,*IMAGE_INFO_PTR;

enum class NUMBER_PLATE_TYPE
{
	LP_TYPE_UNKNOWN,
	LP_TYPE_GREEN_2FLOOR,	//구형 녹색2층
	LP_TYPE_YELLOW_2FLOOR,	//영업 황색2층
	LP_TYPE_WHITE_EURO,		//유럽형 전국번호판
	LP_TYPE_SPECIAL,		//특장차(영)
	LP_TYPE_BIKE,			//이륜차
	LP_TYPE_MAX_NUM

};

enum class ENFORCE_FILE_STEP
{
	ENFILE_STEP_REQ_LPR,	//번호인식 요청 단계
	ENFILE_STEP_RET_LPR,	//번호인식 완료 단계
	ENFILE_STEP_FINISH		//단속 확성 단계
};

typedef struct _TRACKING_CAR_INFO_
{
	int id;
	int iTrajectoryNum;
	POINT ptTrajectory[MAX_TRACKING_POINT_NUM];
	POINT ptSavePosition[MAX_BACKGROUND_IMAGE_NUM];	// 사진 찍은 위치
	IMAGE_INFO imgInfo[MAX_BACKGROUND_IMAGE_NUM];

	SYSTEMTIME smSignalChange;			// 신호 변경시간
	SYSTEMTIME smEntryTime;				// ex: 2003-06-07 19:08:09:nnn
	SYSTEMTIME smExitTime;				// ex: 2003-06-07 19:08:09:nnn
	int iViolationSignal;				// TEMS_SIGNAL_STATUS_TYPE
	int iCarDirection;					// 1: 직진, 2: 좌회전, 3: 우회전, 4: 정지
	int iSignalType;					// 1: 3색등(적황좌), 2: 3색등(적황직), 3: 4색등(적황좌직), 4: 5색등(적황좌직우)

	char cReserved[32];
}TRACKING_CAR_INFO, * TRACKING_CAR_INFO_PTR;


typedef struct _ENFORCEMENT_DATA_
{
	int iFileStep;						//ENFORCE_FILE_STEP
	int id;								//detect object id
	DWORD dwEnforceMode;				//단속모드
	int iViolationType;					//TEMS_VIOLATION_CODE_TYPE
	int iFailType;						//단속 실패 코드
	float fSpeed;						//속도
	int iCarType;						//VIOLATION_CAR_TYPE;
	int iLaneNum;						//실제 차선 (1 Base..)
	SYSTEMTIME smTime;					//단속 시간
	WORD wImageIndex;					//캡쳐 인덱스
	WORD wImageGap;						//이미지 캡쳐 간격
	float fImageSpeed;
	POINT ptPlate[2];					//속도계산용 번호판 중심 좌표
	int iDayNight;						//0: day, 1: night
	char szLprnum[MAX_LPR_STRING_LENGTH];
	RECT rcPlatePos;
	RECT rcCropPos;						// trigger point 기준 cropping한 위치
	RECT rcCropSave;					//이미지 저장시 cropping한 위치
	POINT ptTrigger;					//전체 이미지 좌표에서 트리거가 발생한 좌표
	IMAGE_INFO imgInfo;					//원본 이미지 정보
	IMAGE_INFO plateImgInfo;			//번호판 이미지 정보
	IMAGE_INFO cropImgInfo;				//Cropping 이미지 정보
	BYTE btStopLineWaitCarNum;			//정지선 대기차량수
	BYTE btStopLineVioCarNum;			//정지선 위반차량수
	NUMBER_PLATE_TYPE ePlateType;		//번호판 타입

	TRACKING_CAR_INFO trckingInfo;

	BYTE btHelmet;						//0: no helmet, 1: helmet
	char cReserved[127];


}ENFORCEMENT_DATA, * ENFORCEMENT_DATA_PTR;