#pragma once
#include "drawerWidget.h"
namespace OriginUI {
	/*
		һ���ӵײ������ĵ�������
	*/
	class ORIGIN_UI_EXPORT DrawerWidgetReverse :public DrawerWidget{
	public:
		DrawerWidgetReverse(QWidget* parent = 0);
		~DrawerWidgetReverse();

	public:
		void initPos() override;

 	protected:
		QPoint getPopEndPos() override;
		QPoint getPushEndPos() override;

	};
}