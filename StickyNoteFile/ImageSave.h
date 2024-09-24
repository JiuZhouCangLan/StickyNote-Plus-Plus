#pragma once 
#include <QString>
#include <QImage>
#include "ExportConfig.hpp"
namespace NF {
	class NOTE_FILE_EXPORT ImageSave {
	public:
		ImageSave();
		~ImageSave();
	public:
		bool save(const QImage& image,QString endName = ".png");
		QString getPath();

	public:
		static void setDefaultFolder(const QString& folder);

	private:
		//����ļ����Ƿ���� ���������򴴽�
		void disposFolderExist();
		//��ȡ·�����ļ�������
		int  getFileCount();
		//����ļ��Ƿ��Ѿ�����
		bool disposFileExist(const QString& fileP);
	private:
		static QString defaultFolder;
		QString date;
		QString path;
		QString fileName;
	};
}