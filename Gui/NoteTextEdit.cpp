#include "NoteTextEdit.h"
#include <QVBoxLayout>
#include <QFocusEvent>
#include <iostream>
#include <QToolButton>
#include <QIcon>
#include <QMimeData>
#include <QTextBlock>
#include <QDir>
#include <QFileInfo>
#include <sstream>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QScrollBar>
#include "StickyNote.h"
#include "DrawerWidgetReverse.h"
#include "ToolBar.h"
#include "ImageSave.h"
#include "pugixml.hpp"
#include "LogTool.h"

const int NULL_TIMER_ID = -1;

NGui::NoteTextEdit::NoteTextEdit(QWidget* parent /*= 0*/)
	:SimpleRichTextEdit(parent),autoImageSizeTimerID(NULL_TIMER_ID)
{
	initGUI();
}

NGui::NoteTextEdit::~NoteTextEdit()
{

}


void NGui::NoteTextEdit::initGUI()
{
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setAutoList(true);
	QFont font("Microsoft YaHei",13);
 	this->setFont(font);
	setPlaceholderText(tr("Write something, please ..."));
	connect(this, &NGui::NoteTextEdit::emitSetHtml, this, &NGui::NoteTextEdit::slotSetHtml,Qt::QueuedConnection);
}


void NGui::NoteTextEdit::stopTimer()
{
	if (autoImageSizeTimerID != NULL_TIMER_ID)
		killTimer(autoImageSizeTimerID);
	autoImageSizeTimerID = NULL_TIMER_ID;
}

void NGui::NoteTextEdit::loadNote(const std::shared_ptr<NF::StickyNote> note)
{
	auto textDoc = document();
	auto textNode =  std::dynamic_pointer_cast<NF::StickyNoteText>(note);
	if (PtrIsNull(textNode))
		return;
 	textDoc->setHtml(QString::fromStdString(textNode->contentText.getValue()));
	setDocument(textDoc);
	
	//���ı�������ڶ���
	auto cursor = textCursor();
	cursor.setPosition(0);
	setTextCursor(cursor);

}

void NGui::NoteTextEdit::connectStickyNote(StickyNote* stickyNote)
{
	connect(this, SIGNAL(gotFocus()), stickyNote, SLOT(contentGotFocus()));
	connect(this, SIGNAL(lostFocus()), stickyNote, SLOT(contentLostFocus()));
	connect(this, SIGNAL(textChanged()), stickyNote, SLOT(contentChange()));
}

void NGui::NoteTextEdit::disConnectStickyNote(StickyNote* stickyNote)
{
	disconnect(this, SIGNAL(gotFocus()), stickyNote, SLOT(contentGotFocus()));
	disconnect(this, SIGNAL(lostFocus()), stickyNote, SLOT(contentLostFocus()));
	disconnect(this, SIGNAL(textChanged()), stickyNote, SLOT(contentChange()));
}

void NGui::NoteTextEdit::saveNote(const std::shared_ptr<NF::StickyNote> note)
{
	auto textNode = std::dynamic_pointer_cast<NF::StickyNoteText>(note);
	if (PtrIsNull(textNode))
		return ;
	textNode->contentText = toHtml().toStdString();
}

bool NGui::NoteTextEdit::isEmpty()
{
	return QTextEdit::toPlainText().isEmpty();
}

NGui::NoteTextEdit* NGui::NoteTextEdit::getNoteTextEdit(StickyNoteContent* content)
{
	return dynamic_cast<NoteTextEdit*>(content);
}

void NGui::NoteTextEdit::focusInEvent(QFocusEvent* e)
{
	QTextEdit::focusInEvent(e);
	if (e->gotFocus())
		Q_EMIT gotFocus();
}

void NGui::NoteTextEdit::focusOutEvent(QFocusEvent* e)
{
	QTextEdit::focusOutEvent(e);
	if (e->lostFocus())
		Q_EMIT lostFocus();
}

void NGui::NoteTextEdit::insertFromMimeData(const QMimeData* source)
{
	if (source->hasImage())
	{
		QImage image = qvariant_cast<QImage>(source->imageData());
		insertImage(image);
		return;
	}
	if (source->hasUrls())
	{
		for (QUrl url : source->urls())
		{
			QFileInfo finfo(url.fileName());
			QString endName = finfo.suffix().toLower();

			if(endName != "jpg" && endName !="png"&& endName!="gif")
				continue;
			QString p = url.path();
			//��ק������·�����ж�����ַ�
			p = p.right(p.length() - 1);
			QImage image;
			image.load(p);
			insertImage(image);
		}
		return;
	}
	OriginUI::SimpleRichTextEdit::insertFromMimeData(source);
}

void NGui::NoteTextEdit::resizeEvent(QResizeEvent* e)
{
	OriginUI::SimpleRichTextEdit::resizeEvent(e);
	stopTimer();
	autoImageSizeTimerID = startTimer(100);
}

