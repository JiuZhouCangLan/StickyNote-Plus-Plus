#include "FindWindow.h"
#include <stdlib.h>
#include <malloc.h>


/**
* @brief enumWindowFindDesktopIconWindow  enumWindows�Ļص��������ڻص��в���ͼ��㴰��
* @param HWND hwnd  ���ھ��
* @param LPARAM lParam �ص�����
* @return BOOL  ����False ֹͣö��
*/
BOOL enumWindowFindDesktopIconWindow(HWND hwnd, LPARAM lParam)
{
	long wflags = GetWindowLong(hwnd, GWL_STYLE);
	if (!(wflags & WS_VISIBLE))
		return TRUE;
	HWND sndWnd;
	if (!(sndWnd = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL)))
		return TRUE;
	HWND targetWnd;
	if (!(targetWnd = FindWindowEx(sndWnd, NULL, "SysListView32", "FolderView")))
		return TRUE;
	HWND* resultHwnd = (HWND*)lParam;
	/*
	* �����е����������������������ͼ���֮�ϣ�����ֱ�ӽ��������ڶ���
	*/
	*resultHwnd = hwnd;
	return FALSE;
}


/**
* @brief findDesktopIconWindow ����ͼ���Ĵ���ID
* @return HWND
*/
HWND findDesktopIconWindow()
{
	HWND resultHwnd = NULL;
	EnumWindows((WNDENUMPROC)enumWindowFindDesktopIconWindow, (LPARAM)&resultHwnd);
	return resultHwnd;
}

typedef struct{
	HWND childID; //����ID
	BOOL isChild;

} IsChildPar;

BOOL enumWindowFindWindow(HWND winID, LPARAM par)
{
	IsChildPar* p = (IsChildPar*)par;

	if (winID == p->childID)
	{
		p->isChild = TRUE;
		return FALSE;
	}
	return TRUE;
};

BOOL isChild(HWND parent, HWND child)
{
	/*
	����δ֪ԭ��ʹ��GetParent()��IsChild()���޷���������
	����ֻ��ö�������Ӵ��ڣ�Ȼ��Ѱ�ҵ���Ӧ��child�ɴ���֤�Ƿ�Ϊ���ӹ�ϵ
	*/
	IsChildPar* par = malloc(sizeof(IsChildPar));

	par->childID = child;
	par->isChild = FALSE;

	EnumChildWindows(parent, enumWindowFindWindow, (LPARAM)par);

	BOOL rt = par->isChild;
	free(par);
	return rt;
}

WIN_API_EXPORT BOOL isDeskWindow(HWND hd)
{
	IsChildPar* par = malloc(sizeof(IsChildPar));

	par->childID = hd;
	par->isChild = FALSE;

	EnumWindows(enumWindowFindWindow, (LPARAM)par);

	BOOL rt = par->isChild;
	free(par);
	return rt;
}



BOOL enumWindowFindWallpaperLayer(HWND winID, LPARAM par)
{
	long wflags = GetWindowLong(winID, GWL_STYLE);
	if (!(wflags & WS_VISIBLE))
		return TRUE;
	HWND sndWnd;
	if (!(sndWnd = FindWindowEx(winID, NULL, "SHELLDLL_DefView", NULL)))
		return TRUE;
	HWND targetWnd;
	if (!(targetWnd = FindWindowEx(0, winID, L"WorkerW", 0)))
		return TRUE;
	HWND* resultHwnd = (HWND*)par;
	*resultHwnd = targetWnd;
	return FALSE;
}

HWND findWallpaperLayer()
{
	//Win10�ı�ֽ����������������Ϊ�˱����л���ֽʱ����˸
	//�ڶ����ֻֽ�����л���ֽ��ʱ����֣�������Ҫģ���л���ֽ����һ����Ϣ
	int result;
	HWND windowHandle = FindWindow(L"Progman", NULL);
	SendMessageTimeout(windowHandle, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, (PDWORD_PTR)&result);
	
	HWND wallpaper = NULL;
	EnumWindows(enumWindowFindWallpaperLayer, wallpaper);

	return wallpaper;
}
