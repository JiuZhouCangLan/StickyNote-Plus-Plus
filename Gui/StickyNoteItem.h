#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <memory>
#include <QPolygon>
#include <QTextEdit>
#include "WidgetNoBoder.h"
#include "NoteFile.h"
namespace NGui {

	class StickyNoteItemEventFilter :public QObject{
	protected:
		bool eventFilter(QObject* watched, QEvent* event) override;
	};

	class NoteTextEdit;
	class StickyNoteItemContent;
	class StickyNoteItem :public QWidget{
		Q_OBJECT
	public:
		StickyNoteItem(StickyNoteItemContent* ItemContent,QWidget* parent = 0);
		~StickyNoteItem();

		void setFileNote(const std::shared_ptr<NF::StickyNote> note);
		std::shared_ptr<NF::StickyNote> getFileNote();
		void loadNoteContent(const std::shared_ptr<NF::StickyNote> note);
		//���ñ���ɫ
		void setBackgroundColor(const QColor& c);
		//��ȡ���ı�
		QString getText();
	protected:
		void paintEvent(QPaintEvent* event);
		void mousePressEvent(QMouseEvent* event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event);
		void focusOutEvent(QFocusEvent* event);
		void resizeEvent(QResizeEvent* event);
		void showEvent(QShowEvent* event);
	private:
		void showNoteWidget();
		void closeNoteWidget();
		void deleteNote();
		void autoSize();
	public Q_SLOTS:
		void actionTrigger(bool);
	Q_SIGNALS:
		void deleteItem();
		//֪ͨQListItem�ı��С
		void listItemResize(QSize size);
	private:
		QVBoxLayout *vLayout;
		QHBoxLayout *hLayout;
		StickyNoteItemContent* content;
		QAction* actionShowMenu,*actionDeleteNote,*actionCloseNote,*actionShowNote,*actionRestNote;
		QColor backColor;
		QMenu* menu;
		QLabel* timesLabel;
		StickyNoteItemEventFilter* filter;
		std::shared_ptr<NF::StickyNote> fileNote;
		//�����ͣЧ��
		bool isHover;
		QColor hoverColor;
		//���ڶ����
		QPolygon polygon,triangle;
	};
}