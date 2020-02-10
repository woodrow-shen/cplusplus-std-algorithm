#include <QApplication>
#include <QKeyEvent>
#include "keypress.h"

KeyPress::KeyPress(QWidget *parent) : QWidget(parent)
{
    myLabelText = new QLabel ("You Didn't Pressed / Released any Key");
}

void KeyPress::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up)
    {
        myLabelText->setText("You Pressed Key Arrow Up");
    }
    else if (event->key() == Qt::Key_Down)
    {
        myLabelText->setText("You Pressed Key Arrow Down");
    }
    else
    {
        myLabelText->setText("You Pressed Other Key");
    }
}

void KeyPress::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up)
    {
        myLabelText->setText("You Released Arrow Up");
    }
    else if (event->key() == Qt::Key_Down)
    {
        myLabelText->setText("You Pressed Key Arrow Down");
    }
    else
    {
        myLabelText->setText("You Released Other Key");
    }
}
