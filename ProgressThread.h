// **********************************
//  �v���O���X�E�_�C�A���O�\���p�̃X���b�h
// **********************************
#include "stdafx.h"

// �X���b�h�Ԃő��݂ɎQ�Ƃ������ϐ�
// volatile �Ő錾���邱��
// �R���p�C���ɍœK�����Ȃ��悤�Ɏw������
volatile extern HWND Thread_dialog;
volatile extern HWND Thread_lfn;
volatile extern HWND Thread_mes;
volatile extern int Thread_Global_m_progress;

// �X���b�h�N�����ɓn���p�����[�^
struct ThreadArg
{
	HANDLE sigHandle;
};

// �X���b�h�̃��C���֐�
DWORD WINAPI ProgressThreadMain(LPVOID p);

// EOF