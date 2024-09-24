#pragma once 
#include <vector>
#include <QWidget>
namespace NGui {
	class StickyNote;
	enum MagneticDirection {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3
	};

	class StickyNoteRect
	{
	public:
		StickyNoteRect() = default;
		StickyNoteRect(QWidget* w);
		int x;
		int y;
		int width;
		int height;
	};
	class StickyNotePositon {
	public:
		StickyNotePositon();
		bool operator>(const StickyNotePositon&);
		bool operator<(const StickyNotePositon&);
	public:
		MagneticDirection direction;
		double ditance;
		StickyNoteRect rect;
	};


	class StickyNoteMagnetic {
	public:
		StickyNoteMagnetic();
		~StickyNoteMagnetic();
	public:
		//����
		void magnetic(StickyNote* note, std::vector<StickyNote*> pool);

		//set get
		void setThreshold(const int& v);
		int getThreshold();

		void setEnable(const bool& e);
		bool getEnable();

		void setSpace(const int& s);
		int getSpace();

	private:
		//���������ƶ�����
		void startAnimation(StickyNote* note1, StickyNote* note2);
		void startAnimation(StickyNote* note, const StickyNoteRect& rect);
		//�ж����������Ƿ����ص�
		bool hasOverlap(StickyNote* note1, StickyNote* note2);
		bool hasOverlap(const StickyNoteRect& rc1,const StickyNoteRect& rc2);
		//��ȡ�ƶ����λ��
		StickyNoteRect moveResult(const StickyNoteRect& rect,const StickyNotePositon& magneticPos);
	private:
		//������������ֵ
		int  threshold;
		//�Ƿ����
		bool enable;
		//������϶
		int space;
	};
}