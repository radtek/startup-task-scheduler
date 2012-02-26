// sTaskView.cpp : CSTaskView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "sTask.h"

#include "sTaskDoc.h"
#include "sTaskView.h"
#include "sTaskProp.h"
#include "sTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTaskView

IMPLEMENT_DYNCREATE(CSTaskView, CListView)

BEGIN_MESSAGE_MAP(CSTaskView, CListView)
	//{{AFX_MSG_MAP(CSTaskView)
	ON_COMMAND(ID_VIEW_ICON, OnViewIcon)
	ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
	ON_COMMAND(ID_TASK_NEW, OnTaskNew)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ICON, OnUpdateViewIcon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REPORT, OnUpdateViewReport)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_TASK_CUT, OnTaskCut)
	ON_COMMAND(ID_TASK_UP, OnTaskUp)
	ON_COMMAND(ID_TASK_DOWN, OnTaskDown)
	ON_COMMAND(ID_TASK_OFF, OnTaskOff)
	ON_COMMAND(ID_TASK_CONFIG, OnTaskConfig)
	ON_COMMAND(ID_WRITEREG, OnWriteReg)
	ON_COMMAND(ID_TASK_EXEC, OnTaskExec)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_COMMAND(ID_DEBUG, OnDebug)
	ON_UPDATE_COMMAND_UI(ID_DEBUG, OnUpdateDebug)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_TASK_CUT, OnUpdateTaskCut)
	ON_UPDATE_COMMAND_UI(ID_TASK_EXEC, OnUpdateTaskExec)
	ON_UPDATE_COMMAND_UI(ID_TASK_OFF, OnUpdateTaskOff)
	ON_UPDATE_COMMAND_UI(ID_TASK_CONFIG, OnUpdateTaskConfig)
	ON_UPDATE_COMMAND_UI(ID_TASK_UP, OnUpdateTaskUp)
	ON_UPDATE_COMMAND_UI(ID_TASK_DOWN, OnUpdateTaskDown)
	ON_UPDATE_COMMAND_UI(ID_TASK_NEW, OnUpdateTaskNew)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTaskView �N���X�̍\�z/����

CSTaskView::CSTaskView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	theApp = (CSTaskApp *)AfxGetApp();
}

CSTaskView::~CSTaskView()
{
}

