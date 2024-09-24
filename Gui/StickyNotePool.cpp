#include "StickyNotePool.h"
#include "NoteFile.h"
#include "StickyNote.h"
#include "ActionCommandStateTimer.h"
#include "EventManager.h"
#include "StickyNoteContentChangeEvent.h"
#include "Setting.h"
#include "ToolBar.h"
#include "StickyNoteMagnetic.h"
#include "NoteTextEdit.h"
#include "StickyNoteFactory.h"
#include "LogTool.h"
#include "ToolBar.h"
#include "StickyNote.h"
#include <iostream>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QApplication>
std::shared_ptr<NGui::StickyNotePool> NGui::StickyNotePool::instance;

NGui::StickyNotePool::~StickyNotePool()
{
	for (auto ptr : pool)
		delete ptr;
	delete timer;
	delete magnetic;
	for (auto iter = stickyNoteCreators.begin(); iter != stickyNoteCreators.end(); iter++)
	{
		delete iter->second;
	}
	stickyNoteCreators.clear();
}

NGui::StickyNotePool::StickyNotePool()
{
	timer = new ActionCommandStateTimer();
	auto setting = Setting::GetInstance();
	stickyNoteToolMod = setting->stickyNoteToolMod->getValue();
	stickyNoteAutoPop = setting->stickyNoteToolbarAutoPop->getValue();
	magnetic = new StickyNoteMagnetic();

	loadSetting();
}

void NGui::StickyNotePool::StickyNoteCloseActionTirgger(StickyNote* note)
{
	remove(note);
	note->setNoteVisable(false);
	note->saveNote();
	note->close();
	note->deleteLater();
	EventManager::postEvent(new StickyNoteColseEvent(note->getNFNote()));
}

void NGui::StickyNotePool::closeNoteNoSave(StickyNote* note)
{
	remove(note);
	note->close();
	delete note;
}

void NGui::StickyNotePool::closeStickyNote(const std::string& GUID)
{
	for (auto note : pool)
	{
		if (note->getNFNote()->guid == GUID)
		{
			closeNoteNoSave(note);
			return;
		}
	}
}

void NGui::StickyNotePool::hideStickyNote(const std::string& GUID)
{
	for (auto note : pool)
	{
		if (note->getNFNote()->guid == GUID)
		{
			StickyNoteCloseActionTirgger(note);
			return;
		}
	}
}

NGui::StickyNote* NGui::StickyNotePool::getStickyNote(const std::string& GUID)
{
	for (auto note : pool)
	{
		if (note->getNFNote()->guid == GUID)
		{
			return note;
		}
	}
	return nullptr;
}

void NGui::StickyNotePool::closeAllNote()
{
	for (auto ptr : pool)
		ptr->deleteLater();
	pool.clear();
}

void NGui::StickyNotePool::remove(StickyNote* note)
{
	for (auto iter = pool.begin(); iter != pool.end(); iter++)
	{
		if ((*iter) != note)
			continue;
		pool.erase(iter);
		break;
	}
}

bool NGui::StickyNotePool::hasFileNote(const std::string& GUID)
{
	for (auto note : pool)
	{
		if (note->getNFNote()->guid == GUID)
			return true;
	}

	return false;
}

void NGui::StickyNotePool::loadSetting()
{
	auto setting = Setting::GetInstance();
	setting->loadProperty();
	magnetic->setEnable(setting->autoMagnetic->getValue());
	magnetic->setSpace(setting->magneticDistance->getValue());
	magnetic->setThreshold(setting->magneticThreshold->getValue());
}

void NGui::StickyNotePool::registerStickyNoteCreator(StickyNoteCreator* creator)
{
	stickyNoteCreators[creator->getType()] = creator;
}



NGui::StickyNote* NGui::StickyNotePool::CreatStickyNote(const std::string& type)
{
	auto &creators = GetInstance()->stickyNoteCreators;
	auto iter = creators.find(type);
	if (iter == creators.end())
		return nullptr;

	StickyNote* nt = iter->second->creatStickyNote();
	std::vector<StickyNote*>& pool = GetInstance()->pool;
	pool.push_back(nt);

	//��ʼ��������
	auto setting = Setting::GetInstance();
	std::vector<ActionCommandTypes> cmdTypes;
	for (auto iter = setting->cmdState.begin(); iter != setting->cmdState.end(); iter++)
	{
		ActionCommandTypes types(iter->first, iter->second->getValue());
		cmdTypes.push_back(types);
	}
	nt->updateToolBar(cmdTypes);
	//���ù�����������
	nt->getNavigaitonBar()->push();
	nt->getBottomBar()->push();

	EventManager::postEvent(new StickyNoteCreatEvent(nt->getNFNote()));
	return nt;
}

