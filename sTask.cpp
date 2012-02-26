// **********************************
//  sTask ���C���֐� (sTaskApp)�A����� sTaskAbout �̒�`
//
// (2002/05/11) �ʃX���b�h�Ńv���O���X�_�C�A���O�\����
// **********************************

#include "stdafx.h"
#include "sTask.h"

#include "MainFrm.h"
#include "sTaskDoc.h"
#include "sTaskView.h"
#include "sTaskDlg.h"
#include "InstFunc.h"
#include "CheckWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp

BEGIN_MESSAGE_MAP(CSTaskApp, CWinApp)
	//{{AFX_MSG_MAP(CSTaskApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp �N���X�̍\�z

CSTaskApp::CSTaskApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTaskApp �I�u�W�F�N�g

CSTaskApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp �N���X�̏�����

BOOL CSTaskApp::InitInstance()
{
	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������
	// ��������Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ���
	// ���폜���Ă��������B

	CString tmpstr,tmpstr2;

#ifdef _AFXDLL
	Enable3dControls();		// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif

	// �ݒ肪�ۑ�����鉺�̃��W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͏����ȂǓK�؂Ȃ��̂�
	// �ύX���Ă��������B
	// ���W�X�g���̎w��
	// ���\�[�X��AFX_IDS_APP_TITLE�ɃA�v���P�[�V��������ݒ肷�邱�� 
	// m_pszAppName = "compdel" �Ƃ��Ă͂����Ȃ��I ASSERTION �G���[�ƂȂ�
	SetRegistryKey((LPCTSTR)"hi soft");

	LoadStdProfileSettings(0);	// MRU���X�g�Ǘ�(0:���Ȃ�) *** ver 1.2 Win95 �G���[�̉��

	// **********************************
	//  ���W�X�g������S�ݒ�l��ǂݍ���
	// **********************************
	RegReadAll();

	// **********************************
	//  �R�}���h���C�������ɂ�镪��
	// **********************************
	if(!strcmpi(m_lpCmdLine, ""))
	{	// �I�v�V�����Ȃ�
		// **********************************
		//  �C���X�g�[���̋N���B
		//  ���łɁA�C���X�g�[���ς݂Ǝ��́A�P�� TRUE ��Ԃ�����
		// **********************************
		if(!InstallSeq())
			return FALSE;	// �C���X�g�[�������[�U���L�����Z�������Ƃ� �i�v���O�����I���j

	}
	else if(!strcmpi(m_lpCmdLine, "/start"))
	{	// �������s
		if(m_tasks.g_delay > 0) ::Sleep(m_tasks.g_delay*1000);
		m_tasks.ExecAllTask(NULL);
		RegWriteAll();
		return FALSE;		// �i�E�C���h�E��\�������ɁA�v���O�����I���j
	}
	else if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// �A���C���X�g�[��
		UninstallSeq();
		return FALSE;		// �i�E�C���h�E��\�������ɁA�v���O�����I���j
	}
	else
	{
		// ���̑��̈����̂Ƃ��́A�G���[�Ƃ���B
		// �i�����t���ŁA�h�L�������g�E�r���[�̃N���X���N���ł��Ȃ����߁j
		::MessageBox(NULL, "Parameter Error\r\n\r\n(\"/start\" and \"/delete\" is acceptable)", "sTask (Information)", MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
		return FALSE;		// �i�E�C���h�E��\�������ɁA�v���O�����I���j
	}


	// **********************************
	//  �p�X���[�h�ɂ��ی삪����ꍇ
	// **********************************
	if(m_tasks.g_secure)
	{
		CDlgPasswd dlg;
		dlg.m_message.LoadString(STR_PAS_IMP);
		dlg.DoModal();
		if(dlg.m_passwd != m_tasks.g_passwd)
		{
			tmpstr.LoadString(STR_ERRPAS);
			::MessageBox(NULL, (LPCSTR)tmpstr, "sTask (Error)", MB_OK|MB_ICONSTOP|MB_APPLMODAL);
			return FALSE;		// �p�X���[�h����v���Ȃ��������ߏI��
		}
	}

	// **********************************
	//  �ݒ�_�C�A���O�̕\��
	// **********************************

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B

//		AfxInitRichEdit();	// 1999.11.24 �R�����g�A�E�g�@�s�v�H
//		AfxEnableControlContainer() ; // 99.11.24 �R�����g�A�E�g�@�s�v�H
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSTaskDoc),
		RUNTIME_CLASS(CMainFrame),       // ���C�� SDI �t���[�� �E�B���h�E
		RUNTIME_CLASS(CSTaskView));
	AddDocTemplate(pDocTemplate);

	// ���̂ւ�� MFC �W���̃R�}���h���C����͂͏����Ȃ����ƁB�����Ńh�L�������g�̏����������Ă���
	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// �R�}���h���C���Ńf�B�X�p�b�` �R�}���h���w�肵�܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;	// �E�C���h�E��\������ (TRUE�̏ꍇ)

	// �v���O�����I�����̃��W�X�g���ւ̕ۑ���
	// CSTaskView::OnDestroy() �֐��̒��ōs���Ă���
}

