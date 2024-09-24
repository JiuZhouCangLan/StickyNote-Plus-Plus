#pragma  once
#include <QListWidget>
#include <QObject>
#include "StickyNoteContent.h"
#include <qlistwidget.h>
#include <QList>
namespace NF {
	class StickyNoteToDoListItem;
}
namespace NGui {
	class ToDoListContentD;
	class ToDoListContent :public QListWidget,public StickyNoteContent{
		Q_OBJECT
	public:
		ToDoListContent(QWidget* parent = 0);
		~ToDoListContent();

		//�����Ƿ��ڱ༭״̬
		void setEditState(const bool& state);
		bool getEditState();
	public:
		virtual  void loadNote(const std::shared_ptr<NF::StickyNote> note) override;
		virtual  void saveNote(const std::shared_ptr<NF::StickyNote> note) override;
		virtual  void connectStickyNote(StickyNote* stickyNote) override;
		virtual	 void disConnectStickyNote(StickyNote* stickyNote) override;
		virtual  bool isEmpty() override;
		virtual  void initGui(StickyNote* stickyNote) override;

	protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event);
	public:
		void addToDoListItem();
		void addToDoListItem(NF::StickyNoteToDoListItem& data);
		void addToDoListItem(const QString& text);
		//��ȡitem������
		int getToDoListCount();
		//��ȡitem�Ĵ�С
		QSize getToDoListSize();
		//��ȡ�ı�
		QList<QString> getToDOListText();
		//��ȡ ���� ������ʾ��״̬ 
		void setSplit(const bool& b);
		bool getSplit();
	public slots:
		void deleteItem();
		void addItemButtonClicked();
	Q_SIGNALS:
		void lostFocus();
		void gotFocus();
	private:
		//�����Ƿ���ɷ���
		void splitItem();
		//ȡ������
		void cancelSplitItem();
		//�Ƴ�����item �����ͷ��ڴ�
		void removeAllItem();
	private:
		ToDoListContentD* d;
	};
};