#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <iostream>
#include <memory>
#include <QSystemTrayIcon>
#include "mainWindow.h"
#include <vector>
#include "ToolBar.h"
#include "StickyNote.h"
#include "cpp_log.h"
#include <qfile.h>
#include <QTextStream>
#include "WidgetNoBoder.h"
#include "NoteFile.h"
#include "SystemTrayIcon.h"
#include "StickyNotePool.h"
#include "ToolButtonColor.h"
#include "ColorWidget.h"
#include "ProcessCommunication.h"
#include "HistoryWidget.h"
#include "Setting.h"
#include "StickyNoteText.h"
#include "StickyNoteToDoList.h"
#include <QDir>
#include<QTranslator>
#include "circleSwitch.h"
extern "C"
{
#include "PCStartRun.h"
}
#include "WindowNoBoder.h"
#include "ImageSave.h"
#include "EditLable.h"
#include "TempTextEdit.h"
#include "CompatibilityTool.h"
#include "StickyNoteFactory.h"

//Ϊ�˷���ʹ��qt �����Թ��ܣ������������޸� 
#define QT_TRANSLATE_NOOP(scope, x) QCoreApplication::translate(scope,x)
int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // �ڴ���QCoreApplication����֮ǰ��������
	QApplication a(argc, argv);

	//ʵ����һ������ģ��Ķ��󣬲����뷭���ļ�
	QTranslator translator;
	auto  ok = translator.load(":/translate/translate/StickyNoteGui.qm");
	//ʹ��QApplictioin�Ķ���Ϊ����װ����ģ��
	a.installTranslator(&translator);

	//����Ϊ���һ�����ڹر�ʱ���˳�����
	a.setQuitOnLastWindowClosed(false);

	//����qss 
	QFile file(":/Style/qss/GuiStyle.qss");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	QString qss = stream.readAll();
	a.setStyleSheet(qss);
	file.close();


#ifndef _DEBUG
	Log::CppLog::GetInstance()->onAll();
#endif

#ifndef _DEBUG
	//��������ͨ��
	NGui::ProcessCommunication proC;
	proC.connetction();

	//�ж��Ƿ��н��������У������˳�����
	if (proC.getType() == NGui::CLIENT)
	{
		//���Է�����Ϣ
		proC.sendMessage("hello word!\n");
		return 0;
	}
#endif

	//ע���㴴����
	NGui::StickyNoteCreator::registerAllCreator();

	//����
	auto nt = NGui::StickyNotePool::CreatStickyNote("DiversityNote");
	nt->show();


	NGui::SystemTrayIcon trayIcon;
	trayIcon.show();

	//�ñ�־�ж��Ƿ�Ϊ��һ�δ򿪳���
	bool fierstOpen = false;
	//������ʷ������
	auto noteFile = NF::NoteFile::GetInstance();
	//�����ļ�·����userĿ¼
#ifndef _DEBUG
	CompatibilityTool::moveConfigFile();
	QDir dir;
	auto homePath = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath() + "/../data";
	dir.setPath(homePath);
	if (!dir.exists())
		dir.mkdir(homePath);
	noteFile->setFilePath(homePath.toStdString() + "/note.xml");

	NGui::Setting::setDefaultConfigPath(homePath.toStdString() + "/config.cfg");
	NGui::Setting setting;
	//��������ļ������ڣ��򱣴�һ�����ã����û�е��������Ӧ�ã��ͻ���ÿ������ʱ�ظ�����GUID������
	if (!setting.defaultPathExist())
	{
		
		fierstOpen = true;
		//���ó��򿪻�����
		trayIcon.actionAutoStartTrigger(true);
		//����Ĭ��ͼƬ����·��
		QDir d("../image");
		setting.imageSavePath->setValue(d.absolutePath().toStdString());
		setting.saveProperty();
		//��һ��ʹ�õ���ʹ����ʾ
		trayIcon.showMessage(QT_TRANSLATE_NOOP("main","Welcome to use sticky notes: "),
			QT_TRANSLATE_NOOP("main", "When using it, you can right-click on the tray icon to bring up the menu! \n\
Alternatively, you can left-click on the tray icon to display the history page! \n\
Both pages allow you to create new sticky notes or perform other actions! "), QSystemTrayIcon::Information, 20 * 1000);

	}
	NF::ImageSave::setDefaultFolder(QString::fromStdString(setting.imageSavePath->getValue()));
#endif // _DEBUG


	auto notes = noteFile->getStickyNote();

	for (auto note : notes)
	{
		if (note->visible == false)
			continue;
		auto st = NGui::StickyNotePool::CreatStickyNote(note);
		st->show();
	}

	auto history = NGui::HistoryWidget::Getinstance();

	for (auto note : notes)
	{
		history->addFileNote(note);
	}

	if (fierstOpen) {
	
		//��һ��ʹ�õ�������б�
#if 1
		{

			auto nt = NGui::StickyNotePool::CreatStickyNote("contentText");
			auto textGuiNote = dynamic_cast<NGui::StickyNoteText*>(nt);
			textGuiNote->setHtml(QT_TRANSLATE_NOOP("main", "First help!"));
			textGuiNote->show();

			nt = NGui::StickyNotePool::CreatStickyNote("contentToDoList");
			auto GuiNote = dynamic_cast<NGui::StickyNoteToDoList*>(nt);
			GuiNote->addItem(QT_TRANSLATE_NOOP("main", "Double-click the option to edit the content!"));
			GuiNote->addItem(QT_TRANSLATE_NOOP("main", "To delete the option, you need to click the Edit button at the bottom!"));
			GuiNote->addItem(QT_TRANSLATE_NOOP("main", "Click the plus button below to add the item!"));
			GuiNote->show();

			GuiNote->move(textGuiNote->x() + textGuiNote->width() + 10, textGuiNote->y());

		}
#endif
	
	}


	return a.exec();
}
