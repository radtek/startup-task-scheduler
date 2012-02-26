// sTaskView.h : CSTaskView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STASKVIEW_H__E19A286D_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_STASKVIEW_H__E19A286D_0616_11D3_9DA0_0090CCA1066A__INCLUDED_

#include "sTask.h"	// ClassView �ɂ���Ēǉ�����܂����B
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSTaskView : public CListView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CSTaskView();
	DECLARE_DYNCREATE(CSTaskView)

// �A�g���r���[�g
public:
	CSTaskDoc* GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSTaskView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CSTaskApp *theApp;
	void ChangeMenuGray(CCmdUI *pCmdUI);
	BOOL EditTask(UINT i);
	BOOL DisplayTasks();
	virtual ~CSTaskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	BOOL ChgViewStyle(void);
	DWORD m_ViewStyle;
	CImageList m_sIconList,m_lIconList;
	//{{AFX_MSG(CSTaskView)
	afx_msg void OnViewIcon();
	afx_msg void OnViewReport();
	afx_msg void OnTaskNew();
	afx_msg void OnUpdateViewIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewReport(CCmdUI* pCmdUI);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTaskCut();
	afx_msg void OnTaskUp();
	afx_msg void OnTaskDown();
	afx_msg void OnTaskOff();
	afx_msg void OnTaskConfig();
	afx_msg void OnWriteReg();
	afx_msg void OnTaskExec();
	afx_msg void OnConfig();
	afx_msg void OnDebug();
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateTaskCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskExec(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskConfig(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTaskNew(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // sTaskView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CSTaskDoc* CSTaskView::GetDocument()
   { return (CSTaskDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STASKVIEW_H__E19A286D_0616_11D3_9DA0_0090CCA1066A__INCLUDED_)
