#if !defined(AFX_STASKDLG_H__A8C9BBC1_15EA_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKDLG_H__A8C9BBC1_15EA_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sTaskDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDebug �_�C�A���O

class CDlgDebug : public CDialog
{
// �R���X�g���N�V����
public:
	BOOL m_disable_cancel;
	CDlgDebug(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgDebug)
	enum { IDD = IDD_DEBUG };
	CButton	m_btn_cancel;
	CTime	m_date;
	CTime	m_tim;
	BOOL	m_dlg;
	BOOL	m_dlg0;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgDebug)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgPasswd �_�C�A���O

class CDlgPasswd : public CDialog
{
// �R���X�g���N�V����
public:
	CDlgPasswd(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgPasswd)
	enum { IDD = IDD_DLG_PASSWD };
	CString	m_passwd;
	CString	m_message;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgPasswd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgPasswd)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgInstall �_�C�A���O

class CDlgInstall : public CDialog
{
// �R���X�g���N�V����
public:
	BOOL m_IsAdminInst;
	CDlgInstall(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgInstall)
	enum { IDD = IDD_DLG_INST };
	CButton	m_r_autoexec_ctrl1;		// �蓮�Œǉ�
	CButton	m_r_autoexec_ctrl2;		// �蓮�Œǉ�
	CButton	m_r_autoexec_ctrl3;		// �蓮�Œǉ�
	CButton	m_r_autoexec_ctrl4;		// �蓮�Œǉ�
	BOOL	m_chk_uninst_cpl;
	BOOL	m_chk_uninst_start;
	int		m_r_autoexec;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgInstall)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgInstall)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_chk_protect;
	CString m_edit_password;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STASKDLG_H__A8C9BBC1_15EA_11D3_9DA0_0090CCA1066A__INCLUDED_)