BOOL CSTaskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B
	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSTaskView �N���X�̕`��


/////////////////////////////////////////////////////////////////////////////
// CSTaskView �N���X�̐f�f

#ifdef _DEBUG
void CSTaskView::AssertValid() const
{
	CListView::AssertValid();
}

void CSTaskView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSTaskDoc* CSTaskView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSTaskDoc)));
	return (CSTaskDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTaskView �N���X�̃��b�Z�[�W �n���h��

void CSTaskView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	CListCtrl& ListCtrl = GetListCtrl();

	// �A�C�R���̈�̊m��
	m_sIconList.Create(16,16,ILC_COLORDDB,0,21);
	m_lIconList.Create(32,32,ILC_COLORDDB,0,21);
	m_sIconList.SetBkColor(::GetSysColor(COLOR_WINDOW));
	m_lIconList.SetBkColor(::GetSysColor(COLOR_WINDOW));

	DisplayTasks();

	m_ViewStyle = LVS_REPORT;
	ChgViewStyle();	
}

// �I�����ɂ́A���W�X�g�����A�b�v�f�[�g����
void CSTaskView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// ���炩���߃v���O�����������[�h���Ă����i���p���邽�߁j

	if(!theApp->m_tasks.g_debug)
	{	// �ύX���e�����W�X�g���ɕۑ�����
		theApp->RegWriteAll();
	}
	else
	{
		strTmp.LoadString(STR_MES_DEBUG_NOSAVE);	//�f�o�b�O���[�h�̂��߁A�ύX���e�͕ۑ�����܂���
		::MessageBox(NULL, strTmp, strAppName, MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
	}

	// 99.11.27 �ǉ� �C���[�W���X�g�̊J��
	BOOL i,j;
	// �A�C�R���̍폜�i���łɕ\������Ă���Ƃ��̂��߂̏����j
	j = m_lIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_lIconList.Remove(0);
	j=m_sIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_sIconList.Remove(0);
//	i = m_sIconList.DeleteImageList();
//	i = m_lIconList.DeleteImageList();
//	m_sIconList.DestroyWindow();
//	m_lIconList.DestroyWindow();

}


BOOL CSTaskView::ChgViewStyle()
{
	return ModifyStyle(LVS_TYPEMASK,m_ViewStyle|LVS_SINGLESEL & LVS_TYPEMASK);
}

void CSTaskView::OnViewIcon() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	m_ViewStyle = LVS_ICON;
	ChgViewStyle();	
}
void CSTaskView::OnUpdateViewIcon(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(m_ViewStyle==LVS_ICON ? 0:1);
	
}

void CSTaskView::OnViewReport() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	m_ViewStyle = LVS_REPORT;
	ChgViewStyle();	
}
void CSTaskView::OnUpdateViewReport(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(m_ViewStyle==LVS_REPORT ? 0:1);	
}

// ���X�g�R���g���[���i���C���E�C���h�E�j�Ƀ^�X�N�ꗗ��\������
BOOL CSTaskView::DisplayTasks()
{
	static int insert_col=0;
	int n = theApp->m_tasks.maxtask;	// 0 �̂Ƃ��͑��݂��Ȃ�
	int i,j;
	CListCtrl& ListCtrl = GetListCtrl();
	CString strTmp;

	// ���X�g�̍폜�i���łɕ\������Ă���Ƃ��̂��߂̏����j
	ListCtrl.DeleteAllItems();
	// �A�C�R���̍폜�i���łɕ\������Ă���Ƃ��̂��߂̏����j
	j = m_lIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_lIconList.Remove(0);
	j=m_sIconList.GetImageCount();
	for(i=0; i<j; i++)
		m_sIconList.Remove(0);

	// �A�C�R���̐ݒ�
	m_lIconList.Add(theApp->LoadIcon(IDR_XDOCICON));	// �^�X�N�̒ǉ��̃A�C�R��
	m_sIconList.Add(theApp->LoadIcon(IDR_XDOCICON));
	HICON hicon;
	for(i=0; i<n; i++)
	{
		hicon = ::ExtractIcon(AfxGetApp()->m_hInstance, theApp->m_tasks.tasks[i].fpass ,0);
		if((UINT)hicon == NULL || (UINT)hicon == 1)
		{	// �A�C�R���������Ȃ��t�@�C���̂Ƃ�
			m_lIconList.Add(theApp->LoadIcon(IDR_SCRIPT));	// �h�L�������g�̃A�C�R��
			m_sIconList.Add(theApp->LoadIcon(IDR_SCRIPT));
		}
		else
		{
			m_lIconList.Add(hicon);
			m_sIconList.Add(hicon);
		}
		// 99.11.27 GDI�G���[�̎匴�������@!!! �g���I������n���h���́A����J�����邱��
		::DestroyIcon(hicon);
	}
	//::DestroyIcon(hicon);
	// ���X�g�ɃA�C�R�������т���
	ListCtrl.SetImageList(&m_sIconList, LVSIL_SMALL);
	ListCtrl.SetImageList(&m_lIconList, LVSIL_NORMAL);

	// ��̑}��
	if(!insert_col)
	{
		strTmp.LoadString(STR_VIEW_TASK);	// �^�X�N��
		ListCtrl.InsertColumn(0,strTmp,LVCFMT_LEFT,180);
		strTmp.LoadString(STR_VIEW_SCHEDULE);	// �X�P�W���[��
		ListCtrl.InsertColumn(1,strTmp,LVCFMT_LEFT,300);
		strTmp.LoadString(STR_VIEW_PREVEXEC);	// �O��̎��s��
		ListCtrl.InsertColumn(2,strTmp,LVCFMT_LEFT,100);
		strTmp.LoadString(STR_VIEW_NEXTEXEC);	// ����̎��s�\��
		ListCtrl.InsertColumn(3,strTmp,LVCFMT_LEFT,100);
		strTmp.LoadString(STR_VIEW_STATUS);	// ���
		ListCtrl.InsertColumn(4,strTmp,LVCFMT_LEFT,50);
		insert_col = 1;
	}

	// �A�C�e���̑}��
	strTmp.LoadString(STR_VIEW_ADD_NEW_ITEM);	// �^�X�N�̒ǉ�
	ListCtrl.InsertItem(1, strTmp, 0);
	if(n<1) return TRUE;	// �A�C�e��������Ȃ��Ƃ�
	for(i=0; i<n; i++)
	{
		// ���ڎ��̂̒ǉ��i�P�s�ڂ͂P����n�܂邪�A�u�^�X�N�ǉ��v���΂��ĂQ����j
		ListCtrl.InsertItem(i+2,theApp->m_tasks.tasks[i].name,i+1);
		// ���ڂ̕t�������̒ǉ��i�P�s�ڂ͂O����n�܂�j
		CString str;
		char dummy[20];

		// �P���ڂ̕\��
		if(theApp->m_tasks.tasks[i].apart == 4)
		{
			strTmp.LoadString(STR_PER_RANDOM);	// �����_��
			str = strTmp;
		}
		else
		{
			str = _itoa(theApp->m_tasks.tasks[i].interval, dummy, 10);
			if(theApp->m_tasks.tasks[i].apart == 0)
			{
				strTmp.LoadString(STR_VIEW_AT_LOGON);	// ���O�I����
				str = strTmp;
				str += _itoa(theApp->m_tasks.tasks[i].interval, dummy, 10);
				strTmp.LoadString(STR_PER_LOGON);	// �񂲂�
				str += strTmp;
			}
			else if(theApp->m_tasks.tasks[i].apart == 1)
			{
				strTmp.LoadString(STR_PER_DAY);	// ������
				str += strTmp;
			}
			else if(theApp->m_tasks.tasks[i].apart == 2)
			{
				strTmp.LoadString(STR_PER_WEEK);	// �T����
				str += strTmp;
			}
			else if(theApp->m_tasks.tasks[i].apart == 3)
			{
				strTmp.LoadString(STR_PER_MONTH);	// ������
				str += strTmp;
			}
		}
		if(theApp->m_tasks.tasks[i].date)
		{
			str += " ";
			if(theApp->m_tasks.tasks[i].date & 0x40)
			{
				strTmp.LoadString(STR_SUN);	// ��
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x20)
			{
				strTmp.LoadString(STR_MON);	// ��
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x10)
			{
				strTmp.LoadString(STR_TUE);	// ��
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x08)
			{
				strTmp.LoadString(STR_WED);	// ��
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x04)
			{
				strTmp.LoadString(STR_THU);	// ��
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x02)
			{
				strTmp.LoadString(STR_FRI);	// ��
				str += strTmp;
			}
			if(theApp->m_tasks.tasks[i].date & 0x01)
			{
				strTmp.LoadString(STR_SAT);	// �y
				str += strTmp;
			}
			strTmp.LoadString(STR_EXCEPTION);	// ������
			str += strTmp;
		}
		if(theApp->m_tasks.tasks[i].excludetime)
		{
			str += " ";
			str += theApp->m_tasks.tasks[i].time0.Format("%#H:%M");
			str += " - ";
			str += theApp->m_tasks.tasks[i].time1.Format("%#H:%M");
			strTmp.LoadString(STR_EXCEPTION);	// ������
			str += strTmp;
		}
		if(theApp->m_tasks.tasks[i].waitexit)
		{
			strTmp.LoadString(STR_VIEW_WAITPREV);	// �ҋ@
			str = str + " " + strTmp;
		}
		if(theApp->m_tasks.tasks[i].syncprev)
		{
			strTmp.LoadString(STR_VIEW_LOOKPREV);	// ���O�ƘA��
			str = str + " " + strTmp;
		}
		ListCtrl.SetItemText(i+1,1,str);


		// �Q���ځu�O��̎��s���v�̕\��
		strTmp.LoadString(STR_VIEW_LIST_DATETIME);	// %m��%d�� %H:%M
		ListCtrl.SetItemText(i+1,2,
			theApp->m_tasks.tasks[i].time_lastexec.Format(strTmp));

		// �R���ځu����̎��s�\��v�̕\��
		if(!theApp->m_tasks.tasks[i].exec)
		{
			str = "";
		}
		else if(theApp->m_tasks.tasks[i].apart == 0)
		{
			strTmp.LoadString(STR_VIEW_AT_LOGON);	// ���O�I����
			str = strTmp;
		}
		else
		{
			strTmp.LoadString(STR_VIEW_LIST_DATETIME);	// %m��%d�� %H:%M
			str = theApp->m_tasks.CalcNextTime(i).Format(strTmp);
		}
		ListCtrl.SetItemText(i+1,3,str);

		// �S���ځu��ԁv�̃A�C�e���̕\��
		if(!theApp->m_tasks.tasks[i].exec)
		{
			strTmp.LoadString(STR_VIEW_STOP_STATE);	// ��~��
			ListCtrl.SetItemText(i+1,4,strTmp);
		}
	}
	return TRUE;
}

// �V�����^�X�N�̒ǉ�
void CSTaskView::OnTaskNew() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp;
	CPropAdd0 prop0;
	CPropAdd1 prop1;
	CPropAdd2 prop2;
	CPropAdd4 prop4;
	CPropAdd5 prop5;	// �ŏI�y�[�W�iprop4 �̕ύX���e�𔽉f���邽�߂̃_�~�[�y�[�W)

	strTmp.LoadString(STR_TASK_NEW_SHORT);	// �^�X�N�̋@�\�ݒ�
	CPropertySheet DlgAdd(strTmp, this);
	DlgAdd.AddPage(&prop0);
	DlgAdd.AddPage(&prop1);
	DlgAdd.AddPage(&prop2);
	DlgAdd.AddPage(&prop4);
	DlgAdd.AddPage(&prop5);		// �Ō�̃y�[�W�ł� SetWizardButtons �Łu�����v�\������
	// �E�C�U�[�h�`���ɂ���
	DlgAdd.m_psh.dwFlags |= PSH_WIZARD;
	// �u�K�p�v�{�^����\�����Ȃ�
	DlgAdd.m_psh.dwFlags=(DlgAdd.m_psh.dwFlags|PSH_NOAPPLYNOW);

	// �W���I�ȓ��e�̐ݒ�
	prop0.m_select = 0;
	prop1.is_new = TRUE;
	prop1.m_exec = TRUE;
	prop1.trn_recom = &prop0.m_recom;
	prop1.trn_winfile = &prop0.m_winfile;
	prop1.trn_sel = &prop0.m_select;
	prop2.m_apart = 2;
	prop2.m_interval = 1;
	prop2.m_time0 = CTime(1999,1,1,0,0,0);
	prop2.m_time1 = CTime(1999,1,1,0,0,0);
	prop2.trn_fpass = &prop1.m_fpass;		// �����񏉊��ݒ�p
	prop2.trn_name = &prop1.m_name;			// �����񏉊��ݒ�p
	prop4.m_waitexit = TRUE;
	prop4.m_waitsec = 0;
	prop4.trn_name = &prop1.m_name;			// �����񏉊��ݒ�p
	prop4.m_cmb_syncexec = 0;
	prop5.trn_fpass = &prop1.m_fpass;		// �����񏉊��ݒ�p
	prop5.trn_name = &prop1.m_name;			// �����񏉊��ݒ�p
	if(DlgAdd.DoModal() == ID_WIZFINISH)
	{
		if(prop1.m_name != "" && prop1.m_fpass !="")
		{	// �^�X�N���ƃt�@�C�������������݂�����V�K�^�X�N�Ƃ��ĔF�肷��
			UINT date = (prop2.m_dsun ? 0x40:0) | (prop2.m_dmon ? 0x20:0) | (prop2.m_dtue ? 0x10:0) |
					(prop2.m_dwed ? 0x08:0) | (prop2.m_dthu ? 0x04:0) | (prop2.m_dfri ? 0x02:0) | (prop2.m_dsat ? 0x01:0);
			theApp->m_tasks.NewTask(prop1.m_name, prop1.m_fpass, prop1.m_param ,prop1.m_mes,
					prop1.m_exec, prop2.m_apart, prop2.m_interval , date,
					prop2.m_time, prop2.m_time0, prop2.m_time1,
					prop4.m_waitexit, prop4.m_waitsec, prop4.m_taskoff, prop4.m_taskoffcount,
					prop4.m_dialog, prop4.m_syncprev, prop4.m_cmb_syncexec);
		}
		else
		{
			CString strTmp, strAppName;
			strAppName.LoadString(STR_APPNAME);	// �v���O������
			strTmp.LoadString(STR_ERR_NOADD_NOTASKNAME);	// �^�X�N���ƃ^�X�N�t�@�C�����w�肳��Ă��Ȃ��̂ŁA�V�K�o�^����܂���ł���
			::MessageBox(this->m_hWnd, strTmp, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL );
		}

	}
	ChgViewStyle();	
	DisplayTasks();
}

