
#ifndef KEYWHEEL_H_
#define KEYWHEEL_H_

#define WITH_QT		// Sorgt dafür, dass die PPL-String-Klasse mit QT interaggieren kann

#include <QLabel>
#include "../../ppl6/include/ppl6.h"
#include <QPainter>
#include <QFont>


class KeyWheel : public QLabel
{
	Q_OBJECT

	private:
		void paintEvent ( QPaintEvent * );
		void mousePressEvent ( QMouseEvent * );

		int shiftAngle(int angle);
		int getAngle(int x, int y);
		int getRadius(int x, int y);
		int getKeyFromClickPosition(const QPoint &p);
		void drawKey(QPainter &painter, QPen &PenFront, QPen &PenBorder, int x, int y, int key);


		int key;

		QString keyname[26];

	public:
		KeyWheel(QWidget *parent = 0);
		~KeyWheel();
		void setKeyName(int key, const QString &name);
		void setCurrentKey(int key);
		int currentKey() const;

	signals:
	     void clicked(int key);
};



#endif /* KEYWHEEL_H_ */
