// **********************************
// �^�X�N�z��̊Ǘ��ƁA�^�X�N�̎��s�E���s����
// **********************************

#include "stdafx.h"
#include "sTask.h"
#include "TaskArray.h"

#include "ProgressThread.h"		// �v���O���X�E�_�C�A���O �X���b�h�p

#include "sTaskProgress.h"			// �f�o�b�O���̃v���O���X�E�_�C�A���O�p

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CTaskArray::CTaskArray()
{
	maxtask = 0;
	g_debug = FALSE;
}

CTaskArray::~CTaskArray()
{
}

// **********************************
// �V�����^�X�N��ǉ�����
// **********************************
BOOL CTaskArray::NewTask(CString name, CString fpass, CString param, CString mes,
			BOOL exec, UINT apart, UINT interval, UINT date,
			BOOL excludetime, CTime time0, CTime time1,
			BOOL waitexit, UINT waitsec, BOOL taskoff, UINT taskoffcount,
			BOOL dialog, BOOL syncprev, int syncexec)
{
	if(maxtask>20) return FALSE;
	int i=maxtask;
	maxtask++;

	tasks[i].name=name;
	tasks[i].fpass=fpass;
	tasks[i].param=param;
	tasks[i].mes=mes;
	tasks[i].exec=exec;
	tasks[i].apart=apart;
	tasks[i].interval=interval;
	tasks[i].date=date;
	tasks[i].excludetime=excludetime;
	tasks[i].time0=time0;
	tasks[i].time1=time1;
	tasks[i].waitexit=waitexit;
	tasks[i].waitsec=waitsec;
	tasks[i].taskoff=taskoff;
	tasks[i].taskoffcount=taskoffcount;
	tasks[i].dialog=dialog;
	tasks[i].syncprev=syncprev;
	tasks[i].syncexec=syncexec;
	tasks[i].time_lastexec= CTime::GetCurrentTime();
	tasks[i].cnt_check = 0;
	tasks[i].cnt_exec = 0;

	return TRUE;
}

// **********************************
// ����͂�����^�X�N�����s�ł��邩�A������Ԃ�
// ���� i : �^�X�N�z�񒆂̃^�X�N�ԍ�
// **********************************
CTime CTaskArray::CalcNextTime(UINT i)
{
	int j;
	CTime prev = tasks[i].time_lastexec;
	CTime ret;

	if(tasks[i].apart == 0) // ���񂩂̋N���ɂP��
		ret = CTime(1990,1,1,0,0,0);	// ���ݎ�����Ԃ�
	else if(tasks[i].apart == 1)
	{	// �������ɂP��
		ret = prev + CTimeSpan::CTimeSpan(tasks[i].interval,0,0,0);
	}
	else if(tasks[i].apart == 2)
	{	// ���T���ɂP��
		ret = prev + CTimeSpan::CTimeSpan(tasks[i].interval*7,0,0,0);
	}
	else if(tasks[i].apart == 3)
	{	// ���������ɂP��
		int year = prev.GetYear();
		int month = prev.GetMonth();
		month += tasks[i].interval;
		// ���t�����̃o�O��� 99.11.25
		year += (month - 1) / 12;
		month = month % 12;
		if(month == 0) month = 12;
		ret = CTime(year, month, prev.GetDay(), prev.GetHour(), prev.GetMinute(), prev.GetSecond());
	}
	else ret = CTime::GetCurrentTime();

	// �j�����O����
	for(j=0;j<2;j++)
	{
		if((tasks[i].date & 0x40) && (ret.GetDayOfWeek() == 1)) ret += CTimeSpan::CTimeSpan(1,0,0,0); // ���j
		if((tasks[i].date & 0x20) && (ret.GetDayOfWeek() == 2)) ret += CTimeSpan::CTimeSpan(1,0,0,0); // ���j
		if((tasks[i].date & 0x10) && (ret.GetDayOfWeek() == 3)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x08) && (ret.GetDayOfWeek() == 4)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x04) && (ret.GetDayOfWeek() == 5)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x02) && (ret.GetDayOfWeek() == 6)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
		if((tasks[i].date & 0x01) && (ret.GetDayOfWeek() == 7)) ret += CTimeSpan::CTimeSpan(1,0,0,0);
	}

	// ���Ԃ̎����i���Ԃ��Ԋu�Ƃ��邩�j
	if(g_time)	// ���Ԃ𖳎�����
		ret = CTime(ret.GetYear(),ret.GetMonth(),ret.GetDay(),0,0,0);

	return ret;
}

