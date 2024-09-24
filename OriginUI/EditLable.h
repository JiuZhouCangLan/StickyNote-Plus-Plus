#pragma  once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "ExportConfig.hpp"
namespace OriginUI {
	class EditLableD;
	//����һ�����˫��ʱ���Ա༭��lable
	class ORIGIN_UI_EXPORT EditLable :public QWidget {
		Q_OBJECT
	public:
		EditLable(QWidget* parnet = 0);
		~EditLable();

		void setText(const QString& text);
		QString getText() const;
	public:
		void showLabel();
		void showLineEdit();
	signals:
		void gotFocus();
		void lostFocus();
	public slots:
		void lableDoubleClicked();
		void editOkButtonClicked(bool);
		void editCancelButtonClicked(bool);
		void lineEditOk(bool);
		void editingFinished();
	private:
		EditLableD* d;
	};

	//һ�����Է������˫���¼���lable
	class ORIGIN_UI_EXPORT ClickedLable :public QLabel {
		Q_OBJECT
	public:
		ClickedLable(QWidget* parent = 0);
		~ClickedLable() = default;
	Q_SIGNALS:
		void mouseDoubleClick();
	protected:
		void mouseDoubleClickEvent(QMouseEvent* event);

	};

	//һ�����Է�������뿪�¼���lineEdit
	class ORIGIN_UI_EXPORT LineEditLeave :public QLineEdit {
		Q_OBJECT
	public:
		LineEditLeave(QWidget* parent = 0);
		~LineEditLeave() = default;
	protected:
		void leaveEvent(QEvent* event) override;
	signals:
		void mouseLeave();
	};

}