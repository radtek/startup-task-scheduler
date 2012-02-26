#include "stdafx.h"

// �V���� DDV �̒�`
// �������� a ���� b �̊Ԃɂ��邱�Ƃ��m�F����
void AFXAPI DDV_MinMaxChars(CDataExchange* pDX, CString const& value, int minChars, int maxChars)
{
	ASSERT(minChars >= 1);        // allow them something
	ASSERT(maxChars >= 1);        // allow them something
	if (pDX->m_bSaveAndValidate && (value.GetLength() > maxChars || value.GetLength() < minChars))
	{	// �G���[�̕\��
//		CString mes;
//		mes.Format("%s����%s", minChars, maxChars);
//		TCHAR szT[32];
//		wsprintf(szT, _T("%d"), minChars);
		CString prompt;
//		AfxFormatString1(prompt, AFX_IDP_PARSE_STRING_SIZE, szT);
		prompt.Format("%d����%d�����œ��͂��Ă�������", minChars, maxChars);
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		prompt.Empty(); // exception prep
		pDX->Fail();
	}
	else if (pDX->m_hWndLastControl != NULL && pDX->m_bEditLastControl)
	{
		// limit the control max-chars automatically
		::SendMessage(pDX->m_hWndLastControl, EM_LIMITTEXT, maxChars, 0);
	}
}

