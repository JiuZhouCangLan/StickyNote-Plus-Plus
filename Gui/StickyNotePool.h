#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include "Setting.h"
#include "NoteFile.h"
namespace NF {
	class StickyNote;
}

namespace NGui {
	class StickyNote;
	class ActionCommandStateTimer;
	class StickyNoteMagnetic;
	class StickyNoteCreator;
	class StickyNotePool {
		friend class ActionCommandStateTimer;
	public:
		~StickyNotePool();
		StickyNotePool(const StickyNotePool&) = delete;
		StickyNotePool operator=(const StickyNotePool&) = delete;

		static std::shared_ptr<StickyNotePool> GetInstance() {
			static std::once_flag flag;
			std::call_once(flag, [&]() {
				instance.reset(new StickyNotePool());
				});

			return instance;
		}

	public:
		void StickyNoteCloseActionTirgger(StickyNote* note);
		void closeNoteNoSave(StickyNote* note);
		void closeStickyNote(const std::string& GUID);
		void hideStickyNote(const std::string& GUID);
		StickyNote* getStickyNote(const std::string& GUID);
		void closeAllNote();
		//�����Ƿ��Ѿ�����ʾ
		bool hasFileNote(const std::string& GUID);
		//��������
		void loadSetting();
		//ע�ᴴ����
		void registerStickyNoteCreator(StickyNoteCreator* creator);
	public:
		static StickyNote* CreatStickyNote(const std::string& type);
		static StickyNote* CreatStickyNote(std::shared_ptr<NF::StickyNote> nfNote);
		static void reloadSetting();
		static void magneticStickyNote(StickyNote* note);
		static void toDeskCenter(std::shared_ptr<NF::StickyNote> note);
	private:
		void initPos(StickyNote* note);
		void remove(StickyNote* note);
	private:
		StickyNotePool();
		static std::shared_ptr<StickyNotePool> instance;
		//��¼���б�㴰��
		std::vector<StickyNote*> pool;
		//������еı�㴴����
		std::map<std::string, StickyNoteCreator*>  stickyNoteCreators;
		//��ʱˢ�±�㴰�ڵİ�ť״̬
		ActionCommandStateTimer *timer;

		//��������
		bool stickyNoteToolMod;
		//�������Ƿ��Զ�����
		bool stickyNoteAutoPop;
		//�����Զ�������������
		StickyNoteMagnetic* magnetic;
	};
}