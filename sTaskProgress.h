#if !defined(AFX_STASKPROGRESS_H__0C843A60_1793_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKPROGRESS_H__0C843A60_1793_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sTaskProgress.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress �_�C�A���O

class CDlgProgress : public CDialog
{
// �R���X�g���N�V����
public:
	BOOL m_is_in_thread;
	int *m_progress_data;
	HWND * ptr_m_progress;
	HWND * ptr_m_dialog;
	HWND * ptr_m_lfn;
	HWND * ptr_m_mes;
	CDlgProgress(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DLG_NORMAL };
	CProgressCtrl	m_progress_ctrl;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgProgress)
	public:
	virtual BOOL Create(CWnd *wnd=NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STASKPROGRESS_H__0C843A60_1793_11D3_9DA0_0090CCA1066A__INCLUDED_)
