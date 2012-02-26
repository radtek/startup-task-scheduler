// **********************************
//  sTask ���C���֐� (sTaskApp) �̒�`
// **********************************
// sTask.h : STASK �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_STASK_H__E19A2865_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASK_H__E19A2865_0616_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��
#include "TaskArray.h"	// ClassView �ɂ���Ēǉ�����܂����B

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� sTask.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSTaskApp : public CWinApp
{
public:
	unsigned int GenerateRandom(unsigned int min, unsigned int max);
	BOOL UninstallSeq(void);
	BOOL InstallSeq(void);
	BOOL RegReadAll(void);
	BOOL RegWriteAll(void);
	CTaskArray m_tasks;
	CSTaskApp();
	BOOL m_bMutexOwner;
	HANDLE m_hMutex;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSTaskApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CSTaskApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void HtmlHelp(DWORD dwData,UINT nCmd);

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STASK_H__E19A2865_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)
