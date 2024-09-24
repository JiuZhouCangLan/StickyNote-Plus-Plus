#pragma once
#include "ExportConfig.hpp"
#include <QTextEdit>


namespace OriginUI {
	class  SimpleRichTextEditD;
	class ORIGIN_UI_EXPORT SimpleRichTextEdit :public QTextEdit{
		Q_OBJECT
	public:
		SimpleRichTextEdit(QWidget* parent = 0);
		~SimpleRichTextEdit();

	public:
		//��������
		void fontSizeUp();
		//��С����
		void fontSizeDown();
		//�����»���
		void setUnderLine(const bool& b);
		bool getUnderLine();
		//����ɾ����
		void setStrikOut(const bool& b);
		bool getStrikOut();
		//���þ���
		void setTextCenter(const bool& b);
		bool getTextCenter();
		//���þ���
		void setTextLeft();
		bool getTextLeft();
		//���þ���
		void setTextRight(const bool& b);
		bool getTextRight();
		//���õ�ǰ�ı�Ϊlist
		void setListFormat(QString format = "1. ");
		bool getListFormat();
		//�����Զ�ʶ���б�
		void setAutoList(const bool& b);

		//����������ɫ
		void setFontColor(const QColor& color);
		QColor getFontColor();

		//���ö��䱳��ɫ
		void setBackground(const QColor& color);
		//��ն��䱳��ɫ
		void clearBackground();
		//��ȡ���䱳��ɫ
		QColor getBackground();
		//�����Ƿ��ڵ��URL�Զ���ת������
		void setAutoOpenUrl(const bool& b);
		bool getAutoOpenUrl();
		//����б��
		void setItalic(const bool& b);
		bool getItalic();
		//���ô���
		void setFontBlod(const bool& b);
		bool getFontBlod();
		
		//��ȡ��ǰ���ı���ʽ�����߻�ȡ��ǰѡ�е��ı���ʽ
		QTextCharFormat getTextCharFormat();

	protected:
		void insertFromMimeData(const QMimeData* source) override;
	public Q_SLOTS:
		void inserChanged(int position, int charsRemoved, int charsAdded);
	private:
		void setFormatOnWordOrSelection(const QTextCharFormat& format);
		void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
		QTextCharFormat getFormatOnWordOrSelection();
		void openUrlFromCurrentCursor();

	protected:
		void mousePressEvent(QMouseEvent* e) override;
		void keyPressEvent(QKeyEvent* event) override;
	private:
		SimpleRichTextEditD* d;
	};

}