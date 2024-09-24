#pragma once
#include <qwidget.h>
#include <qaction.h>
#include <QPoint>
#include <QTextCharFormat>
#include <map>
#include <QVBoxLayout>
#include <memory>
#include <QShortcut>
#include "WidgetNoBoder.h"
#include "NoteFile.h"
#include "ColorWidget.h"
#include "ActionCommand.h"
#include "StickyNoteContent.h"

class QToolButton;
namespace OriginUI {
	class ToolBar;
	class Action;
	class DrawerWidgetTraceMouse;
}
namespace NGui {
	class Pushpin;
	class NoteTextEdit;
	class ColorWidget;
	class StickyNoteD;
	class ActionCommand;
	class StickyNoteSlot;
	class StickyNoteContent;
	class StickyNote : public OriginUI::WidgetNoBoder{
		Q_OBJECT
			friend class StickyNotePool;
	public:
		StickyNote(std::shared_ptr<NF::StickyNote> note, StickyNoteContent* content,QWidget* parent = 0);
		virtual ~StickyNote();
	public:
		void loadNote(std::shared_ptr<NF::StickyNote> nt);
		void saveNote();
		void clearContent();
		std::shared_ptr<NF::StickyNote> getNFNote();
		StickyNoteContent* getContent();

		//������ɫ����
		void popColorSetting();
		void setNoteVisable(bool b);
		void connectSlot();
		void disConnectSlot();
		void show();
		void updateCmdState();
		//ˢ�¹����� ����ˢ�¹���������Щ���߻���ʾ
		void updateToolBar(std::vector<ActionCommandTypes>& cmdTypes);
		//��ʼ����ݼ�
		void initShortCut();
		//���ô���λ��
		void toDesk();
		void toTop();
		//���ò˵����Ƿ񵯳�
		bool getTopToolbarAutoPop();
		bool getBottomToolbarAutoPop();
		void setTopToolbarAutoPop(const bool& b);
		void setBottomToolbarAutoPop(const bool& b);

		bool getTopToolbarAutoPush();
		bool getBottomToolbarAutoPush();
		void setTopToolbarAutoPush(const bool& b);
		void setBottomToolbarAutoPush(const bool& b);

		//��ȡ���ڱ����ڵײ��״̬
		bool getOnlyBottom();
		void setOnlyBottom(const bool& b);

		void setAcitonCommandCheck(const std::string& cmd, bool check);
		void addActionCommand();
		void addActionCommand(std::vector<ActionCommandTypes> cmdTypes, OriginUI::ToolBar* bar);
		void addActionCommand(const std::string& cmd, OriginUI::ToolBar* bar, const bool& visible);

		//��ȡ�����õײ���������״̬
		void setBottomToolbarHide(const bool& b);
		bool getBottomToolbarHide();

		//��ȡ��ʵ��size() ����ڸ�dpiģʽ�±��Ŵ���Ȼ�᷵����ʵ�Ĵ�С
		QSize getRealSize();
		//��������
		virtual void loadSetting();
	private:
		void init();
	protected:
		void resizeEvent(QResizeEvent* event) override;
		void paintEvent(QPaintEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
		bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result)override;
#else
		bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
		/* ��ʼ���������Ҫ��һЩ����
		*  ��Щ����ֱ�������Ϸ��Ĺ��������·��Ĺ�����
		*  ��Щ������Ӻ󲢲���������ʾ�����������ڵ���addActionCommand()ʱ���Ż�ʵ����ӵ�������
		*  ��������ӵ�������֮ǰ�����������ж��������������Ƿ��������Ƿ���ʾ
		*  ���״̬Ϊ����ʾ����ôҲ������ʾ��������
		*  ��ʹû����ʾ����������������ӵ�����Ҳ�����ù���ݼ�����������п�ݼ��������
		*/
		void initActionCommandToTop(const ActionCommandTypes& cmd);
		void initActionCommandToBottom(const ActionCommandTypes& cmd);
		/* ���ڳ�ʼ�������������
		*/
		virtual void initActionCommand() = 0;
		//��ȡ���¹�����
		OriginUI::ToolBar* getNavigaitonBar();
		OriginUI::ToolBar* getBottomBar();
		//���ñ�����ݿؼ�
		StickyNoteContent* getStickyNoteContent();
		void setStickyNoteContent(StickyNoteContent* content);
	private:
		StickyNoteD* d;

	public Q_SLOTS:
		void navigaitonBarMouseMove(QPoint pos);
		void contentGotFocus();
		void contentLostFocus();
		void contentChange();
		void colorChange(QColor color);
		void toolbarLeftButtonRelease();
		void shortcutActive();
	};
}