#include "StickyNoteMagnetic.h"
#include "StickyNote.h"
#include <map>
#include <list>
#include <QRect>
#include <QPropertyAnimation>
NGui::StickyNoteMagnetic::StickyNoteMagnetic()
	:threshold(40),enable(true),space(5)
{

}

NGui::StickyNoteMagnetic::~StickyNoteMagnetic()
{

}

/**
* ���������жϣ�
* @brief NGui::StickyNoteMagnetic::magnetic
* @param StickyNote * note �ƶ�λ�õĴ���
* @param std::vector<StickyNote * > pool ���еı�㴰��
* @return void
*/
void NGui::StickyNoteMagnetic::magnetic(StickyNote* note, std::vector<StickyNote*> pool)
{
	/*
		[1] �ó��ô�������������֮��ľ��룬�������ֻ��������ֵ�����ľ���Ϊ��Ч���룬�����Ϊ��Ч����
		[2] ��¼��x�����y���е���Զ�ľ���
		[3] �ó���������Ĵ���
		[4] ���û��������ֵ�Ĵ��ڣ��򲻴�������
		[5] ʹ�þ�������Ĵ��ڴ�������
	*/

	//���Ϊ������״̬�������κβ���
	if (!enable)
		return;
	//��ȡ����λ�ô�С��Ϣ
	StickyNoteRect rect(note);

	std::list<StickyNotePositon> postions;
	for (int i = 0; i < pool.size(); i++)
	{
		StickyNote* poolNote = pool[i];
		StickyNoteRect poolNoteRect(poolNote);
		//�����ͬһ�����ڣ�����һ����Ч����
		if (note == poolNote)
			continue;
		//����ص������������
		if(hasOverlap(note, poolNote))
			continue;
		
		double distanceX = (note->x() > poolNote->x()) ? note->x() - (poolNote->x() + poolNote->width()) : poolNote->x() - (note->x() + note->width());
		double distanceY = (note->y() > poolNote->y()) ? note->y() - (poolNote->y() + poolNote->height()) : poolNote->y() - (note->y() + note->height());

		//������뵥�������Ϊ������������ۺϾ���ʱ������̫�󣬻��������
		//���ｫΪ���ľ�������Ϊ0
		distanceX = distanceX < 0 ? 0 : distanceX;
		distanceY = distanceY < 0 ? 0 : distanceY;


		//�������С����ֵ��������
		if(distanceX > threshold || distanceY > threshold)
			continue;

		StickyNotePositon notePos;
		notePos.ditance = sqrt(distanceX * distanceX + distanceY * distanceY);
		notePos.rect = poolNoteRect;


		//�ж��ĸ���ľ���Ƚ�Զ�������ĸ���Ϊ��׼
		//������x��Ϊ��׼����ô����ֻ��������
		if (distanceX > distanceY)
		{
			//��x��Ϊ��׼
			notePos.direction = note->x() < poolNote->x() ? LEFT : RIGHT;
		}else {
			//��y��Ϊ��׼
			notePos.direction = note->y() < poolNote->y() ? UP : DOWN;
		}
		postions.push_back(notePos);
	}

	if(postions.size() <= 0)
		return;
	postions.sort();

	for (auto postion : postions)
	{
		//�ж��ƶ����λ���Ƿ���ԭ�ȵĴ����ظ�
		//����ظ���ѡ����һ����������
		StickyNoteRect r = moveResult(rect, postion);
		bool ok = false;
		for (auto iter = pool.begin(); iter != pool.end(); iter++)
		{
			//������Լ������ж�
			if(*iter == note)
				continue;
			StickyNoteRect temp(*iter);
			ok = ok || hasOverlap(r, temp);
		}
		//����ص�,ѡ����һ��
		if(ok)
			continue;
		//���λ�ó�����Ļ�⣬�򲻴���
		if(r.x < 0 || r.y < 0)
			continue;
		startAnimation(note, r);
		break;
	}
}

