#pragma once
#include <QWidget>
#include "ExportConfig.hpp"
namespace OriginUI {
	class TempTextEditD;
	//�ṩһ����ʱ���ı��༭�����༭���ں���һ��ȷ����ȡ���İ�ť
	class ORIGIN_UI_EXPORT TempTextEdit :public QWidget{
		Q_OBJECT
	public:
		TempTextEdit(QWidget* parent = 0);
		~TempTextEdit();

	public:
		void setText(const QString& text);
		QString getText() const;
	public Q_SLOTS:
		void toolButtonClicked(bool);

	Q_SIGNALS:
		void okButtonClicked(bool);
		void cancelButtonClicked(bool);

	protected:
		void leaveEvent(QEvent* event);

	private:
		TempTextEditD* d;
	};
}
