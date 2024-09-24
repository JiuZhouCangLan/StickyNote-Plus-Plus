#pragma  once
#include "StickyNoteFile/NoteFile.h"
#include <memory>
namespace NGui {
	class StickyNote;
	class StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote ��Ҫ�����ع������Բ���Ҫ����ָ��
		*/
		virtual StickyNote* creatStickyNote() = 0;
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>) = 0;
		//��ȡ���ͣ����ڴ���ʱ����
		virtual std::string getType() = 0;
		//�����д�����ע�ᵽ������
		static void registerAllCreator();

	};
	class TextNoteCreator:public StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote ��Ҫ�����ع������Բ���Ҫ����ָ��
		*/
		virtual StickyNote* creatStickyNote();
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>);
		//��ȡ���ͣ����ڴ���ʱ����
		virtual std::string getType();

	};

	class ToDoListNoteCreator :public StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote ��Ҫ�����ع������Բ���Ҫ����ָ��
		*/
		virtual StickyNote* creatStickyNote();
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>);
		//��ȡ���ͣ����ڴ���ʱ����
		virtual std::string getType();

	};

	class DiversityNoteCreator :public StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote ��Ҫ�����ع������Բ���Ҫ����ָ��
		*/
		virtual StickyNote* creatStickyNote();
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>);
		//��ȡ���ͣ����ڴ���ʱ����
		virtual std::string getType();

	};
}