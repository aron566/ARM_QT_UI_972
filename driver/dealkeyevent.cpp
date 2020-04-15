#include "dealkeyevent.h"
#include <QDebug>
dealKeyEvent::dealKeyEvent(QObject *parent) : QObject(parent)
{
    timerID = this->startTimer(3000);
}
//模拟键盘按键> 向右
void dealKeyEvent::on_rightButton_clicked()
{
    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QString(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier, QString(""));
    QApplication::sendEvent(receiver, &keyPress);
    QApplication::sendEvent(receiver, &keyRelease);
}

//模拟键盘按键<
void dealKeyEvent::on_leftButton_clicked()
{
    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, QString(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier, QString(""));
    QApplication::sendEvent(receiver, &keyPress);
    QApplication::sendEvent(receiver, &keyRelease);
}

//模拟键盘按键close window
void dealKeyEvent::on_closeButton_clicked()
{
    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent tabKey(QEvent::KeyPress, Qt::Key_F4, Qt::AltModifier);
    QCoreApplication::sendEvent(receiver, &tabKey);
}

//模拟键盘按键TAB
void dealKeyEvent::on_tabButton_clicked()
{
    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier, QString(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Tab, Qt::NoModifier, QString(""));
    QApplication::sendEvent(receiver, &keyPress);
    QApplication::sendEvent(receiver, &keyRelease);
}

//模拟键盘按键空格
void dealKeyEvent::on_spaceButton_clicked()
{
    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, QString(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier, QString(""));
    QApplication::sendEvent(receiver, &keyPress);
    QApplication::sendEvent(receiver, &keyRelease);
}

//单按键状态虚拟按下
void dealKeyEvent::on_key_pressed(int key)
{

    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent keyPress(QEvent::KeyPress, static_cast<Qt::Key>(key), Qt::NoModifier, QString(""));
    QApplication::sendEvent(receiver, &keyPress);
}
//单按键状态虚拟释放
void dealKeyEvent::on_key_release(int key)
{

    QWidget *receiver = QApplication::focusWidget();
    QKeyEvent keyRelease(QEvent::KeyRelease, static_cast<Qt::Key>(key), Qt::NoModifier, QString(""));
    QApplication::sendEvent(receiver, &keyRelease);
}


void dealKeyEvent::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerID)
    {
        this->killTimer(timerID);
        on_key_pressed(Qt::Key_Right);
        on_key_release(Qt::Key_Space);

//        on_key_pressed(Qt::Key_Space);
//        on_key_release(Qt::Key_Space);

    }
}