// �I�𒆂̃^�X�N����������
void CSTaskView::OnTaskCut() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i)
	{	// �ǂ���I������Ă��Ȃ�������
		return;
	}
	// i-2 : tasks[0] ... tasks[maxtask-1]
	theApp->m_tasks.DeleteTask(i-2);
	ChgViewStyle();	
	DisplayTasks();
}

// �^�X�N���_�u���N���b�N����ƁA�^�X�N�̋@�\�ݒ���s��
void CSTaskView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	*pResult = 0;	// ���^�[���@�H�H

	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i) return;	// �I���Ȃ�
	// �^�X�N�z��� 0 ����n�܂�B
	if(i==1)
	{
		if(theApp->m_tasks.maxtask > 20) return;
		OnTaskNew();	// �擪�́u�^�X�N�ǉ��v
		return;
	}
	i -= 2;	// �^�X�N�z����O����n�܂�悤�ɂ���

	EditTask(i);	// �^�X�N�̋@�\�ݒ�

}

// �I�𒆂̃^�X�N�̋@�\�ݒ�
void CSTaskView::OnTaskConfig() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i) return;	// �I���Ȃ�
	// �^�X�N�z��� 0 ����n�܂�B
	if(i==1)
	{
		OnTaskNew();	// �擪�́u�^�X�N�ǉ��v
		return;
	}
	i -= 2;	// �^�X�N�z����O����n�܂�悤�ɂ���
	EditTask(i);	
}


