#ifndef LINUXKEYEVENT_H
#define LINUXKEYEVENT_H
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#include <QApplication>
#include <QWidget>
#include <QKeyEvent>
#include <QObject>
#include <QTimer>
#define DEVICE_NAME "/dev/input/event0"

typedef struct input_event INPUT_EVENT;

class LinuxKeyEvent : public QObject
{
    Q_OBJECT
public:
    explicit LinuxKeyEvent(QObject *parent = nullptr);

signals:
//    void is_leftkeySignal();
//    void is_rightkeySignal();
//    void is_spacekeySiganl();
//    void is_tabkeySiganl();
    void is_keypressSignal(int);
    void is_keyreleaseSignal(int);
public slots:
    void key_check_Task(const QString &parameter);
private:
    bool is_stopflag;
    int timerID;
    bool keycheck_active;
public:
    void stop_task(bool flag = true);
    void Enable_Keycheck(bool flag = true);
private:
    void dealtimerout();
private:
    QTimer *m_timer;
};

#endif // LINUXKEYEVENT_H
