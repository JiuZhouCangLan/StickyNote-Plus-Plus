#pragma  once
#include "ExportConfig.hpp"
#include <QWidget>
#include <QPoint>
class QPropertyAnimation;
class QBoxLayout;
namespace OriginUI {
	class  DrawerWidgetD;
	/*
		һ�����е��������Ķ�������
	*/
	class ORIGIN_UI_EXPORT DrawerWidget :public QWidget {
		Q_OBJECT
	public:
		DrawerWidget(QWidget* parent = 0);
		virtual ~DrawerWidget();

		//���ö���ʱ��
		void setPopTime(unsigned int mec);
		void setPushTime(unsigned int mec);
		//����һ������widget
		void setCenterWidget(QWidget* widget);
		//��ʼ������λ��
		virtual void initPos();

		virtual void pop();  //��������
		virtual void push(); //�ջش���
	signals:
		void widgetMoveSignal(QPoint point);

	protected:
		//��ȡ������ʼλ��
		virtual QPoint getPopStartPos();
		virtual QPoint getPopEndPos();
		virtual QPoint getPushEndPos();
		virtual QPoint getPushStartPos();
	
		QPropertyAnimation* getPopAnimation();
		QPropertyAnimation* getPushAnimaiton();

		void paintEvent(QPaintEvent* event) override;

		//��ȡ���ڵ�λ�ñ仯�¼�
		void moveEvent(QMoveEvent* event) override;

	private:
		DrawerWidgetD* d;
	};
}

