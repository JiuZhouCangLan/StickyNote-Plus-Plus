#include "StickyNote.h"
#include "NoteTextEdit.h"
#include "ToolBar.h"
#include "pushpin.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QTextDocument>
#include <QApplication>
#include <qpainter.h>
#include <QVariant>
#include <iostream>
#include <QTextBlock>
#include <QMessageBox>
#include "drawerWidget.h"
#include "DrawerWidgetTraceMouse.h"
#include "DrawerWidgetReverse.h"
#include "StickyNotePool.h"
#include "ColorWidget.h"
#include "Action.h"
#include "StickyNoteSlot.h"
#include <vector>
#include "ActionCommand.h"
#include "HistoryWidget.h"
#include "StickyNoteContentChangeEvent.h"
#include "EventManager.h"
#include "Setting.h"


namespace NGui {

	class StickyNoteD {
	public:
		StickyNoteD();
		~StickyNoteD();

		void initActionCommand();
		void setBackgroundColor(const QColor& color);
		//�������¹������Ĵ�С����������С��С
		QSize getNodeMiniSize();

		QToolButton* creatToolButton();
		OriginUI::ToolBarDynamic *navigationBar;
		OriginUI::ToolBar * bottomToolbar; //������
		OriginUI::DrawerWidgetTraceMouse* traceMouse; //�������е�����ƶ�����
		ColorDrawerWidget* colorBar;	//��ɫ���õ�������
		ColorWidget* colorWidget;		//��ɫ���ô���
		//NoteTextEdit* noteTextEdit; // �༭��
		QVBoxLayout* layout;	//���ֶ���
		QWidget* contentWidget;	//���ر������
		StickyNoteContent* noteContent;	//�������


		StickyNoteSlot* noteSlot;	//cmd�ۺ���������
		using ActionCommands = std::map<std::string, ActionCommand*>;
		ActionCommands actions;	//action��
		std::shared_ptr<NF::StickyNote> note; //����

		//������ɫ
		QColor backgroundColor;
		std::vector<ActionCommandTypes> topCmd, bottomCmd; //������ť���ײ���ť
		//�Ƿ�ˢ��cmd״̬
		bool updateCmdState;
		//�����Ƿ����ڵײ�
		bool onlyBottom;
		//�����ݼ�
		std::map<QShortcut*, ActionCommand*> shortcuts;
		//��¼����������������ʾ��״̬
		bool stickyNoteToolMod;

	};