/////////////////////////////////////////////////////////////////////////////
// CSTaskApp ���b�Z�[�W �n���h��

// **********************************
// ���W�X�g���ɏ������ށi�S���j
// **********************************
BOOL CSTaskApp::RegWriteAll()
{
	CString str_n;

	if(m_tasks.g_debug) return FALSE;	// �f�o�b�O���[�h�̂Ƃ��͕ۑ����Ȃ�

	if(m_tasks.g_delay != GetProfileInt("Settings","delay",3))
		WriteProfileInt("Settings","delay",m_tasks.g_delay);
	if(m_tasks.g_time != (BOOL)GetProfileInt("Settings","time",1))
		WriteProfileInt("Settings","time",m_tasks.g_time);
	if(m_tasks.g_dialog != (BOOL)GetProfileInt("Settings","dlg",1))
		WriteProfileInt("Settings","dlg",m_tasks.g_dialog);
	if(m_tasks.g_dialog0 != (BOOL)GetProfileInt("Settings","dlg0",0))
		WriteProfileInt("Settings","dlg0",m_tasks.g_dialog0);
	if(m_tasks.g_faildlg != (BOOL)GetProfileInt("Settings","dlgfl",1))
		WriteProfileInt("Settings","dlgfl",m_tasks.g_faildlg);
	if(m_tasks.g_secure != (BOOL)GetProfileInt("Settings","secure",0))
		WriteProfileInt("Settings","secure",m_tasks.g_secure);
	if(m_tasks.g_passwd != GetProfileString("Settings","pass",""))
		WriteProfileString("Settings","pass",m_tasks.g_passwd);
	if(m_tasks.maxtask != GetProfileInt("Settings","maxtask",0))
		WriteProfileInt("Settings","maxtask",m_tasks.maxtask);

	for(UINT i=0; i<m_tasks.maxtask; i++)
	{
		str_n.Format("task%d",i);
		if(m_tasks.tasks[i].time_lastexec != CTime(GetProfileInt(str_n,"tlexec",0)))
			WriteProfileInt(str_n,"tlexec",m_tasks.tasks[i].time_lastexec.GetTime());
		if(m_tasks.tasks[i].name != GetProfileString(str_n,"name"))
			WriteProfileString(str_n,"name",m_tasks.tasks[i].name);
		if(m_tasks.tasks[i].fpass != GetProfileString(str_n,"fpass"))
			WriteProfileString(str_n,"fpass",m_tasks.tasks[i].fpass);
		if(m_tasks.tasks[i].param != GetProfileString(str_n,"param"))
			WriteProfileString(str_n,"param",m_tasks.tasks[i].param);
		if(m_tasks.tasks[i].mes != GetProfileString(str_n,"mes"))
			WriteProfileString(str_n,"mes",m_tasks.tasks[i].mes);
		if(m_tasks.tasks[i].exec != (BOOL)GetProfileInt(str_n,"exec",0))
			WriteProfileInt(str_n,"exec",m_tasks.tasks[i].exec);
		if(m_tasks.tasks[i].apart != GetProfileInt(str_n,"apart",0))
			WriteProfileInt(str_n,"apart",m_tasks.tasks[i].apart);
		if(m_tasks.tasks[i].interval != GetProfileInt(str_n,"intvl",1))
			WriteProfileInt(str_n,"intvl",m_tasks.tasks[i].interval);
		if(m_tasks.tasks[i].date != GetProfileInt(str_n,"date",0))
			WriteProfileInt(str_n,"date",m_tasks.tasks[i].date);
		if(m_tasks.tasks[i].excludetime != (BOOL)GetProfileInt(str_n,"extime",0))
			WriteProfileInt(str_n,"extime",m_tasks.tasks[i].excludetime);
		if(m_tasks.tasks[i].time0 != CTime(GetProfileInt(str_n,"t0",0)))
			WriteProfileInt(str_n,"t0",m_tasks.tasks[i].time0.GetTime());
		if(m_tasks.tasks[i].time1 != CTime(GetProfileInt(str_n,"t1",0)))
			WriteProfileInt(str_n,"t1",m_tasks.tasks[i].time1.GetTime());
		if(m_tasks.tasks[i].waitexit != (BOOL)GetProfileInt(str_n,"wexit",0))
			WriteProfileInt(str_n,"wexit",m_tasks.tasks[i].waitexit);
		if(m_tasks.tasks[i].waitsec != GetProfileInt(str_n,"wsec",0))
			WriteProfileInt(str_n,"wsec",m_tasks.tasks[i].waitsec);
		if(m_tasks.tasks[i].taskoff != (BOOL)GetProfileInt(str_n,"toff",0))
			WriteProfileInt(str_n,"toff",m_tasks.tasks[i].taskoff);
		if(m_tasks.tasks[i].taskoffcount != GetProfileInt(str_n,"toffcnt",0))
			WriteProfileInt(str_n,"toffcnt",m_tasks.tasks[i].taskoffcount);
		if(m_tasks.tasks[i].dialog != (BOOL)GetProfileInt(str_n,"dlg",1))
			WriteProfileInt(str_n,"dlg",m_tasks.tasks[i].dialog);
		if(m_tasks.tasks[i].syncprev != (BOOL)GetProfileInt(str_n,"sync",0))
			WriteProfileInt(str_n,"sync",m_tasks.tasks[i].syncprev);
		if(m_tasks.tasks[i].cnt_check != GetProfileInt(str_n,"c_c",0))
			WriteProfileInt(str_n,"c_c",m_tasks.tasks[i].cnt_check);
		if(m_tasks.tasks[i].cnt_exec != GetProfileInt(str_n,"c_e",0))
			WriteProfileInt(str_n,"c_e",m_tasks.tasks[i].cnt_exec);
		
	}
	return TRUE;
}

