#include "stdafx.h"

#include <wincrypt.h>
#include <atlenc.h>

// �V���� DDV �̒�`
// �������� a ���� b �̊Ԃɂ��邱�Ƃ��m�F����
void AFXAPI DDV_MinMaxChars(CDataExchange* pDX, CString const& value, int minChars, int maxChars)
{
	ASSERT(minChars >= 1);        // allow them something
	ASSERT(maxChars >= 1);        // allow them something
	if (pDX->m_bSaveAndValidate && (value.GetLength() > maxChars || value.GetLength() < minChars))
	{	// �G���[�̕\��
		CString prompt;
		prompt.Format("%d����%d�����œ��͂��Ă�������", minChars, maxChars);
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		prompt.Empty(); // exception prep
		pDX->Fail();
	}

}


// �n�b�V�����v�Z����
// ����
//   pData : ���f�[�^�i�����񓙁j
//   dwdataLen : ���f�[�^�̃o�C�g��
// �o��
//  pcbHashData : �v�Z���ʂ̃n�b�V���l�i�o�C�i���j
//  dwHashLen : �n�b�V���l�̃o�C�g�� �iMD5��128bit�Ȃ̂�16�o�C�g�ASHA-1��160bit�Ȃ̂�20�o�C�g���w��j
// ����
//  dwFlag : CryptCreateHash�֐���ALG_ID�iCALG_MD5, CALG_SHA1�Ȃǂ��w��j
// �߂�l
//  �����̏ꍇ true
bool CalcHash_Raw(const void * pData, DWORD dwDataLen, BYTE *pcbHashData, DWORD dwHashLen, DWORD dwFlag)
{
	bool ret;
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	BYTE pbHash[0x7f];

	::ZeroMemory(pcbHashData, dwHashLen);
	if(pData == NULL || dwDataLen == 0)
		return false;

	hHash = NULL;
	hCryptProv = NULL;
	ret = false;
	if(::CryptAcquireContext(&hCryptProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		if(::CryptCreateHash(hCryptProv,dwFlag,0,0,&hHash))
		{
			if(::CryptHashData(hHash,(BYTE*)pData,dwDataLen,0))
			{
				if(::CryptGetHashParam(hHash,HP_HASHVAL,pbHash,&dwHashLen,0))
				{
					::memcpy(pcbHashData,pbHash,dwHashLen);
					ret = true;
				}
			}
		}
	}

	if(hHash)
		::CryptDestroyHash(hHash);
	if(hCryptProv)
		::CryptReleaseContext(hCryptProv,0);

	if(ret == false)
		::ZeroMemory(pcbHashData,dwHashLen);

	return ret;
}

// ������̃n�b�V�����v�Z���A������`���ŕԂ�
// ����
//  pStr : ������
//  mode : "MD5", "SHA1"
// �߂�l
//  �����̏ꍇ�n�b�V��������B���s�̏ꍇ ""�i�����[���̕�����j
char * CalcHash_String(const char * pStr, char * mode)
{
	BYTE pcbData[256];	// �n�b�V���v�Z�l���󂯎��iMD5�̏ꍇ128bit/8=16bytes, SHA1�̏ꍇ160bit/8=20bytes�K�v�j
	DWORD dwHashByte;
	DWORD dwFlag;
	static char strHash[256];	// �n�b�V���𕶎���ϊ��������́iMD5�̏ꍇ128bit/8*2=32bytes, SHA1�̏ꍇ160bit/8*2=40bytes�K�v�j

	strcpy(strHash, "");

	if(!_stricmp(mode, "MD5"))
	{
		dwHashByte = 16;
		dwFlag = CALG_MD5;
	}
	else if(!_stricmp(mode, "SHA1"))
	{
		dwHashByte = 20;
		dwFlag = CALG_SHA1;
	}
	else return(strHash);

	if(CalcHash_Raw(pStr, strlen(pStr), pcbData, dwHashByte, dwFlag) == true)
	{
		char strCh[3];
		for(int i = 0; i < (int)dwHashByte; i++)
		{
			sprintf(strCh, _T("%02x"), pcbData[i]);
			strcat(strHash, strCh);
		}
	}

	return(strHash);
}