	StickyNoteD::StickyNoteD()
	{
		onlyBottom = false;
		updateCmdState = false;
		stickyNoteToolMod = false;
		//��ʼ������ɫ
		backgroundColor = QColor(255, 247, 209);

		//��ʼ���۴�����
		noteSlot = new StickyNoteSlot();

		//��ʼ��������
		traceMouse = new OriginUI::DrawerWidgetTraceMouse();
		traceMouse->setPopTime(200);
		traceMouse->setPushTime(1000);
		navigationBar = new OriginUI::ToolBarDynamic(traceMouse);
		navigationBar->setMaxHeight(40);
		navigationBar->setAutoPop(false);
		navigationBar->setAutoPush(false);

		auto drawerWidget = new OriginUI::DrawerWidgetReverse();
		drawerWidget->setObjectName("BottomDrawerWidget");

		bottomToolbar = new OriginUI::ToolBar(drawerWidget);
		bottomToolbar->setAutoPush(false);
		bottomToolbar->setAutoPop(false);
		bottomToolbar->setMinimumHeight(40);
		bottomToolbar->setAlignment(Qt::AlignLeft);
		bottomToolbar->setAutoPush(false);
		navigationBar->setAutoPop(false);

		//��ʼ������
		layout = new QVBoxLayout();
		layout->setSpacing(0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		layout->setContentsMargins(0, 0, 0, 0);
#else
		layout->setMargin(0);
#endif

		colorBar = new ColorDrawerWidget();
		colorWidget = new ColorWidget();
		colorBar->setCenterWidget(colorWidget);

		contentWidget = new QWidget();
		contentWidget->setLayout(new QVBoxLayout(contentWidget));
		contentWidget->layout()->setSpacing(0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		layout->setContentsMargins(0,0,0,0);
#else
		contentWidget->layout()->setMargin(0);
#endif
		
		noteContent = nullptr;

		layout->addWidget(navigationBar);
		layout->addWidget(contentWidget);
		layout->addWidget(bottomToolbar);

		initActionCommand();
	}

	StickyNoteD::~StickyNoteD()
	{
		delete layout;
		delete navigationBar;
		delete bottomToolbar;
		delete colorBar;
		delete noteContent;

		for (auto iter = actions.begin(); iter != actions.end(); iter++)
		{
			delete iter->second;
		}
		for (auto iter = shortcuts.begin(); iter != shortcuts.end(); iter++)
		{
			delete iter->first;
		}
	}

	void StickyNote::setAcitonCommandCheck(const std::string& cmd, bool check)
	{
		auto  iter = d->actions.find(cmd);
		if (iter == d->actions.end())
		{
			std::cerr << "StickyNoteD::setAcitonCommandCheck  not found cmd: " << cmd << std::endl;
			return;
		}
		iter->second->setChecked(check);
	}

	void StickyNoteD::initActionCommand()
	{

	}



	void StickyNote::addActionCommand(const std::string& cmd, OriginUI::ToolBar* bar, const bool& visible)
	{
		auto action = ActionCmdFactory::GetInstance()->creatActionCommand(cmd);
		if (!action)
			return;
		if (!action->isUsable(this))
			return;
		if (visible)
		{
			auto button = d->creatToolButton();
			button->setDefaultAction(action);
			bar->addWidget(button);
		}
		d->noteSlot->connectSlot(action);
		d->actions.insert(StickyNoteD::ActionCommands::value_type(cmd, action));
	}

	void StickyNote::addActionCommand(std::vector<ActionCommandTypes> cmdTypes, OriginUI::ToolBar* bar)
	{
		for (auto cmd : cmdTypes)
		{
			addActionCommand(cmd.cmd, bar,cmd.visible);
		}
	}

	void StickyNote::addActionCommand()
	{
		//�����֮ǰ��
		d->navigationBar->clear();
		d->bottomToolbar->clear();
		addActionCommand(d->topCmd, d->navigationBar);
		addActionCommand(d->bottomCmd,d->bottomToolbar);
	}

	void StickyNote::setBottomToolbarHide(const bool& b)
	{
		d->bottomToolbar->setHidden(b);
	}

	/**
	* @brief NGui::StickyNoteD::setBackgroundColor ����֮ǰ���͸���ȣ�����͸����Ϊ0 ���´����޷���ȡ����
	* @param const QColor & color
	* @return void
	*/
	void StickyNoteD::setBackgroundColor(const QColor& color)
	{
		if(color == backgroundColor)
			return;
		backgroundColor = color;
		if (backgroundColor.alpha() <= 0)
			backgroundColor.setAlpha(1);
		EventManager::postEvent(new StickyNoteColorChangeEvent(note,backgroundColor));
	}

	QSize StickyNoteD::getNodeMiniSize()
	{
		/*
		*	�ó���Ч���¹���������Ч���ߵĸ���
		*	����๤��Ϊ׼��������С���
		*/
		int topCount = 0, bottomCount = 0;

		for(auto cmd : topCmd)
		{
			if (!cmd.visible)
				continue;
			topCount++;
		}

		for(auto cmd: bottomCmd)
		{
			if (!cmd.visible)
				continue;
			bottomCount++;
		}

		int maxCount = topCount > bottomCount ? topCount : bottomCount;
		
		int miniWidth = maxCount * 40 + 60;


		return QSize(miniWidth, 100);
	}

	QToolButton* StickyNoteD::creatToolButton()
	{
		auto button = new QToolButton();
		button->setMinimumSize(QSize(40, 40));
		button->setIconSize(QSize(20, 20));
		return button;
	}

}

NGui::StickyNote::StickyNote(std::shared_ptr<NF::StickyNote> note, StickyNoteContent* content, QWidget* parent /*= 0*/)
	:OriginUI::WidgetNoBoder(parent)
{
	resize(note->width, note->height);
	d = new StickyNoteD();
	d->note = note;
	setStickyNoteContent(content);
	init();
	connectSlot();
	loadNote(note);
	loadSetting();
}

NGui::StickyNote::~StickyNote()
{
	delete d;

}

void NGui::StickyNote::loadNote(std::shared_ptr<NF::StickyNote> nt)
{
	auto note = nt;
	d->note = note;
	//��ʼ������
	d->noteContent->loadNote(nt);

	//��ʼ����С��λ��
	//���λ�ó�����Ļ��  ����Ϊ 0 0
	int miniX = 0, miniY = 0;
	note->posX = note->posX < miniX ? miniY : note->posX;
	note->posY = note->posY < miniX ? miniY : note->posY;
	move(note->posX, note->posY);

	//������ڹ�С�����
	int miniWidth = 100, miniHeight = 100;
	note->width = note->width < miniWidth ? miniWidth : note->width;
	note->height = note->height < miniHeight ? miniHeight : note->height;
	resize(note->width, note->height);

	//��ʼ��������ɫ
	auto str = QString::fromStdString(note->customColor);
	d->colorWidget->setCustomColor(QColor(str));
	auto bcolor = QColor(QString::fromStdString(note->backgroundColor));
	if (!bcolor.isValid())
	{
		d->setBackgroundColor(d->colorWidget->getButtonColor(note->backgroundColorIndex));
		note->backgroundColor = QVariant(d->backgroundColor).toString().toStdString();
	}
	else
		d->setBackgroundColor(bcolor);
	d->colorWidget->setButtonCheck(note->backgroundColorIndex);


	//��ʼ������״̬
	switch (note->state)
	{
	case NF::StickyNote::PIN_ALWAYS_TOP:
		Pushpin::toTop(this);
		break;
	case NF::StickyNote::PIN_ICON_LAYER:
		Pushpin::toDesktop(this);
		break;
	default:
		break;
	}

}


/**
* @brief NGui::StickyNote::saveNote �����ļ�
* @return void
*/
void NGui::StickyNote::saveNote()
{
	
	auto noteFile = NF::NoteFile::GetInstance();
	auto note = d->note;
	if (note->node.empty())
		note->node = noteFile->creatNode();

	if (d->noteContent->isEmpty())
		note->contentIsNull = true;
	else
		note->contentIsNull = false;
	
	note->posX = pos().x();
	note->posY = pos().y();
	note->width = width();
	note->height = height();

	if (Pushpin::isTop(this))
		note->state = NF::StickyNote::PIN_ALWAYS_TOP;
	else if (Pushpin::isDesktop(this))
		note->state = NF::StickyNote::PIN_ICON_LAYER;
	else if (Pushpin::isNormal(this))
		note->state = NF::StickyNote::NORMAL;

	note->backgroundColorIndex = d->colorWidget->getCheckIndex();
	note->customColor = QVariant(d->colorWidget->getCustomColor()).toString().toStdString();
	note->backgroundColor = QVariant(d->backgroundColor).toString().toStdString();
	d->noteContent->saveNote(note);

	note->save();
	if (!noteFile->save())
	{
		int ret = QMessageBox::warning(this, tr("StickyNote"),
			tr("Failed to save file for unknown reason! \n"
				"Please timely feedback to the author."),
			QMessageBox::Ok);
	}
}


/**
* @brief NGui::StickyNote::clearContent ��ձ�������
* @return void
*/
void NGui::StickyNote::clearContent()
{
	//d->noteTextEdit->clear();
}

std::shared_ptr<NF::StickyNote>  NGui::StickyNote::getNFNote()
{
	return d->note;
}

NGui::StickyNoteContent* NGui::StickyNote::getContent()
{
	return d->noteContent;
}

void NGui::StickyNote::popColorSetting()
{
	d->colorBar->pop();
	d->colorBar->setFocus();
	d->colorBar->raise();
	//��ֹ�ı��༭����ʧ����󵯻صײ�������
	d->bottomToolbar->pop();
}

void NGui::StickyNote::setNoteVisable(bool b)
{
	d->note->visible = b;
}

void NGui::StickyNote::connectSlot()
{
	connect(d->traceMouse, SIGNAL(mouseMove(QPoint)), this, SLOT(navigaitonBarMouseMove(QPoint)));
	connect(d->traceMouse, SIGNAL(leftButtonRelease()), this, SLOT(toolbarLeftButtonRelease()));
	setLayout(d->layout);
	if(d->noteContent)
		d->noteContent->connectStickyNote(this);

	//��ʼ����ɫ����
	connect(d->colorWidget, SIGNAL(colorChange(QColor)), this, SLOT(colorChange(QColor)));

	d->noteSlot->setStickyNote(this);
	d->colorBar->setParent(this);
}

void NGui::StickyNote::disConnectSlot()
{

	disconnect(d->traceMouse, SIGNAL(mouseMove(QPoint)), this, SLOT(navigaitonBarMouseMove(QPoint)));
	//��ʼ����ɫ����
	disconnect(d->colorWidget, SIGNAL(colorChange(QColor)), this, SLOT(colorChange(QColor)));
	if(d->noteContent)
		d->noteContent->disConnectStickyNote(this);

	for (auto iter = d->shortcuts.begin(); iter != d->shortcuts.end(); iter++)
	{
		disconnect(iter->first, &QShortcut::activated, this, &StickyNote::shortcutActive);
		delete iter->first;
	}
	d->shortcuts.clear();
}

void NGui::StickyNote::show()
{
	QWidget::show();
	raise(); //�����ڲ㼶���ڶ���
}

void NGui::StickyNote::updateCmdState()
{
	if (!d->updateCmdState)
		return;
	for (auto iter = d->actions.begin(); iter != d->actions.end(); iter++)
	{
		iter->second->updateState(this);
	}
}

void NGui::StickyNote::updateToolBar(std::vector<ActionCommandTypes>& cmdTypes)
{
	for (auto cmdType : cmdTypes)
	{
		//ͬ�����¹������е���Ϣ
		for (auto cmd = d->topCmd.begin(); cmd != d->topCmd.end(); cmd++)
		{
			if (cmd->cmd == cmdType.cmd)
			{
				cmd->visible = cmdType.visible;
				break;
			}
		}
		for (auto cmd = d->bottomCmd.begin();cmd != d->bottomCmd.end();cmd++)
		{
			if (cmd->cmd == cmdType.cmd)
			{
				cmd->visible = cmdType.visible;
				break;
			}
		}
	}

	addActionCommand();
	initShortCut();
	//���ݹ������Ĵ�С���ô��ڵ���С��С
	setMinimumSize(d->getNodeMiniSize());
}

void NGui::StickyNote::initShortCut()
{
	for (auto iter = d->actions.begin(); iter != d->actions.end(); iter++)
	{
		auto action = iter->second;
		//��ӿ�ݼ�
		QString key = action->getShortcut();
		if (key.isEmpty())
			continue;
		QShortcut* sh = new QShortcut(this);
		sh->setContext(Qt::WidgetWithChildrenShortcut);
		sh->setKey(QKeySequence(key));
		d->shortcuts[sh] = action;
		connect(sh, &QShortcut::activated, this, &StickyNote::shortcutActive);
	}
}

void NGui::StickyNote::toDesk()
{
	d->note->state = NF::StickyNote::PIN_ICON_LAYER;
	Pushpin::toDesktop(this);
}

void NGui::StickyNote::toTop()
{
	d->note->state = NF::StickyNote::PIN_ALWAYS_TOP;
	Pushpin::toTop(this);
}

bool NGui::StickyNote::getTopToolbarAutoPop()
{
	return d->navigationBar->getAutoPop();
}

bool NGui::StickyNote::getBottomToolbarAutoPop()
{
	return d->bottomToolbar->getAutoPop();
}

void NGui::StickyNote::setTopToolbarAutoPop(const bool& b)
{
	d->navigationBar->setAutoPop(b);
}

void NGui::StickyNote::setBottomToolbarAutoPop(const bool& b)
{
	d->bottomToolbar->setAutoPop(b);

}

bool NGui::StickyNote::getTopToolbarAutoPush()
{
	return d->navigationBar->getAutoPush();
}

bool NGui::StickyNote::getBottomToolbarAutoPush()
{
	return d->bottomToolbar->getAutoPush();
}

void NGui::StickyNote::setTopToolbarAutoPush(const bool& b)
{
	d->navigationBar->setAutoPush(b);
}

void NGui::StickyNote::setBottomToolbarAutoPush(const bool& b)
{
	d->navigationBar->setAutoPush(b);
}

bool NGui::StickyNote::getOnlyBottom()
{
	return d->onlyBottom;
}

void NGui::StickyNote::setOnlyBottom(const bool& b)
{
	d->onlyBottom = b;
	
#if 0
	//δ��ɵĹ���  ��Ϊ��ȡ������ȷ��Qt::Tool��״̬
	//�������ڵײ�ʱ��������������ʾ
	//�������������Ƿ�ѡ�����
	if (d->onlyBottom) {
		d->stickyNoteToolMod = windowFlags() & Qt::Tool;
		setWindowFlag(Qt::Tool, false);
		d->stickyNoteToolMod = windowFlags() & Qt::Tool;
		show();
	}
	else
	{
		setWindowFlag(Qt::Tool,d->stickyNoteToolMod);
		show();
	}
#endif

}

bool NGui::StickyNote::getBottomToolbarHide()
{
	return d->bottomToolbar->isHidden();
}

QSize NGui::StickyNote::getRealSize()
{
	return QSize();
}


void  NGui::StickyNote::loadSetting()
{
	auto setting = Setting::GetInstance();
	setWindowFlag(Qt::Tool, setting->stickyNoteToolMod->getValue());
	setTopToolbarAutoPop(setting->stickyNoteToolbarAutoPop->getValue());
}

void NGui::StickyNote::init()
{
	setAttribute(Qt::WA_TranslucentBackground);
	//setWindowFlag(Qt::Tool);
}

void NGui::StickyNote::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	d->colorBar->autoSize(this->size());
}

void NGui::StickyNote::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QBrush(d->backgroundColor));
	WidgetNoBoder::paintEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void NGui::StickyNote::enterEvent(QEnterEvent* event)