// **********************************
// ���W�X�g������ǂݍ��ށi�S���j
// **********************************
BOOL CSTaskApp::RegReadAll()
{
	CString str_n;

	m_tasks.g_delay = GetProfileInt("Settings","delay",3);
	m_tasks.g_time = GetProfileInt("Settings","time",1);
	m_tasks.g_dialog = GetProfileInt("Settings","dlg",1);
	m_tasks.g_dialog0 = GetProfileInt("Settings","dlg0",0);
	m_tasks.g_faildlg = GetProfileInt("Settings","dlgfl",1);
	m_tasks.g_secure = GetProfileInt("Settings","secure",0);
	m_tasks.g_passwd = GetProfileString("Settings","pass","");
	m_tasks.maxtask = GetProfileInt("Settings","maxtask",0);

	for(UINT i=0; i<m_tasks.maxtask; i++)
	{
		str_n.Format("task%d",i);
		m_tasks.tasks[i].time_lastexec = CTime(GetProfileInt(str_n,"tlexec",0));
		m_tasks.tasks[i].name = GetProfileString(str_n,"name");
		m_tasks.tasks[i].fpass = GetProfileString(str_n,"fpass");
		m_tasks.tasks[i].param = GetProfileString(str_n,"param");
		m_tasks.tasks[i].mes = GetProfileString(str_n,"mes");
		m_tasks.tasks[i].exec = GetProfileInt(str_n,"exec",0);
		m_tasks.tasks[i].apart = GetProfileInt(str_n,"apart",0);
		m_tasks.tasks[i].interval = GetProfileInt(str_n,"intvl",1);
		m_tasks.tasks[i].date = GetProfileInt(str_n,"date",0);
		m_tasks.tasks[i].excludetime = GetProfileInt(str_n,"extime",0);
		m_tasks.tasks[i].time0 = CTime(GetProfileInt(str_n,"t0",0));
		m_tasks.tasks[i].time1 = CTime(GetProfileInt(str_n,"t1",0));
		m_tasks.tasks[i].waitexit = GetProfileInt(str_n,"wexit",0);
		m_tasks.tasks[i].waitsec = GetProfileInt(str_n,"wsec",0);
		m_tasks.tasks[i].taskoff = GetProfileInt(str_n,"toff",0);
		m_tasks.tasks[i].taskoffcount = GetProfileInt(str_n,"toffcnt",0);
		m_tasks.tasks[i].dialog = GetProfileInt(str_n,"dlg",1);
		m_tasks.tasks[i].syncprev = GetProfileInt(str_n,"sync",0);
		m_tasks.tasks[i].cnt_check = GetProfileInt(str_n,"c_c",0);
		m_tasks.tasks[i].cnt_exec = GetProfileInt(str_n,"c_e",0);
		// �I�[�o�[�t���[�΍�
		if(m_tasks.tasks[i].cnt_check > 1000) m_tasks.tasks[i].cnt_check=0;
		if(m_tasks.tasks[i].cnt_exec > 1000) m_tasks.tasks[i].cnt_exec = 0;
	}
	return TRUE;

}


