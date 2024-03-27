#pragma once


// TEMS������ ���ǵ� �ܼӸ�� ����
typedef enum _TEMS_VIOLATION_MODE_TYPE_
{
	// 0 Byte : 0Bit(�ӵ�����), 1Bit(��������), 2Bit(��ȣ����), 3Bit(��������), 4Bit(�����ܼ�), 5Bit(��������), 6Bit(��������), 7Bit(��������)
	// 1 Byte : 0Bit(��������), ���� Bit Reserved..
	// ���� Bypte Reserved..

	tvmtNone = 0x0000,			// �̻�� / �̼���
	tvmtSpeed = 0x0001,			// �ӵ�����
	tvmtBusOnly = 0x0002,			// ��������
	tvmtSignal = 0x0004,			// ��ȣ����
	tvmtSideRoad = 0x0008,			// ��������
	tvmtSection = 0x0010,			// �����ܼ�
	tvmtTailFollow = 0x0020,			// ��������
	tvmtLane = 0x0040,			// ��������
	tvmtDriveSideWalk = 0x0080,			// ��������
	tvmtNoHelmet = 0x0100,			// ��������
}TEMS_VIOLATION_MODE_TYPE;