// **********************************
// �^�X�N���ЂƂ�������
// **********************************
BOOL CTaskArray::DeleteTask(UINT i)
{
	// i : �I������Ă���A�C�e�� 0 ... maxtask-1
	if(i>=maxtask) return FALSE;
	if(i==maxtask-1)
	{	// �ŏI�A�C�e���̍폜�̏ꍇ
		maxtask--;
		return TRUE;
	}
	// �����l�߂鏈��������
	// tasks[0] ... tasks[maxtask-2] �ƂȂ�
	for(UINT j=i; j<maxtask-1; j++)
		tasks[j] = tasks[j+1];	// j+1��maxtask�ɂȂ�܂ŌJ��Ԃ�
	maxtask--;
	return TRUE;
}

// **********************************
// �^�X�N�����ւ���
// **********************************
BOOL CTaskArray::SwapTask(UINT i, UINT j)
{
	if(i>=maxtask || j>=maxtask) return FALSE;
	CTask dummyTask;
	dummyTask = tasks[i];
	tasks[i] = tasks[j];
	tasks[j] = dummyTask;
	return TRUE;
}

// **********************************
// �^�X�N�i�P�j�̎��s
// **********************************
// ����Ƀ^�X�N�����s���ꂽ�� TRUE ��Ԃ�
//
#include <process.h>	// _P_WAIT �p
BOOL CTaskArray::ExecTask(UINT i, HWND hWnd)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTmp1, strTmp2;
	PROCESS_INFORMATION pi;		// CreateProcess �p
	STARTUPINFO si;				// CreateProcess �p
	char strTmp3[MAX_PATH];		// CreateProcess �p
   
	_splitpath((LPCSTR)tasks[i].fpass, drive, dir, fname, ext);

	// �f�o�b�O���[�h�ł̕\��
	if(g_debug)
	{

		strTmp2.LoadString(STR_EXEC_DEBUG);
		strTmp1.Format(strTmp2, tasks[i].name, tasks[i].fpass, tasks[i].param, ext);
		strTmp2.LoadString(STR_EXEC_DEBUG_TITLE);
		if(::MessageBox(hWnd, strTmp1, strTmp2, MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND)
			!= IDYES) return FALSE;
	}
	// ��f�o�b�O���[�h�ł̕\��
	else if(tasks[i].dialog)
	{
		strTmp2.LoadString(STR_EXEC_NORM);
		if(tasks[i].mes == "")
			strTmp1.Format(strTmp2, tasks[i].name);
		else strTmp1 = tasks[i].mes;
		strTmp2.LoadString(STR_APPNAME);
		if(::MessageBox(hWnd, strTmp1, strTmp2, MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND)
			!= IDYES) return FALSE;
	}

	// �E�G�C�g�}��
	if(tasks[i].waitsec > 0) ::Sleep(tasks[i].waitsec*1000);

	// **********************************
	// �g���q�ɂ��A���s�\�t�@�C���� SPAWN �A����ȊO�̓V�F�����s
	// **********************************