// �I�𒆂̃^�X�N�̋@�\�ݒ�
BOOL CSTaskView::EditTask(UINT i)
{
	// �_�C�A���O�̏���
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// �v���O������

	CPropAdd1 prop1;
	CPropAdd2 prop2;
	CPropAdd3 prop3;
	strTmp.LoadString(STR_TASK_CONFIG_SHORT);	// �^�X�N�̋@�\�ݒ�
	CPropertySheet DlgAdd(strTmp, this);
	DlgAdd.AddPage(&prop1);
	DlgAdd.AddPage(&prop2);
	DlgAdd.AddPage(&prop3);

	// �u�K�p�v�{�^����\�����Ȃ�
	DlgAdd.m_psh.dwFlags=(DlgAdd.m_psh.dwFlags|PSH_NOAPPLYNOW);

	// ���e�̐ݒ�
	prop1.m_name = theApp->m_tasks.tasks[i].name;
	prop1.m_fpass = theApp->m_tasks.tasks[i].fpass;
	prop1.m_param = theApp->m_tasks.tasks[i].param;
	prop1.m_mes = theApp->m_tasks.tasks[i].mes;
	prop1.m_exec = theApp->m_tasks.tasks[i].exec;
	prop2.m_apart = theApp->m_tasks.tasks[i].apart;
	prop2.m_interval = theApp->m_tasks.tasks[i].interval;
	if(theApp->m_tasks.tasks[i].date & 0x40) prop2.m_dsun = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x20) prop2.m_dmon = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x10) prop2.m_dtue = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x08) prop2.m_dwed = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x04) prop2.m_dthu = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x02) prop2.m_dfri = TRUE;
	if(theApp->m_tasks.tasks[i].date & 0x01) prop2.m_dsat = TRUE;
	prop2.m_time = theApp->m_tasks.tasks[i].excludetime;
	prop2.m_time0 = theApp->m_tasks.tasks[i].time0;
	prop2.m_time1 = theApp->m_tasks.tasks[i].time1;
	prop2.trn_fpass = &prop1.m_fpass;
	prop2.trn_name = &prop1.m_name;
	prop3.m_waitexit = theApp->m_tasks.tasks[i].waitexit;
	prop3.m_waitsec = theApp->m_tasks.tasks[i].waitsec;
	prop3.m_taskoff = theApp->m_tasks.tasks[i].taskoff;
	prop3.m_taskoffcount = theApp->m_tasks.tasks[i].taskoffcount;
	prop3.m_dialog = theApp->m_tasks.tasks[i].dialog;
	prop3.m_syncprev = theApp->m_tasks.tasks[i].syncprev;
	prop3.m_cmb_syncexec = theApp->m_tasks.tasks[i].syncexec;
	prop3.m_cnt_check = theApp->m_tasks.tasks[i].cnt_check;
	prop3.m_cnt_exec = theApp->m_tasks.tasks[i].cnt_exec;
	prop3.m_lastexec_date = theApp->m_tasks.tasks[i].time_lastexec;
	prop3.m_lastexec_time = theApp->m_tasks.tasks[i].time_lastexec;
	if(DlgAdd.DoModal() == TRUE)
	{
		if(prop1.m_name == "" || prop1.m_fpass =="")
		{
			strTmp.LoadString(STR_ERR_NOEDIT_NOTASKNAME);	// �^�X�N���ƃ^�X�N�t�@�C�����w�肳��Ă��Ȃ��̂ŁA�ύX����܂���ł���

			::MessageBox(this->m_hWnd, strTmp, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL );
			return FALSE;
		}
		theApp->m_tasks.tasks[i].name = prop1.m_name;
		theApp->m_tasks.tasks[i].fpass = prop1.m_fpass;
		theApp->m_tasks.tasks[i].param = prop1.m_param;
		theApp->m_tasks.tasks[i].mes = prop1.m_mes;
		theApp->m_tasks.tasks[i].exec = prop1.m_exec;
		theApp->m_tasks.tasks[i].apart = prop2.m_apart;
		theApp->m_tasks.tasks[i].interval = prop2.m_interval;
		theApp->m_tasks.tasks[i].date = (prop2.m_dsun ? 0x40:0) | (prop2.m_dmon ? 0x20:0) |
					(prop2.m_dtue ? 0x10:0) | (prop2.m_dwed ? 0x08:0) | (prop2.m_dthu ? 0x04:0) |
					(prop2.m_dfri ? 0x02:0) | (prop2.m_dsat ? 0x01:0);
		theApp->m_tasks.tasks[i].excludetime = prop2.m_time;
		theApp->m_tasks.tasks[i].time0 = prop2.m_time0;
		theApp->m_tasks.tasks[i].time1 = prop2.m_time1;
		theApp->m_tasks.tasks[i].waitexit = prop3.m_waitexit;
		theApp->m_tasks.tasks[i].waitsec = prop3.m_waitsec;
		theApp->m_tasks.tasks[i].taskoff = prop3.m_taskoff;
		theApp->m_tasks.tasks[i].taskoffcount = prop3.m_taskoffcount;
		theApp->m_tasks.tasks[i].dialog = prop3.m_dialog;
		theApp->m_tasks.tasks[i].syncprev = prop3.m_syncprev;
		theApp->m_tasks.tasks[i].syncexec = prop3.m_cmb_syncexec;
		theApp->m_tasks.tasks[i].cnt_check = prop3.m_cnt_check;
		theApp->m_tasks.tasks[i].cnt_exec = prop3.m_cnt_exec;
		theApp->m_tasks.tasks[i].time_lastexec = CTime(prop3.m_lastexec_date.GetYear(),
				prop3.m_lastexec_date.GetMonth(), prop3.m_lastexec_date.GetDay(),
				prop3.m_lastexec_time.GetHour(), prop3.m_lastexec_time.GetMinute(),
				prop3.m_lastexec_time.GetSecond());
		ChgViewStyle();	
		DisplayTasks();

	}
	return TRUE;
}

