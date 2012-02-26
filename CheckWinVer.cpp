// ************************************************************
// Windows �o�[�W�����֘A�֐�
//
// ************************************************************

#include "StdAfx.h"
#include "CheckWinVer.h"


// �������s�̃��W�X�g���ʒu
#define STR_REGKEY_RUN		"Software\\Microsoft\\Windows\\CurrentVersion\\Run"


// ************************************************************
// WindowsNT �n��� Windows 95 �n��̔���
// WinNT �n��̂Ƃ� TRUE
// ************************************************************
BOOL IsWinNT(void)
{
	OSVERSIONINFO ver;

	// OS�o�[�W����
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);


	if(ver.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return TRUE;

	return FALSE;
}

// ************************************************************
// HKLM �� ���W�X�g�� Run �ɏ������݌��������邩���ׂ�
// ��������Ƃ� TRUE
// ************************************************************

BOOL IsPossibleWriteHklmRun(void)
{
	BOOL ret = FALSE;
	HKEY hCU;
	char szProg[MAX_PATH] = "hi_soft_test_data";

	// �e�X�g�f�[�^�����݂��Ă���ꍇ�͏������Ă���
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteValue( hCU, "hi_soft_test");
		RegCloseKey(hCU);
	}

	// �Ƃ肠�����e�X�g�f�[�^����������ł݂āA���肷��
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{	// ���W�X�g�����������݃��[�h�ŃI�[�v���o�����Ƃ�
		if(RegSetValueEx( hCU, "hi_soft_test", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg)) != ERROR_SUCCESS)
			ret = FALSE;	// �������݂ł��Ȃ������Ƃ�
		else
			ret = TRUE;		// �������ݐ��������Ƃ�
		RegCloseKey(hCU);
	}
	else
	{	// ���W�X�g�����������݃��[�h�ŃI�[�v���ł��Ȃ������Ƃ�
		ret = FALSE;
	}

	// �e�X�g�f�[�^����������
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REGKEY_RUN, 0, KEY_WRITE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteValue( hCU, "hi_soft_test");
		RegCloseKey(hCU);
	}
	
	return ret;
}