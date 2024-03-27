#pragma once


// TEMS서버에 정의된 단속모드 종류
typedef enum _TEMS_VIOLATION_MODE_TYPE_
{
	// 0 Byte : 0Bit(속도위반), 1Bit(버스전용), 2Bit(신호위반), 3Bit(갓길위반), 4Bit(구간단속), 5Bit(꼬리물기), 6Bit(차로위반), 7Bit(보도주행)
	// 1 Byte : 0Bit(헬멧미착용), 이후 Bit Reserved..
	// 이후 Bypte Reserved..

	tvmtNone = 0x0000,			// 미사용 / 미설정
	tvmtSpeed = 0x0001,			// 속도위반
	tvmtBusOnly = 0x0002,			// 버스전용
	tvmtSignal = 0x0004,			// 신호위반
	tvmtSideRoad = 0x0008,			// 갓길위반
	tvmtSection = 0x0010,			// 구간단속
	tvmtTailFollow = 0x0020,			// 꼬리물기
	tvmtLane = 0x0040,			// 차로위반
	tvmtDriveSideWalk = 0x0080,			// 보도주행
	tvmtNoHelmet = 0x0100,			// 헬멧미착용
}TEMS_VIOLATION_MODE_TYPE;

