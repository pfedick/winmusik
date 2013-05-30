#include "wmtoolbutton.h"
#include <QMouseEvent>
#include <QInputEvent>


WMToolButton::WMToolButton(QWidget *parent)
    : QToolButton(parent)
{
	//printf ("WMToolButton\n");
}

WMToolButton::~WMToolButton()
{

}


void WMToolButton::mousePressEvent ( QMouseEvent * event )
{
	//printf ("Klick\n");
	if (event->type()==QEvent::MouseButtonPress && event->modifiers()==Qt::ControlModifier) {
		controlClicked();
		return;
	}


	QToolButton::mousePressEvent(event);
}
