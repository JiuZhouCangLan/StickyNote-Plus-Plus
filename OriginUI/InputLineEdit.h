#pragma once
#include "ExportConfig.hpp"
#include <QWidget>
#include <QLineEdit>
class QToolButton;

namespace OriginUI {

	//�ṩһ�����Է��ͽ���״̬�źŵ�lineEidt
	class InputLineEditFocus :public QLineEdit {
		Q_OBJECT
	Q_SIGNALS:
		void gotFocus();
		void lostFocus();
	protected:
		void focusInEvent(QFocusEvent* e);
		void focusOutEvent(QFocusEvent* e);

	};


	enum InputLineEditLayoutMod {
		BUTTON_LEFT = 0,		//��ť�����		
		BUTTON_RIGHT		//��ť���ұ�
	};

	class InputLineEditD;
	class ORIGIN_UI_EXPORT InputLineEdit:public QWidget {
		Q_OBJECT
	public:
		InputLineEdit(QWidget *parent = 0);
		~InputLineEdit();

		//���ò���mod
		void setButtonLaoutMod(const InputLineEditLayoutMod& mod);
		InputLineEditLayoutMod getButtonLaoutMod();

		//���ð�ťͼ��
		void setOkIcon(QIcon icon);
		void setCancelIcon(QIcon icon);

		//��ȡ��ťָ��
		QToolButton* getOkButton();
		QToolButton* getCancelButton();
		//��ȡ�༭��
		QLineEdit* getLineEdit();
		//�Ƿ��Զ���ʾȡ����ť
		void setAutoShowCancelButton(const bool& b);
		bool getAutoShowCancelButton();
	public Q_SLOTS:
		void textChanged(const QString& text);
	Q_SIGNALS:
		void gotFocus();
		void lostFocus();
	protected:
		void paintEvent(QPaintEvent* event);
		void keyPressEvent(QKeyEvent* event);
	private:
		InputLineEditD* d;

	};

}