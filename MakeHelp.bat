@echo off
REM -- �ŏ��� Microsoft Visual C++ �ō쐬���� resource.h ����}�b�v �t�@�C�����쐬���܂�
echo // MAKEHELP.BAT generated Help Map file.  Used by STASK.HPJ. >"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // �R�}���h (ID_* , IDM_*) >>"hlp\sTask.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // �v�����v�g (IDP_*) >>"hlp\sTask.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // ���\�[�X (IDR_*) >>"hlp\sTask.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // �_�C�A���O (IDD_*) >>"hlp\sTask.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\sTask.hm"
echo. >>"hlp\sTask.hm"
echo // �t���[�� �R���g���[�� (IDW_*) >>"hlp\sTask.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\sTask.hm"
REM -- �v���W�F�N�g STASK �̃w���v���쐬


echo Win32 ���� ̧�ق�����ޒ�
start /wait hcw /C /E /M "hlp\sTask.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\sTask.hlp" goto :Error
if not exist "hlp\sTask.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\sTask.hlp" Debug
if exist Debug\nul copy "hlp\sTask.cnt" Debug
if exist Release\nul copy "hlp\sTask.hlp" Release
if exist Release\nul copy "hlp\sTask.cnt" Release
echo.
goto :done

:Error
echo hlp\sTask.hpj(1) : error: �w���v�t�@�C�����쐬���鎞�ɖ�肪�������܂����B

:done
echo.