#else
void NGui::StickyNote::enterEvent(QEvent* event)
#endif
{
	QWidget::enterEvent(event);
	d->updateCmdState = true;
}

void NGui::StickyNote::leaveEvent(QEvent* event)
{
	QWidget::leaveEvent(event);
	//��ʱһ��
	QApplication::processEvents(QEventLoop::AllEvents, 1000);
	d->updateCmdState = false;
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool NGui::StickyNote::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool NGui::StickyNote::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	//�����Ҫ�������ڵײ�ʱ
	//������λ�÷����ı�ʱ�����������ô��ڵ�λ��
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
	MSG* msg = *static_cast<MSG**>(message);
#else
	MSG* msg = static_cast<MSG*>(message);
#endif
	switch (msg->message) {
	case WM_WINDOWPOSCHANGING:	//���ڵ�z��λ�÷����ı�
	{
		if (!d->onlyBottom)
			break;
		{
			WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(msg->lParam);
			if (wp->hwndInsertAfter != HWND_BOTTOM)
				lower();
		}
		break;
	}
	case WM_ACTIVATE:		//����ͨ����ݼ���������������
	{
		if (!d->onlyBottom)
			break;
		lower();
		break;
	}
	case  WM_ENTERSIZEMOVE: //���ڿ�ʼ��ק�ƶ�
	{
		break;
	}
	case WM_EXITSIZEMOVE:	//���ڽ�����ק�ƶ�
	{
		toolbarLeftButtonRelease();
		break;
	}
	default:
		break;
	}
	return OriginUI::WidgetNoBoder::nativeEvent(eventType, message, result);
}