// **********************************
//  �C���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�C���X�g�[�������܂��͕s�v,  FALSE:���[�U�L�����Z��
// **********************************
BOOL CSTaskApp::InstallSeq()
{
	CDlgInstall dlg;
	dlg.m_r_autoexec = 1;	// �X�^�[�g���j���[�ɓo�^
	dlg.m_chk_uninst_start = TRUE;	// �X�^�[�g���j���[�ɃA���C���X�g�[���A�C�R��
	dlg.m_IsAdminInst = TRUE;


	// **********************************
	//  �C���X�g�[���t���O�̃`�F�b�N (HKLM/HKCU)
	// **********************************
	if(ChkHkcuInstReg() || ChkHklmInstReg()) return TRUE;

// ***********************
// sTask �ŗL  �O�o�[�W�����܂ł̐ݒ�l������
// ***********************
	::RmUserRegEx();

	// **********************************
	//  Windows NT �n��Ƃ���ȊO�ŋ��
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT �̂Ƃ� ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run �������݉̂Ƃ� (Admin ���[�h)
			if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
			if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin �C���X�g�[��
			else ::MkHkcuInstReg();	// Current User �C���X�g�[��
		}
		else
		{	// HKLM ... Run �������ݕs�̂Ƃ� (���[�U ���[�h)
			dlg.m_IsAdminInst = FALSE;	
			if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
			::MkHkcuInstReg();
		}
	}
	else
	{	// ********************  WinNT�ȊO �̂Ƃ� ********************
		if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
		if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin �C���X�g�[��
		else ::MkHkcuInstReg();	// Current User �C���X�g�[��
	}

	// **********************************
	//  �C���X�g�[���_�C�A���O�ł̑I�����ڂɂ��A�C���X�g�[�����
	// **********************************
	//  �������s�I�v�V����
	//  �I�����ꂽ�I�v�V�����ȊO�́u�������s�v���폜���邱�Ƃ������ɍs��
	// **********************************
	switch(dlg.m_r_autoexec)
	{
	case 0 :	// �������s���Ȃ�
		::RmHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	case 1 :	// �X�^�[�g���j���[
		::RmHklmLnk();
		::RmHkcuLnk();
		::MkStartMnu();
		break;
	case 2 :	// HKCU �Ɏ������s
		::RmHklmLnk();
		::MkHkcuLnk();
		::RmStartMnu();
		break;
	case 3 :	// HKLM �Ɏ������s
		::MkHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	}

	// **********************************
	//  �X�^�[�g���j���[�ɐݒ�A�C�R���o�^
	// **********************************
	::MkProgramsMnu();

	// **********************************
	//  �R���p�l����A���C���X�g�[������ݒ�
	// **********************************
	if(dlg.m_chk_uninst_cpl)
	{
		if(::IsWinNT()) ::MkHkcuUninstMnu();
		else ::MkHklmUninstMnu();
	}

	// **********************************
	//  �X�^�[�g���j���[����A���C���X�g�[������ݒ�
	// **********************************
	if(dlg.m_chk_uninst_start)
		::MkUninstMnu();

// sTask �ŗL !
	// **********************************
	// �p�X���[�h�̍쐬
	// **********************************
	int i;
	char strTmp3[2];	// �p�X���[�h�쐬�p
	CString tmpstr, tmpstr2;
	tmpstr = "";
	for(i=0; i<8; i++)
	{
		strTmp3[0] = GenerateRandom(0x41, 0x5a);
		strTmp3[1] = (char)NULL;
		tmpstr += strTmp3;
	}
	m_tasks.g_passwd = tmpstr;
	tmpstr.LoadString(STR_MES_NEW_PASSWORD);
	tmpstr2.Format(tmpstr, m_tasks.g_passwd);
	::MessageBox(NULL, (LPCSTR)tmpstr2, "sTask (information) NEW PASSWORD", MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);

	return TRUE;
}

// **********************************
//  �A���C���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�����܂��͕s�v,  FALSE:���s�܂��̓��[�U�L�����Z��
// **********************************
BOOL CSTaskApp::UninstallSeq()
{
	CString sTmpMsg;

	// **********************************
	//  Windows NT �n��Ƃ���ȊO�ŋ��
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT �̂Ƃ� ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run �������݉̂Ƃ� (Admin ���[�h�����H)
			if(::ChkHklmInstReg())
			{	// Admin ���[�h�ŃC���X�g�[������Ă����Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_ADM);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmHklmInstReg();
			}
			else
			{	// �J�����g�E���[�U�E���[�h�̂Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmHkcuInstReg();
			}
		}
		else
		{	// HKLM ... Run �������ݕs�̂Ƃ� (���[�U ���[�h)
			if(::ChkHklmInstReg())
			{	// Admin ���[�h�ŃC���X�g�[������Ă����Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_NOT);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmUserRegEx();	// ���[�U�f�[�^�̂ݏ�������
				::RmHkcuInstReg();	// �ǂ���ɂ��Ă��A����͏����Ă���
				//::RmHklmLnk();	// HKLM �̃C���X�g�[���t���O�͏������Ȃ�
				::RmHkcuLnk();
				::RmStartMnu();
				::RmHkcuUninstMnu();
				::RmUninstMnu();
				::RmProgramsMnu();
				return FALSE;
			}
			else
			{	// �J�����g�E���[�U�E���[�h�̂Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmHkcuInstReg();
			}
		}
	}
	else
	{	// ********************  WinNT�ȊO �̂Ƃ� ********************
		if(::ChkHklmInstReg())
		{	// Admin ���[�h�ŃC���X�g�[������Ă����Ƃ�
			sTmpMsg.LoadString(STR_MES_UNIN_ADM);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ���[�U���L�����Z��
			::RmHklmInstReg();
		}
		else
		{	// �J�����g�E���[�U�E���[�h�̂Ƃ�
			sTmpMsg.LoadString(STR_MES_UNIN_CUR);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "sTask Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ���[�U���L�����Z��
			::RmHkcuInstReg();
		}
	}

	::RmHkcuInstReg();		// �ǂ���ɂ��Ă��A����͏����Ă���
	::RmHklmLnk();
	::RmHkcuLnk();
	::RmStartMnu();
// ***********************
// sTask �ŗL
// ***********************
	::RmUserRegEx();
//	::RmUserReg();

	::RmHkcuUninstMnu();
	::RmHklmUninstMnu();
	::RmUninstMnu();

	::RmProgramsMnu();

	return TRUE;

}

unsigned int CSTaskApp::GenerateRandom(unsigned int min, unsigned int max)
{
	unsigned int i=0;
	do
	{
		i = ((rand() & 0xff) * (max-min+1))/0xff + min;
	}while(i<min || i>max);

	return i;
}


// **********************************
// sTaskAbout �̒�`
// **********************************
/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard ���z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CSTaskApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