// �I�𒆂̃^�X�N�̎��s�������A�ЂƂ��
void CSTaskView::OnTaskUp() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(i<=2)
	{	// ���݂̃^�X�N����Ɉڂ��Ȃ�
		// 0:�I���Ȃ��A1: �u�^�X�N�ǉ��v�A2:�u��ԏ�̃^�X�N�v
		return;
	}
	theApp->m_tasks.SwapTask(i-2,i-3);
	ChgViewStyle();	
	DisplayTasks();

}

// �I�𒆂̃^�X�N�̎��s�������A�ЂƂ���
void CSTaskView::OnTaskDown() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(i==0) return;	// �I���Ȃ�
	if(i>theApp->m_tasks.maxtask)
	{	// ���݂̃^�X�N�����Ɉڂ��Ȃ�
		return;
	}
	theApp->m_tasks.SwapTask(i-2,i-1);
	ChgViewStyle();	
	DisplayTasks();
}

// �I�𒆂̃^�X�N���~���ɂ���B
void CSTaskView::OnTaskOff() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(!i)
	{	// �ǂ���I������Ă��Ȃ�������
		return;
	}
	i -= 2;	// �^�X�N�z����O����n�܂�悤�ɂ���
	theApp->m_tasks.tasks[i].exec = FALSE;
	ChgViewStyle();	
	DisplayTasks();
}

