#ifndef __SET_WINDOW_STATE_H__
#define __SET_WINDOW_STATE_H__
#include "WinApiExport.h"
#include <windows.h>

//�����ڹ̶�������
WIN_API_EXPORT void setWindowAlwaysTop(HWND winID);
//�жϴ����Ƿ����ڶ���
WIN_API_EXPORT BOOL isWindowAlwaysTop(HWND winID);

//���ô���Ϊ��ͨ����
WIN_API_EXPORT void setWindowNormal(HWND winID);
//�жϴ����Ƿ�Ϊ��ͨ����
WIN_API_EXPORT BOOL isWindowNormal(HWND winID);

//����������ͼ���
WIN_API_EXPORT void setWindowToIconLayer(HWND winID);
//�жϴ����Ƿ���ͼ���
WIN_API_EXPORT BOOL isWindowToIconLayer(HWND winID);



#endif