void NGui::StickyNote::initActionCommandToTop(const ActionCommandTypes& cmd)
{
	d->topCmd.push_back(cmd);
}

void NGui::StickyNote::initActionCommandToBottom(const ActionCommandTypes& cmd)
{
	d->bottomCmd.push_back(cmd);
}

OriginUI::ToolBar* NGui::StickyNote::getNavigaitonBar()
{
	return d->navigationBar;
}

OriginUI::ToolBar* NGui::StickyNote::getBottomBar()
{
	return d->bottomToolbar;
}

NGui::StickyNoteContent* NGui::StickyNote::getStickyNoteContent()
{
	return d->noteContent;
}

void NGui::StickyNote::setStickyNoteContent(StickyNoteContent* content)
{
	if (d->noteContent == content)
		return;
	delete d->noteContent;
	d->noteContent = content;
	d->noteContent->connectStickyNote(this);
	d->noteContent->initGui(this);
	d->contentWidget->layout()->addWidget(dynamic_cast<QWidget*>(d->noteContent));
}

/**
* @brief NGui::StickyNote::navigaitonBarMouseMove ���浼�����ƶ�
* @param QPoint pos
* @return void
*/
void NGui::StickyNote::navigaitonBarMouseMove(QPoint pos)
{
	this->move(this->pos() + pos);
}

