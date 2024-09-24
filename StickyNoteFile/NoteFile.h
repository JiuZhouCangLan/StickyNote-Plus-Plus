#pragma once
#include "ExportConfig.hpp"
#include "xml/pugixml.hpp"
#include "Property.h"
#include <memory>
#include <mutex>
#include <vector>
#include <list>
namespace NF {
	using namespace pugi;
	class NOTE_FILE_EXPORT StickyNote
	{
	public:
		enum State {
			NORMAL = 0, //��������
			PIN_ICON_LAYER, //�̶���ͼ���
			PIN_ALWAYS_TOP //�̶�����Ļ
		};
	public:
		StickyNote();
		~StickyNote() = default;
		bool operator==(const StickyNote& note);

		//��xml����������
		virtual void loadNode(xml_node& node);
		//�洢���ݵ�xml
		virtual void toNode(xml_node& node);
		//�洢���ݵ�xml�����뱣֤xml_node��һ����Ч��
		void save();
		//�������ݣ�������node
		StickyNote copy();
		//����guid
		static std::string creatGUID();
		//ɾ���ļ�
		void deleteFile();
		//��ȡ��ǰϵͳ�¼�
		std::string getTimes();
		//�Ƿ񱣴�
		bool isSaved();

	public:
		int posX,posY; //����λ��
		int width, height;//���ڴ�С
		std::string times; //����ʱ��
		xml_node node;	//�ļ��ڵ�
		bool visible;  //�Ƿ����
		State state;	//����״̬
		int backgroundColorIndex; //����ɫ����
		std::string backgroundColor; //������ɫ
		std::string customColor;	//�Զ�����ɫֵ
		bool contentIsNull;	//�����Ƿ�Ϊ��
		std::string guid; //����ʶ��
		double version;//�汾��
		PropertyString stickyNoteType;
	};

	class NOTE_FILE_EXPORT StickyNoteText :public StickyNote {
	public:
		StickyNoteText();
		//��xml����������
		virtual void loadNode(xml_node& node) override;
		//�洢���ݵ�xml
		virtual void toNode(xml_node& node) override;
		//��������
		static std::string getStickyNoteType();
	public:
		PropertyString contentText;
	};

	struct NOTE_FILE_EXPORT StickyNoteToDoListItem{
	public:
		StickyNoteToDoListItem();
		~StickyNoteToDoListItem() = default;
		bool load(xml_node& node);
		bool save(xml_node& node);
		static std::list<StickyNoteToDoListItem> loadAll(xml_node& node);
		PropertyString contentText,createDate,finishedDate;
		PropertyBool isFilished;
	};
	class NOTE_FILE_EXPORT StickyNoteToDoList :public StickyNote {
	public:
		StickyNoteToDoList();
		//��xml����������
		virtual void loadNode(xml_node& node) override;
		//�洢���ݵ�xml
		virtual void toNode(xml_node& node) override;
		//��������
		static std::string getStickyNoteType();
	public:
		std::list<StickyNoteToDoListItem> toDoListItems;
		//��ʾʱ�Ƿ�Ӧ�÷��� -> ��ʱֻ�а����Ƿ���ɷ���
		PropertyBool isSplit;

	};

	class NOTE_FILE_EXPORT NoteFile {
	public:
		~NoteFile();
		NoteFile(const NoteFile& file) = delete;
		NoteFile& operator=(const NoteFile&) = delete;
		static std::shared_ptr<NoteFile> GetInstance() {
			static std::once_flag flag;

			std::call_once(flag, [&]() {
				instance.reset(new NoteFile);
				});
			return instance;
		}
	private:
		NoteFile();

	public:
		std::vector<std::shared_ptr<StickyNote>> getStickyNote();
		xml_node creatNode();
		bool save();
		static std::shared_ptr<StickyNote> creatStickyNote(const std::string& stickyNoteType);
	public:
		//get set
		void setFilePath(const std::string& path);
		std::string getFilePath();
	private:
		static std::shared_ptr<NoteFile> instance;

		pugi::xml_document xml_doc;

		std::string filePath = "test.xml";
	};

}