//			******************** ver 1.61b *.cmd �� CreateProcess �Ŏ��s
	if(!strcmpi(ext,".exe") || !strcmpi(ext,".com") || !strcmpi(ext,".bat") || !strcmpi(ext,".cmd"))	
	{
		if(tasks[i].waitexit)
		{	// ���̃^�X�N��ҋ@������
			// (2002/05/11) spawnl ���� spawnlp �ɕύX

//			******************** ver 1.61b �v���Z�X���s���@��ύX�i�X�y�[�X�̂���p�X���ɑΉ��j
//			if(_spawnlp(_P_WAIT,tasks[i].fpass,tasks[i].fpass,tasks[i].param,NULL) < 0)
//				return FALSE;


			// �r���ɃX�y�[�X�̂���p�X���ւ̑Ή�
			if(strstr(tasks[i].fpass, " ") != NULL)
				sprintf(strTmp3, "\"%s\" %s", tasks[i].fpass, tasks[i].param);
			else
				sprintf(strTmp3, "%s %s", tasks[i].fpass, tasks[i].param);

			// �v���Z�X�̍쐬�E���s
			ZeroMemory(&si,sizeof(si));
			si.cb=sizeof(si);
			if(!::CreateProcess(NULL, strTmp3, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
				return FALSE;	// �v���Z�X�쐬�Ɏ��s

			::WaitForSingleObject(pi.hProcess, INFINITE);		// �v���O�������I������܂őҋ@
		}
		else
		{
//			******************** ver 1.61b �v���Z�X���s���@��ύX�i�X�y�[�X�̂���p�X���ɑΉ��j
//			if(_spawnlp(_P_NOWAIT,tasks[i].fpass,tasks[i].fpass,tasks[i].param,NULL) < 0)
//				return FALSE;
			// �r���ɃX�y�[�X�̂���p�X���ւ̑Ή�
			if(strstr(tasks[i].fpass, " ") != NULL)
				sprintf(strTmp3, "\"%s\" %s", tasks[i].fpass, tasks[i].param);
			else
				sprintf(strTmp3, "%s %s", tasks[i].fpass, tasks[i].param);

			// �v���Z�X�̍쐬�E���s
			ZeroMemory(&si,sizeof(si));
			si.cb=sizeof(si);
			if(!::CreateProcess(NULL, strTmp3, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
				return FALSE;	// �v���Z�X�쐬�Ɏ��s

		}

	}
	else
	{	// �f�[�^�t�@�C���Ȃǂ̃V�F�����s
		if((UINT)::ShellExecute(NULL,"OPEN",tasks[i].fpass,tasks[i].param,"",SW_SHOW) <= 32)
			return FALSE;
	}

	tasks[i].cnt_exec++;	// �J�E���g�A�b�v
	tasks[i].time_lastexec = CTime::GetCurrentTime();	// ���ݎ����̑��
	if(tasks[i].cnt_exec >= tasks[i].taskoffcount && tasks[i].taskoff)
		tasks[i].exec = FALSE;	// �w��񐔂őҋ@��Ԃ�
	tasks[i].cnt_check = 0;	// �`�F�b�N�J�E���^�[�̓��Z�b�g�����
	if(i < maxtask-1)
	{	// ���̃^�X�N�����݂��āA���O�̃^�X�N�ɓ��������郂�[�h��������
		if(tasks[i+1].syncprev) tasks[i+1].time_lastexec = tasks[i].time_lastexec;
	}

	return TRUE;	// ����Ɏ��s���ꂽ
}

// **********************************
// ���ׂẴ^�X�N�̎��s�����𒲂ׂāA���s�ł�������Ȃ���s����
// **********************************
BOOL CTaskArray::ExecAllTask(HWND hWnd)
{
	UINT i;
	CTime Tnow, Tnext;
	CString strTmp1, strTmp2, strAppName;
	// ****** �X���b�h�p ******
	static ThreadArg p_ThreadParam;	// �X���b�h�p�p�����[�^
	HANDLE h_ThreadHandle = NULL;	// �X���b�h�E�n���h��
	HANDLE h_EventHandle = NULL;	// �X���b�h�ԒʐM�p�̃C�x���g�E�n���h��
	DWORD i_ThreadPID;
	// ****** ���[�h���X�_�C�A���O�p�i�f�o�b�O���̂ݎg�p�j ******
	CDlgProgress *dlgDebugProgress;				// �v���O���X�\�� ���[�h���X�_�C�A���O
	BOOL debug_dlg_OK = FALSE;		// ���[�h���X�E�_�C�A���O�쐬�t���O

	BOOL bLastTaskExec;				// ��O�̃^�X�N�����s���ꂽ �isyncexec ����p�j

	if(maxtask==0) return FALSE;	// ���s���ׂ��^�X�N���Ȃ�
	strAppName.LoadString(STR_APPNAME);	// ���炩���߃v���O�����������[�h���Ă����i���p���邽�߁j

	// **********************************
	//  �X���b�h�ԒʐM�p �C�x���g�E�n���h���̍쐬
	// **********************************
	h_EventHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	p_ThreadParam.sigHandle = h_EventHandle;	// �X���b�h���Ŏg�p���邽��
	// **********************************
	//  �X���b�h���쐬����i�v���O���X�E�_�C�A���O�p�j �i�f�o�b�O���͎g�p���Ȃ��j
	// **********************************
	// (2002/05/11) �f�o�b�O���[�h�ŃG���[�̂��߁A�X���b�h�𗘗p���Ȃ�
	if(g_dialog && !g_debug)
	{
		Thread_Global_m_progress = 0;	// �v���O���X�E�o�[�� �����l 0
		// (2002/05/13) PID ���󂯎�������n���iWin98�ł̃X���b�h�쐬�s�ɑΉ��j
		h_ThreadHandle = ::CreateThread(NULL, 0, ::ProgressThreadMain, (LPVOID)&p_ThreadParam, 0, &i_ThreadPID);
		if(h_ThreadHandle == NULL)
		{
			// �X���b�h���쐬�ł��Ȃ�����
			::MessageBox(NULL, "Internal Error (Fail to Create Progress Dialog Thread)", "sTask (Warning)", MB_ICONWARNING|MB_OK|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
		}
		else
		{
			if(h_EventHandle != NULL)
			{
				// **********************************
				//  �X���b�h�̊J�n��҂�  �i�R�b�Ń^�C���A�E�g�j
				// **********************************
				if(::WaitForSingleObject(h_EventHandle, 3000) == WAIT_TIMEOUT)
				{
					// �X���b�h������ɍ쐬����Ă��Ȃ��Ɣ��f
					// �X���b�h���I��������
					::CloseHandle(h_ThreadHandle);
					h_ThreadHandle = NULL;		// NULL ���Z�b�g���āA���̌�g���Ȃ��悤�ɂ���
				}
				else
				{
					::Sleep(500);	// �_�C�A���O�쐬�܂ŗ]�T��^���� 0.5sec
				}
				// �C�x���g�̃��Z�b�g
				::ResetEvent(h_EventHandle);
			}
			else ::Sleep(2000);	// �C�x���g�����p�ł��Ȃ��Ƃ��� 2 �b�҂i���S��j
		}
	}
	// **********************************
	//  �v���O���X�E�_�C�A���O�̍쐬 �i�f�o�b�O���̂݁j
	//  ���[�h���X�E�_�C�A���O���쐬����
	// **********************************
	// (2002/05/11) �f�o�b�O���[�h�ŃG���[�̂��߁A�X���b�h�𗘗p���Ȃ�
	// (2002/05/14) �X���b�h�쐬���s���Ή� (g_dialog && h_ThreadHandle == NULL)
	if(g_debug || (g_dialog && h_ThreadHandle == NULL))
	{
		dlgDebugProgress = new CDlgProgress();
		if(dlgDebugProgress->Create() != TRUE)
		{	// �_�C�A���O���쐬�ł��Ȃ�
			::MessageBox(NULL, "Internal Error (Fail to Create Progress Dialog (Modeless))", "sTask (Warning)", MB_ICONWARNING|MB_OK|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
			delete dlgDebugProgress;
		}
		else debug_dlg_OK = TRUE;
	}


	bLastTaskExec = FALSE;		// �O��̃^�X�N���s��Ԃ̃��Z�b�g

	// **********************************
	//  �^�X�N�����Ɏ��s���肵�A���s����
	// **********************************
	for(i=0; i<maxtask; i++)
	{
		// **********************************
		//  �v���O���X�E�_�C�A���O�\���̃A�b�v�f�[�g
		// **********************************
		if(g_dialog && h_ThreadHandle != NULL)
		{
			strTmp2.LoadString(STR_EXEC_DLGPRGRESS);
			strTmp1.Format(strTmp2, tasks[i].name, i+1, maxtask);
			::SetWindowText(Thread_lfn, strTmp1);	// �_�C�A���O�ɒ��ڏ�������
			Thread_Global_m_progress = ((i+1)*100)/maxtask;
			::PostMessage(Thread_dialog, WM_PAINT, 0, 0);	// �ĕ`�惁�b�Z�[�W
		}
		// **********************************
		//  ���[�h���X�E�_�C�A���O�\���̃A�b�v�f�[�g �i�f�o�b�O���̂݁j
		// **********************************
		else if(debug_dlg_OK)
		{
			strTmp2.LoadString(STR_EXEC_DLGPRGRESS);
			strTmp1.Format(strTmp2, tasks[i].name, i+1, maxtask);
			dlgDebugProgress->SetDlgItemText(IDC_LFNAME, strTmp1);
			// �_�C�A���O�̃v���O���X�R���g���[��
			dlgDebugProgress->m_progress_ctrl.SetPos(((i+1)*100)/maxtask);
			// �_�C�A���O���A�b�v�f�[�g
			dlgDebugProgress->RedrawWindow();
			dlgDebugProgress->UpdateWindow();
		}

		if(!tasks[i].exec)
		{	// OFF�̏ꍇ
			if(g_dialog0)
			{	// ���[�U�[�ݒ�Œ�~�Ɛݒ肳�ꂽ����
				strTmp2.LoadString(STR_EXEC_STOP);
				strTmp1.Format(strTmp2, tasks[i].name);
				::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
			}
			continue;	// ���̃^�X�N��
		}	// OFF�̏ꍇ

		// ���ݎ����̐ݒ�i�f�o�b�O�Ή��j
		if(g_debug) Tnow = g_debugtime;
		else Tnow = CTime::GetCurrentTime();

		tasks[i].cnt_check++;	// �`�F�b�N�񐔂̃J�E���^�[


		// ���O�̃^�X�N�Ƃ̘A������
		// syncecec = 0 : ���̋@�\���g��Ȃ�
		// syncecec = 1 : ���O�̃^�X�N�F���s �� ���̃^�X�N�F�������s
		// syncecec = 2 : ���O�̃^�X�N�F�����s �� ���̃^�X�N�F�������s
		// syncecec = 3 : ���O�̃^�X�N�F���s �� ���̃^�X�N�F���������s
		// syncecec = 4 : ���O�̃^�X�N�F�����s �� ���̃^�X�N�F���������s

		if(tasks[i].syncexec == 3 && bLastTaskExec == TRUE && i != 0)
		{
			bLastTaskExec = FALSE;
			continue;		// �������~
		}
		if(tasks[i].syncexec == 4 && bLastTaskExec == FALSE && i != 0)
		{
			bLastTaskExec = FALSE;
			continue;		// �������~
		}

		if(((tasks[i].syncexec == 1 && bLastTaskExec == TRUE) || (tasks[i].syncexec == 2 && bLastTaskExec == FALSE)) && i != 0)
		{
			bLastTaskExec = FALSE;
			// �������s
		}
		else
		{	// �ʏ�ǂ������������s��
			bLastTaskExec = FALSE;

			Tnext = CalcNextTime(i);
			if(Tnext > Tnow)
			{	// ���������s�\�����ɒB���Ă��Ȃ��ꍇ
				if(g_dialog0)
				{
					strTmp2.LoadString(STR_NEXEC_NREACH);
					strTmp1.Format(strTmp2,
						tasks[i].name, Tnext.GetYear(), Tnext.GetMonth(), Tnext.GetDay(), Tnext.GetHour(), Tnext.GetMinute(), Tnext.GetSecond(),
						Tnow.GetYear(), Tnow.GetMonth(), Tnow.GetDay(), Tnow.GetHour(), Tnow.GetMinute(), Tnow.GetSecond());
					::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
				}
				continue;	// ���̃^�X�N��
			}	// �����`�F�b�N�I���
			if(tasks[i].excludetime)
			{	// ���ԏ��O������ꍇ
				long int t0,t1,tn;	// �P����b�P�ʂɒ����A�O��֌W���v�Z����
				t0 = tasks[i].time0.GetHour() * 3600 + tasks[i].time0.GetMinute() * 60 + tasks[i].time0.GetSecond();
				t1 = tasks[i].time1.GetHour() * 3600 + tasks[i].time1.GetMinute() * 60 + tasks[i].time1.GetSecond();
				tn = Tnow.GetHour() * 3600 + Tnow.GetMinute() * 60 + Tnow.GetSecond();
				if((t0 <= tn) && (tn <= t1))
				{	// ��������̌v�Z (t0 <= tn <= t1)
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_EXCLUDETIME);
						strTmp1.Format(strTmp2,
							tasks[i].name, tasks[i].time0.GetHour(), tasks[i].time0.GetMinute(), tasks[i].time0.GetSecond(),
							tasks[i].time1.GetHour(), tasks[i].time1.GetMinute(), tasks[i].time1.GetSecond(),
							Tnow.GetHour(), Tnow.GetMinute(), Tnow.GetSecond());
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;	// ���̃^�X�N��
				}
				else if((t1 < t0) && ((t0 <= tn) || (tn <= t1)))
				{	// ��������̌v�Z (t0 <= tn��0:00 <= t1)
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_EXCLUDETIME);
						strTmp1.Format(strTmp2,
							tasks[i].name, tasks[i].time0.GetHour(), tasks[i].time0.GetMinute(), tasks[i].time0.GetSecond(),
							tasks[i].time1.GetHour(), tasks[i].time1.GetMinute(), tasks[i].time1.GetSecond(),
							Tnow.GetHour(), Tnow.GetMinute(), Tnow.GetSecond());
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;	// ���̃^�X�N��
				}
			}	// ���ԏ��O�I���
			if(tasks[i].date)
			{	// �j�����O�̃`�F�b�N
				CString mes2 = "";
				if((Tnow.GetDayOfWeek() == 1) && (tasks[i].date & 0x40)) mes2.LoadString(STR_SUN);
				if((Tnow.GetDayOfWeek() == 2) && (tasks[i].date & 0x20)) mes2.LoadString(STR_MON);
				if((Tnow.GetDayOfWeek() == 3) && (tasks[i].date & 0x10)) mes2.LoadString(STR_TUE);
				if((Tnow.GetDayOfWeek() == 4) && (tasks[i].date & 0x08)) mes2.LoadString(STR_WED);
				if((Tnow.GetDayOfWeek() == 5) && (tasks[i].date & 0x04)) mes2.LoadString(STR_THU);
				if((Tnow.GetDayOfWeek() == 6) && (tasks[i].date & 0x02)) mes2.LoadString(STR_FRI);
				if((Tnow.GetDayOfWeek() == 7) && (tasks[i].date & 0x01)) mes2.LoadString(STR_SAT);
				
				if(mes2 != "")
				{
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_EXCLUDEDAY);
						strTmp1.Format(strTmp2,
							tasks[i].name, mes2);
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;	// ���̃^�X�N��
				}
			}	// �j�����O�I���
			if(tasks[i].apart == 0)
			{	// �N���񐔃J�E���g����
				if(tasks[i].cnt_check < tasks[i].interval)
				{
					if(g_dialog0)
					{
						strTmp2.LoadString(STR_NEXEC_COUNTER);
						strTmp1.Format(strTmp2,
							tasks[i].name, tasks[i].interval, tasks[i].cnt_check);
						::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
					}
					continue;
				}
			}

		}

		if(!ExecTask(i, hWnd))
		{	// �^�X�N�����s�ł��Ȃ�������
			if(g_faildlg)
			{	// �_�C�A���O��\������ �i�^�X�N�����s�ł��Ȃ������Ƃ��j
				strTmp2.LoadString(STR_NEXEC_EXECERR);
				strTmp1.Format(strTmp2,
					tasks[i].name);
					::MessageBox(hWnd, strTmp1, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL|MB_TOPMOST|MB_SETFOREGROUND);
			}
			bLastTaskExec = FALSE;	// (syncexec ����p�j
		}
		else bLastTaskExec = TRUE;	// �isyncexec ����p�j
	}

	if(g_dialog && h_ThreadHandle != NULL)
	{
		::Sleep(500);	// 500msec
		// **********************************
		//  �v���O���X�E�_�C�A���O���I��
		// **********************************
		::PostMessage(Thread_dialog, WM_CLOSE, 0, 0);
		// **********************************
		//  �E�C���h�E������ꂽ���Ƃ̃V�O�i����҂�  �i�R�b�Ń^�C���A�E�g�j
		// **********************************
		if(h_EventHandle != NULL)
		{
			::WaitForSingleObject(h_EventHandle, 3000);
			::ResetEvent(h_EventHandle);	// �C�x���g�̃��Z�b�g
		}
		::Sleep(500);	// 500msec
	}
	if(g_dialog && h_ThreadHandle != NULL)
	{
		// **********************************
		//  �X���b�h�̏����i�v���O���X�E�_�C�A���O�p�j
		// **********************************
		::CloseHandle(h_ThreadHandle);
	}
	// **********************************
	//  �X���b�h�ԒʐM�p �C�x���g�E�n���h���̊J��
	// **********************************
	if(h_EventHandle != NULL) ::CloseHandle(h_EventHandle);

	// **********************************
	//  ���[�h���X�E�_�C�A���O�̊J�� �i�f�o�b�O���̂݁j
	// **********************************
	if(debug_dlg_OK)
	{
		::Sleep(500);	// 500msec
		dlgDebugProgress->DestroyWindow();
		delete dlgDebugProgress;
	}

	return TRUE;
}	