NGui::StickyNote* NGui::StickyNotePool::CreatStickyNote(std::shared_ptr<NF::StickyNote> nfNote)
{
	auto& creators = GetInstance()->stickyNoteCreators;
	auto iter = creators.find(nfNote->stickyNoteType.getValue());
	if (iter == creators.end())
		return nullptr;
	StickyNote* nt = iter->second->creatStickyNote(nfNote);
	std::vector<StickyNote*>& pool = GetInstance()->pool;
	pool.push_back(nt);

	//��ʼ��������
	auto setting = Setting::GetInstance();
	std::vector<ActionCommandTypes> cmdTypes;
	for (auto iter = setting->cmdState.begin(); iter != setting->cmdState.end(); iter++)
	{
		ActionCommandTypes types(iter->first, iter->second->getValue());
		cmdTypes.push_back(types);
	}
	nt->updateToolBar(cmdTypes);
	//���ù�����������
	nt->getNavigaitonBar()->push();
	nt->getBottomBar()->push();

	EventManager::postEvent(new StickyNoteCreatEvent(nt->getNFNote()));
	return nt;
}

void NGui::StickyNotePool::reloadSetting()
{
	//��������
	GetInstance()->loadSetting();
	//�������ñ������
	auto setting = Setting::GetInstance();
	auto pool = GetInstance()->pool;
	for (auto stickyNote : pool)
	{
		//��ȡԭ����״̬�����״̬һ����������
		bool toolMod = GetInstance()->stickyNoteToolMod;
		if (toolMod != setting->stickyNoteToolMod->getValue())
		{
			stickyNote->hide();
			stickyNote->setWindowFlag(Qt::Tool, setting->stickyNoteToolMod->getValue());
			stickyNote->show();
		}

		bool autoPop = GetInstance()->stickyNoteAutoPop;
		if (autoPop != setting->stickyNoteToolbarAutoPop->getValue())
		{
			stickyNote->setTopToolbarAutoPop(setting->stickyNoteToolbarAutoPop->getValue());
		}
		NoteTextEdit::getNoteTextEdit(stickyNote->getContent())->setAutoOpenUrl(setting->visitiUrl);
		stickyNote->update();
	}
	GetInstance()->stickyNoteToolMod = setting->stickyNoteToolMod->getValue();
	GetInstance()->stickyNoteAutoPop = setting->stickyNoteToolbarAutoPop->getValue();
}

void NGui::StickyNotePool::magneticStickyNote(StickyNote* note)
{
	StickyNoteMagnetic* magnetic = GetInstance()->magnetic;
	auto pool = GetInstance()->pool;
	magnetic->magnetic(note, pool);
}

/**
* @brief NGui::StickyNotePool::toDeskCenter  ������������Ļ������
* @param std::shared_ptr<NF::StickyNote> note
* @return void
*/
void NGui::StickyNotePool::toDeskCenter(std::shared_ptr<NF::StickyNote> note)
{
	QPoint desktopCenter;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QScreen* primaryScreen = QGuiApplication::primaryScreen();
	QRect screenGeometry = primaryScreen->geometry();
	desktopCenter = screenGeometry.center();
#else
	// ��ȡ�������
	QDesktopWidget* desktop = QApplication::desktop();
	// ��ȡ�������ĵ�����
	desktopCenter = desktop->availableGeometry().center();
#endif

	

	note->posX = desktopCenter.x() - note->width / 2;
	note->posY = desktopCenter.y() - note->height / 2;
}


/**
* @brief NGui::StickyNotePool::initPos  ��ʼ������λ�ã����ⴰ���ص�
* @param StickyNote * note
* @return void
*/
void NGui::StickyNotePool::initPos(StickyNote* note)
{
	//��ʱδ�����ι��λ���ظ�����
}

