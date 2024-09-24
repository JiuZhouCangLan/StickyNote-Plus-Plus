#pragma once
#include "StickyNoteFile/Setting.h"
#include <map>
#include <memory>
#include <mutex>
namespace NF{
	class PropertyBool;
	class PropertyString;
}
namespace NGui {
	class Setting :public NF::Setting {
	public:
		
		~Setting();

	public:
		//�����뱣���ļ����� ʹ��Ĭ��·��
		void loadProperty();
		void saveProperty();
	public:
		//����Ĭ�ϵ������ļ�·��
		static void setDefaultConfigPath(const std::string& path);
		static std::string getDefaultConfigPath();
		static bool defaultPathExist();
		static std::shared_ptr<Setting> GetInstance();
	private:
		void initProperty();
		Setting();
		
	
	public:
		//���������ڹ������Ƿ��Զ�����
		NF::PropertyBool* stickyNoteToolbarAutoPop;
		//�����������Ƿ���ʾ��������
		NF::PropertyBool* stickyNoteToolMod;
		//��ʷ���ڿ�ݼ�
		NF::PropertyString* historyWidgetShortcut;
		//���԰汾
		NF::PropertyDouble* ignoreVersion;
		//�Զ�������
		NF::PropertyBool* autoUpdate;
		//�û���GUID�����ڼ�¼�û��汾
		NF::PropertyString* userID;
		//�����Զ���������ֵ
		NF::PropertyInt* magneticThreshold;
		//�Ƿ񴥷���������
		NF::PropertyBool* autoMagnetic;
		//����������ļ��
		NF::PropertyInt* magneticDistance;
		//Ĭ������
		NF::PropertyString* stickyNoteFont;
		//Ĭ��ͼƬ�洢·��
		NF::PropertyString* imageSavePath;
		//����͸��
		NF::PropertyBool* transparentWidget;
		//�̶�������
		NF::PropertyBool* pinIconLayer;
		//Ĭ�ϵı��������ڵĿ����߶�
		NF::PropertyInt* stickyNoteWidth, * stickyNoteHeight;
		//���������Ĭ��״̬
		std::map<std::string, NF::PropertyBool*> cmdState;
		//�Ƿ��Զ�ʶ������
		NF::PropertyBool* visitiUrl;
	private:
		static std::string defaultPath;
		static std::shared_ptr<Setting> _instance;

	};
}