// ���W�X�g���ɕۑ�����i�����j
void CSTaskView::OnWriteReg() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString strAppName, strTmp;
	theApp->RegWriteAll();

	strAppName.LoadString(STR_APPNAME);	// �v���O������
	strTmp.LoadString(STR_MES_REGSAVED);
	::MessageBox(this->m_hWnd, strTmp, strAppName, MB_OK|MB_ICONINFORMATION);

}

// �I�𒆃^�X�N�����s����
void CSTaskView::OnTaskExec() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// �v���O������
	CString mes;
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();

	if(i<=1) return;	// �I���Ȃ�
	i -= 2;	// �^�X�N�z����O����n�܂�悤�ɂ���

	if(!theApp->m_tasks.ExecTask(i, this->m_hWnd))
	{
		if(theApp->m_tasks.g_faildlg)
		{	// �_�C�A���O��\������
			strTmp.LoadString(STR_NEXEC_EXECERR);	// �^�X�N �u%s�v �͎��s�ł��܂���ł���\n\n�t�@�C�������݂��Ȃ������Ă���\��������܂�
			mes.Format(strTmp,
				theApp->m_tasks.tasks[i].name);
			::MessageBox(this->m_hWnd, mes, strAppName, MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL );
		}
	}
	ChgViewStyle();	
	DisplayTasks();
}