void NGui::NoteTextEdit::timerEvent(QTimerEvent* e)
{
	/*
	Ϊ�˱�����ڴ��ڴ�СʱƵ���ĵ����Զ����ڴ�С������
	����Ϊ100ms����һ�Ρ�
	*/
	if (e->timerId() == autoImageSizeTimerID)
	{
		QSize size(this->width(), this->height());
		/*
		* ����ʹ�ö��̣߳����ܻ��ڴ��ڹر�ʱ���̻߳������С�
		* ���ʱ�����źŻ������쳣��û����������ǰ��ʱ��ʹ�ö��߳�
		*/
		//QtConcurrent::run(this, &NGui::NoteTextEdit::autoImageSize, toHtml(),size);
		autoImageSize(toHtml(), size);
		stopTimer();
	}
}

void NGui::NoteTextEdit::buttonUpClicked(bool)
{
	fontSizeUp();
}

void NGui::NoteTextEdit::buttonLowerClicked(bool)
{
	fontSizeDown();
}


void NGui::NoteTextEdit::slotSetHtml(QString html)
{
	/*
	* ��Ϊ����ĵ������п��������û�����ʹ�õ�ʱ��
	* ����Ϊ�˱�֤�û�û�б������ĸо�����Ҫ����html֮�󣬹���봹ֱ��������λ�ò������仯
	* �������ı�֮ǰ���Ȼ�ȡ���͹����������ݣ��������֮���ٽ�ֵ���û�ȥ��
	*/
	auto cursorPos = textCursor().position();
	// ��ȡ��ֱ������
	QScrollBar* verticalScrollBar = this->verticalScrollBar();

	// ��ȡ�������ĵ�ǰλ��
	int scrollPosition = verticalScrollBar->value();
	setHtml(html);
	auto cursor = textCursor();
	cursor.setPosition(cursorPos);
	setTextCursor(cursor);
	verticalScrollBar->setValue(scrollPosition);
}


void NGui::NoteTextEdit::autoImageSize(const QString& ht, const QSize& widgetSize)
{
	/*
	* ���ͼƬ���С�ڴ��ڿ�ȣ��򲻸ı��С��
	* ������ڴ��ڿ�ȣ��������С�봰��һ�£�Ȼ�󽫸߶Ȱ��տ�ȵ����ű����ȱ�����С��
	*/
	QString html = ht;
	pugi::xml_document doc;
	pugi::xml_document ndoc;
	pugi::xml_parse_result re = doc.load_string(html.toUtf8());
	if (!re)
		return;
	/*
		����ֻ����body�е����ݣ��������֮�������������û�ȥ����Ӷ���Ŀհ���
	*/
	{
		pugi::xml_node node = doc.child("html");
		if (node.empty())
			return;
		node = node.child("body");
		if (node.empty())
			return;
		loopXmlNode(node,widgetSize);
		ndoc.append_copy(node);
	}

	std::stringstream stream;
	ndoc.save(stream);
	html = QString::fromStdString(stream.str());
	//html = html.simplified();
	//���źŴ������ã���Ϊ����Ҫʹ�ö��̵߳���
	emitSetHtml(html);
}

QSize NGui::NoteTextEdit::getImageSize(const QSize& imageSize, const QSize& widgetSize)
{
	QSize size;
	const int marigin = 20;
	if (imageSize.width() < widgetSize.width())
	{
		size.setWidth(imageSize.width());
		size.setHeight(imageSize.height());
	}
	else {
		int w = widgetSize.width() - marigin;
		w = w < 150 ? 150 : w;
		int h = ((double)w / imageSize.width()) * imageSize.height();
		size.setWidth(w);
		size.setHeight(h);
	}
	return size;
}

void NGui::NoteTextEdit::setImageXmlNodeSize(pugi::xml_node node, const QSize& widgetSize)
{
	int w = node.attribute("width").as_int(0);
	int h = node.attribute("height").as_int(0);
	std::string path = node.attribute("src").as_string();
	QImage image;
	image.load(QString::fromStdString(path));

	if (image.isNull())
		return;
	QSize size(image.width(), image.height());
	size = getImageSize(size,widgetSize);
	node.attribute("width").set_value(size.width());
	node.attribute("height").set_value(size.height());
}

void NGui::NoteTextEdit::insertImage(const QImage& image)
{
	if (image.isNull())
		return;
	NF::ImageSave isave;
	if (!isave.save(image))
		return;
	QString path = isave.getPath();

	QTextImageFormat format;
	const int marigin = 20;
	auto size = getImageSize(QSize(image.width(), image.height()),QSize(this->width(),this->height()));
	format.setWidth(size.width());
	format.setHeight(size.height());
	format.setName(path);
	textCursor().insertText("\n");
	textCursor().insertImage(format);
	setAlignment(Qt::AlignCenter);
	textCursor().insertText("\n");
	setAlignment(Qt::AlignLeft);
}

void NGui::NoteTextEdit::loopXmlNode(pugi::xml_node node, const QSize& widgetSize)
{
	if (node.empty())
		return;
	std::string name(node.name());
	if (name == "img")
	{
		setImageXmlNodeSize(node,widgetSize);
	}
	for (auto iter = node.begin(); iter != node.end(); iter++)
	{
		loopXmlNode(*iter,widgetSize);
	}
}

