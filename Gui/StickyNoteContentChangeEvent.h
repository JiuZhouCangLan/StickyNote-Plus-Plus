#pragma once
#include <QEvent>
#include <QString>
#include <QColor>
#include "NoteFile.h"
namespace NGui {
	enum StickyNoteEventType {
		NONE = QEvent::Type::User,
		CONTENT_CHANGE,		//�ı��ı��¼�
		COLOR_CHANGE,		//��ɫ�ı�
		STICKY_NOTE_CREAT,	//��㱻����
		STICKY_NOTE_COLSE,	//��㱻�ر�
		STICKY_NOTE_DELETE	//���ɾ��
	};


	class StickyNoteEvent :public QEvent {
	public:
		StickyNoteEvent(const std::shared_ptr<NF::StickyNote> n,const StickyNoteEventType& type)
			:QEvent((QEvent::Type)type)
		{
			note = n;
		};
		std::shared_ptr<NF::StickyNote> note;

	};

	class StickyNoteContentChangeEvent:public StickyNoteEvent{
	public:
		StickyNoteContentChangeEvent(const std::shared_ptr<NF::StickyNote> n)
		:StickyNoteEvent(n,CONTENT_CHANGE){};
	};

	class StickyNoteColorChangeEvent :public StickyNoteEvent {
	public:
		StickyNoteColorChangeEvent(const std::shared_ptr<NF::StickyNote> n, const QColor& color)
		:StickyNoteEvent(n,COLOR_CHANGE),color(color){};
		QColor color;
	};

	class StickyNoteCreatEvent : public StickyNoteEvent {
	public:
		StickyNoteCreatEvent(const std::shared_ptr<NF::StickyNote> n) 
		:StickyNoteEvent(n, STICKY_NOTE_CREAT){};
	};

	class StickyNoteColseEvent : public StickyNoteEvent {
	public:
		StickyNoteColseEvent(const std::shared_ptr<NF::StickyNote> n) 
		:StickyNoteEvent(n, STICKY_NOTE_COLSE){};
	};

	class StickyNoteDeleteEvent : public StickyNoteEvent {
	public:
		StickyNoteDeleteEvent(const std::shared_ptr<NF::StickyNote> n) 
		:StickyNoteEvent(n, STICKY_NOTE_DELETE) {};
	};
}