// ���ݒ�i�S�ʁj
void CSTaskView::OnConfig() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp;
	CPropCnf1 prop1;
	CPropCnf2 prop2;
//	CPropCnf3 prop3;
	strTmp.LoadString(STR_CONFIG_SHORT);	// ���ݒ�
	CPropertySheet DlgAdd(strTmp, this);
	DlgAdd.AddPage(&prop1);
	DlgAdd.AddPage(&prop2);
//	DlgAdd.AddPage(&prop3);
	// �u�K�p�v�{�^����\�����Ȃ�
	DlgAdd.m_psh.dwFlags=(DlgAdd.m_psh.dwFlags|PSH_NOAPPLYNOW);

	prop1.m_delay = theApp->m_tasks.g_delay;
	prop1.m_dialog = theApp->m_tasks.g_dialog;
	prop1.m_dialog0 = theApp->m_tasks.g_dialog0;
	prop1.m_faildlg = theApp->m_tasks.g_faildlg;
	prop1.m_time = theApp->m_tasks.g_time;
	prop2.m_secure = theApp->m_tasks.g_secure;
	if(DlgAdd.DoModal() == TRUE)
	{
		theApp->m_tasks.g_delay = prop1.m_delay;
		theApp->m_tasks.g_dialog = prop1.m_dialog;
		theApp->m_tasks.g_dialog0 = prop1.m_dialog0;
		theApp->m_tasks.g_faildlg = prop1.m_faildlg;
		theApp->m_tasks.g_time = prop1.m_time;
		theApp->m_tasks.g_secure = prop2.m_secure;
	}
	ChgViewStyle();	
	DisplayTasks();
}

