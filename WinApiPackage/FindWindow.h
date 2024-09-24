#ifndef __FIND_WINDOW_H__
#define __FIND_WINDOW_H__
#include "WinApiExport.h"
#include <windows.h>

//����ͼ���Ĵ��ھ��
WIN_API_EXPORT HWND findDesktopIconWindow();

//�жϴ����Ƿ�Ϊ���ӹ�ϵ
//�ƺ���㸸�ӹ�ϵҲ�᷵��TRUE  ��ʱ�޷����
WIN_API_EXPORT BOOL isChild(HWND parent, HWND child);

//�ж��Ƿ���һ�����洰��
WIN_API_EXPORT BOOL isDeskWindow(HWND hd);

//���ұ�ֽ��
WIN_API_EXPORT HWND findWallpaperLayer();

#endif