void NGui::StickyNote::contentGotFocus()
{
	d->navigationBar->pop();
	d->navigationBar->setAutoPush(false);
	d->bottomToolbar->pop();
	//����colorbar
	d->colorBar->push();
}

void NGui::StickyNote::contentLostFocus()
{
	d->navigationBar->push();
	d->navigationBar->setAutoPush(true);
	d->bottomToolbar->push();

	saveNote();
}


void NGui::StickyNote::contentChange()
{
	//���ı��ı��¼����͸���ʷ��¼����
	//����ı��������򲻷���
// 	auto cusor = d->noteTextEdit->textCursor();
// 	auto block = cusor.block();
// 	if (block.firstLineNumber() > 6)
// 		return;
	d->noteContent->saveNote(d->note);
	EventManager::postEvent(new StickyNoteContentChangeEvent(d->note));
}

void NGui::StickyNote::colorChange(QColor color)
{
	d->setBackgroundColor(color);
	update();
}

void NGui::StickyNote::toolbarLeftButtonRelease()
{
	StickyNotePool::magneticStickyNote(this);
}

void NGui::StickyNote::shortcutActive()
{
	QShortcut* shortcut = dynamic_cast<QShortcut*>(sender());
	if (!shortcut)
		return;
	auto iter = d->shortcuts.find(shortcut);
	if (iter == d->shortcuts.end())
		return;
	iter->second->active(this,!(iter->second->isChecked()));
}

