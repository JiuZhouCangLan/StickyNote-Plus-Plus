#pragma once
#include <QWidget>
#include <QColor>
#include <vector>
#include "drawerWidget.h"
namespace OriginUI {
	class ToolButtonColor;
}
namespace NGui {
	class ColorWidget :public QWidget {
		Q_OBJECT
	public:
		ColorWidget(QColor color = QColor(118, 118, 118), QWidget* parent = 0);
		~ColorWidget();
	public:
		int getCheckIndex();
		QColor getButtonColor(const int& index);
		void setCustomColor(const QColor& color);
		QColor getCustomColor();
		void setButtonCheck(const int& index);
	public Q_SLOTS:
		void buttonClicked(bool);
	Q_SIGNALS:
		void colorChange(QColor);

	private:
		OriginUI::ToolButtonColor* creatButton();
	private:
		std::vector<OriginUI::ToolButtonColor*> buttons;
		//Ĭ����ɫ��������������ʱ����
		QColor defaultColor;
	};

	class ColorDrawerWidget :public OriginUI::DrawerWidget {
	public:
		ColorDrawerWidget(QWidget* parent = 0);
		void autoSize(const QSize& parentSize);
	protected:
		void focusOutEvent(QFocusEvent* event) override;
	};
}