// �f�o�b�O���[�h
void CSTaskView::OnDebug() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString strTmp, strAppName;
	strAppName.LoadString(STR_APPNAME);	// �v���O������
	if(!theApp->m_tasks.g_debug)
	{
		CDlgDebug dlg;
		dlg.m_date = CTime::GetCurrentTime();
		dlg.m_tim = CTime::GetCurrentTime();
		dlg.m_dlg0 = theApp->m_tasks.g_dialog0;
		dlg.m_dlg = theApp->m_tasks.g_dialog;
		if(dlg.DoModal() == TRUE)
		{
			theApp->m_tasks.g_debug = TRUE;	// �f�o�b�O���[�h�ɓ���
			theApp->m_tasks.g_dialog0 = dlg.m_dlg0;
			theApp->m_tasks.g_dialog = dlg.m_dlg;
			theApp->m_tasks.g_debugtime = CTime(dlg.m_date.GetYear(),
				dlg.m_date.GetMonth(), dlg.m_date.GetDay(),
				dlg.m_tim.GetHour(), dlg.m_tim.GetMinute(),
				dlg.m_tim.GetSecond());
			theApp->m_tasks.ExecAllTask(this->m_hWnd);
			ChgViewStyle();	
			DisplayTasks();
		}
	}
	else
	{
		strTmp.LoadString(STR_MES_EXIT_DEBUG);	// ���݂܂ł̕ҏW���ʂ����Z�b�g���āA�f�o�b�O���[�h���甲���܂��� �H\n�u�������v�̏ꍇ�́A�Ăуf�o�b�O���s����܂�
		if(::MessageBox(this->m_hWnd, strTmp, strAppName, MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL) == IDYES)
		{
			theApp->m_tasks.g_debug = FALSE;	// �f�o�b�O���[�h���甲����
			theApp->RegReadAll();
			ChgViewStyle();	
			DisplayTasks();
		}
		else
		{
			CDlgDebug dlg;
			dlg.m_date = theApp->m_tasks.g_debugtime;
			dlg.m_tim = theApp->m_tasks.g_debugtime;
			dlg.m_dlg0 = theApp->m_tasks.g_dialog0;
			dlg.m_dlg = theApp->m_tasks.g_dialog;
			dlg.m_disable_cancel = TRUE;
			dlg.DoModal();
			theApp->m_tasks.g_dialog0 = dlg.m_dlg0;
			theApp->m_tasks.g_dialog = dlg.m_dlg;
			theApp->m_tasks.g_debugtime = CTime(dlg.m_date.GetYear(),
				dlg.m_date.GetMonth(), dlg.m_date.GetDay(),
				dlg.m_tim.GetHour(), dlg.m_tim.GetMinute(),
				dlg.m_tim.GetSecond());
			theApp->m_tasks.ExecAllTask(this->m_hWnd);
			ChgViewStyle();	
			DisplayTasks();
		}
	}
}

// ����ȍ~�A���j���[��{�^���̌������̏���

void CSTaskView::OnUpdateDebug(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	if(theApp->m_tasks.g_debug)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);

}

void CSTaskView::OnUpdateTaskCut(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	ChangeMenuGray(pCmdUI);	
}

void CSTaskView::ChangeMenuGray(CCmdUI *pCmdUI)
{
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();
	if(i>1) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);

}

void CSTaskView::OnUpdateTaskExec(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	ChangeMenuGray(pCmdUI);	
	
}

void CSTaskView::OnUpdateTaskOff(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	ChangeMenuGray(pCmdUI);	
	
}

void CSTaskView::OnUpdateTaskConfig(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	ChangeMenuGray(pCmdUI);	
	
}

void CSTaskView::OnUpdateTaskUp(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();
	if(i>2) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
	
}

void CSTaskView::OnUpdateTaskDown(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	CListCtrl& ListCtrl = GetListCtrl();
	UINT i = (UINT)ListCtrl.GetFirstSelectedItemPosition();
	if(i>1 && i<=theApp->m_tasks.maxtask) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CSTaskView::OnUpdateTaskNew(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

	// �^�X�N���Q�P��������A�����ǉ��ł��Ȃ��B
	if(theApp->m_tasks.maxtask > 20) pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CSTaskView::OnHelpFinder() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CSTaskApp *theApp;
	theApp = (CSTaskApp *)AfxGetApp();

	theApp->WinHelp(0);

	return ;
}
