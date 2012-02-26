// sTaskProgress.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "sTask.h"
#include "sTaskProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress �_�C�A���O


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
	//}}AFX_DATA_INIT

	// **********************************
	//  �ʃX���b�h�p�̏����������邩�ǂ����̔���t���O
	// **********************************
	m_is_in_thread = FALSE;		// �W���́A���Ȃ��B
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PRGRESS, m_progress_ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress ���b�Z�[�W �n���h��

BOOL CDlgProgress::Create(CWnd *wnd) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	return CDialog::Create(IDD, wnd);
}

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	// **********************************
	//  �ʃX���b�h�p�̏�����
	// **********************************
	if(m_is_in_thread)
	{
		*this->ptr_m_dialog = this->m_hWnd;
		*this->ptr_m_lfn = GetDlgItem(IDC_LFNAME)->m_hWnd;
		*this->ptr_m_mes = GetDlgItem(IDC_MES)->m_hWnd;
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDlgProgress::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	// **********************************
	//  �ʃX���b�h�p�̃_�C�A���O�E�A�b�v�f�[�g
	//  *m_progress_data �Ƀf�[�^���Z�b�g���� WM_PAINT ���b�Z�[�W�𑗂��Ă�������
	// **********************************
	if(m_is_in_thread)
	{
		m_progress_ctrl.SetPos(*m_progress_data);
	}
	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���
}
