// sTaskDoc.cpp : CSTaskDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "sTask.h"

#include "sTaskDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc

IMPLEMENT_DYNCREATE(CSTaskDoc, CDocument)

BEGIN_MESSAGE_MAP(CSTaskDoc, CDocument)
	//{{AFX_MSG_MAP(CSTaskDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc �N���X�̍\�z/����

CSTaskDoc::CSTaskDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CSTaskDoc::~CSTaskDoc()
{
}

BOOL CSTaskDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc �V���A���C�[�[�V����

void CSTaskDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc �N���X�̐f�f

#ifdef _DEBUG
void CSTaskDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSTaskDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTaskDoc �R�}���h
