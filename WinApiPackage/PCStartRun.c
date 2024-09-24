
#include "PCStartRun.h"
#include <windows.h>

//���õ�ǰ���򿪻�������
void AutoStart()
{
	HKEY hKey;
	//std::string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///��������       
	{
		//2���õ������������ȫ·��
		TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

		//3���ж�ע������Ƿ��Ѿ�����
		TCHAR strDir[MAX_PATH];
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, NULL, "StickyNote", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4���Ѿ�����
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			//5�����һ����Key,������ֵ��"GISRestart"��Ӧ�ó������֣����Ӻ�׺.exe�� 
			RegSetValueEx(hKey, "StickyNote", 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));

			//6���ر�ע���
			RegCloseKey(hKey);
		}
	}
}


//ȡ����ǰ���򿪻�����
void CanclePowerOn()
{
	HKEY hKey;
	//std::string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		//2��ɾ��ֵ
		RegDeleteValue(hKey, "StickyNote");

		//3���ر�ע���
		RegCloseKey(hKey);
	}
}

 BOOL IsAutoStart()
{
	HKEY hKey;
	//std::string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	//1���ҵ�ϵͳ��������  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///��������       
	{
		//2���õ������������ȫ·��
		TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

		//3���ж�ע������Ƿ��Ѿ�����
		TCHAR strDir[MAX_PATH];
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, NULL, "StickyNote", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4���Ѿ�����
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			return 0;
		}

	}

	return 1;
}
