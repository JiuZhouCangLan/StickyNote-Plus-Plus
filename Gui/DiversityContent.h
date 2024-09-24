#pragma once
#include <QListWidget>
#include <QObject>
#include "StickyNoteContent.h"
#include <qlistwidget.h>
#include <QList>
namespace NGui {
	class DiversityContent:public QListWidget, public StickyNoteContent {
		//Q_OBJECT
	public:
		DiversityContent(QWidget* parent = 0);
		~DiversityContent();
	public:
		//��html����������
		virtual  void loadNote(const std::shared_ptr<NF::StickyNote> note) override;
		//�������ݵ�html
		virtual  void saveNote(const std::shared_ptr<NF::StickyNote> note) override;
		//�������㴰�ڵ��źźͲ�
		virtual  void connectStickyNote(StickyNote* stickyNote) override;
		//ȡ����㴰�ڵ��źŵĲ�
		virtual	 void disConnectStickyNote(StickyNote* stickyNote) override;
		//�жϱ�������Ƿ�Ϊ��
		virtual  bool isEmpty() override;
		//��ʼ��ҳ�棬�������������һЩ�Ա�㴰�ڵ�����
		virtual  void initGui(StickyNote* stickyNote) override;

	};
};