// **********************************
//  �v���O���X�E�_�C�A���O�\���p�̃X���b�h
//
// (2002/05/11) �V�K�ɍ쐬
// **********************************
#include "stdafx.h"
#include "sTask.h"
#include "sTaskProgress.h"
#include "ProgressThread.h"

// �X���b�h�Ԃő��݂ɎQ�Ƃ������ϐ�
volatile HWND Thread_dialog;
volatile HWND Thread_lfn;
volatile HWND Thread_mes;
volatile int Thread_Global_m_progress;


// **********************************
// �X���b�h�̃��C���֐�
// **********************************
DWORD WINAPI ProgressThreadMain(LPVOID p)
{
	ThreadArg *pArg=(ThreadArg*)p;	// �֐����ŃA�N�Z�X���邽�߂ɐ錾���Ȃ���

	CDlgProgress dlg;		// �v���O���X�E�_�C�A���O

	// **********************************
	//  �ʃX���b�h�p�̏����������邩�ǂ����̔���t���O
	// **********************************
	dlg.m_is_in_thread = TRUE;

	// �Ăяo�����̃X���b�h�ŗ��p���邽�߁A�O���[�o���ϐ��Ɋi�[
	// �Ȃ��A�E�C���h�E�n���h���́ACDlgProgress �� InitDialog ��
	// �l���Z�b�g����邽�߁A�|�C���^��n���Ă���
	dlg.ptr_m_dialog = (HWND *)&Thread_dialog;
	dlg.ptr_m_lfn = (HWND *)&Thread_lfn;
	dlg.ptr_m_mes = (HWND *)&Thread_mes;
	dlg.m_progress_data = (int *)&Thread_Global_m_progress;

	// **********************************
	//  �X���b�h�̊J�n���쐬���̃X���b�h�ɒʒm (�X���b�h�ԒʐM)
	// **********************************
	if(pArg->sigHandle != NULL) ::SetEvent(pArg->sigHandle);	// �C�x���g�̃Z�b�g

	// **********************************
	//  �v���O���X�E�_�C�A���O�̕\��
	//  �i�\���̍X�V�́A�쐬���X���b�h��� WM_PAINT ���b�Z�[�W���󂯂čs���j
	// **********************************
	dlg.DoModal();

	// **********************************
	//  �X���b�h�̊J�n���쐬���̃X���b�h�ɒʒm (�X���b�h�ԒʐM)
	//  �i�E�C���h�E��������Ƃ�ʒm�j
	// **********************************
	if(pArg->sigHandle != NULL) ::SetEvent(pArg->sigHandle);	// �C�x���g�̃Z�b�g

	return 0L;
}

// EOF