/**
* ����note1 ��note2 �ƶ��Ķ���
* @brief NGui::StickyNoteMagnetic::startAnimation
* @param StickyNote * note1
* @param StickyNote * note2
* @return void
*/
void NGui::StickyNoteMagnetic::startAnimation(StickyNote* note1, StickyNote* note2)
{
	const int space = 2;
	QPropertyAnimation* animation = new QPropertyAnimation();
	QPoint pos1 = note1->pos();
	QPoint pos2 = note2->pos();

	QPoint pos;
	pos.setX(pos2.x() + note2->width() + space);
	pos.setY(pos2.y());

	animation->setPropertyName("pos");
	animation->setStartValue(pos1);
	animation->setEndValue(pos);
	animation->setTargetObject(note1);
	animation->setDuration(200);
	animation->start();

}

void NGui::StickyNoteMagnetic::setThreshold(const int& v)
{
	threshold = v;
}

int NGui::StickyNoteMagnetic::getThreshold()
{
	return threshold;
}

void NGui::StickyNoteMagnetic::setEnable(const bool& e)
{
	this->enable = e;
}

bool NGui::StickyNoteMagnetic::getEnable()
{
	return enable;
}

void NGui::StickyNoteMagnetic::setSpace(const int& s)
{
	this->space = s;
}

int NGui::StickyNoteMagnetic::getSpace()
{
	return space;
}

void NGui::StickyNoteMagnetic::startAnimation(StickyNote* note, const StickyNoteRect& rect)
{
	QPropertyAnimation* animation = new QPropertyAnimation();
	QPoint pos1 = note->pos();
	QPoint pos2 = note->pos();

	pos2.setX(rect.x);
	pos2.setY(rect.y);

	animation->setPropertyName("pos");
	animation->setStartValue(pos1);
	animation->setEndValue(pos2);
	animation->setTargetObject(note);
	animation->setDuration(200);
	animation->start();
}

bool NGui::StickyNoteMagnetic::hasOverlap(StickyNote* note1, StickyNote* note2)
{
	StickyNoteRect rc1(note1);
	StickyNoteRect rc2(note2);
	return hasOverlap(rc1, rc2);
}



NGui::StickyNoteRect NGui::StickyNoteMagnetic::moveResult(const StickyNoteRect& rect, const StickyNotePositon& magneticPos)
{
	StickyNoteRect result = rect;
	switch (magneticPos.direction)
	{
	case LEFT:
		result.x = magneticPos.rect.x - space - rect.width;
		result.y = magneticPos.rect.y;
		break;
	case RIGHT:
		result.x = magneticPos.rect.x + magneticPos.rect.width + space;
		result.y = magneticPos.rect.y;
		break;
	case UP:
		result.y = magneticPos.rect.y - result.height - space;
		result.x = magneticPos.rect.x;
		break;
	case DOWN:
		result.y = magneticPos.rect.height + magneticPos.rect.y + space;
		result.x = magneticPos.rect.x;
		break;
	}

	return result;
}

bool NGui::StickyNoteMagnetic::hasOverlap(const StickyNoteRect& rc1, const StickyNoteRect& rc2)
{
	if (rc1.x + rc1.width > rc2.x &&
		rc2.x + rc2.width > rc1.x &&
		rc1.y + rc1.height > rc2.y &&
		rc2.y + rc2.height > rc1.y
		)
		return true;
	else
		return false;

}


NGui::StickyNotePositon::StickyNotePositon()
	:ditance(0.0),direction(UP)
{

}

bool NGui::StickyNotePositon::operator>(const StickyNotePositon& pos)
{
	return this->ditance > pos.ditance;
}

bool NGui::StickyNotePositon::operator<(const StickyNotePositon& pos)
{
	return this->ditance < pos.ditance;
}

NGui::StickyNoteRect::StickyNoteRect(QWidget* w)
{
	x = w->x();
	y = w->y();
	width = w->width();
	height = w->height();
}
