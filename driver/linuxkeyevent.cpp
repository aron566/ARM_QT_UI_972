#include "linuxkeyevent.h"
#include <QDebug>
#include <QThread>

LinuxKeyEvent::LinuxKeyEvent(QObject *parent) : QObject(parent)
{
    stop_task(false);
    Enable_Keycheck(false);
    //创建定时器，不指定父对象
    m_timer = new QTimer();
    //启动定时器
    m_timer->start(3000);
    //连接定时器信号槽，直接连接方式，在线程模式下取消默认队列连接方式，改为直接连接（同一个线程中处理定时器信号--槽）
    connect(m_timer,&QTimer::timeout, this,&LinuxKeyEvent::dealtimerout,Qt::DirectConnection);
}

/*停止线程运行*/
void LinuxKeyEvent::stop_task(bool flag)
{
    is_stopflag = flag;
}

void LinuxKeyEvent::key_check_Task(const QString &parameter)
{
    qDebug() << parameter;

    int fd;
    int num;
    INPUT_EVENT event;

    fd = open( DEVICE_NAME , O_RDONLY, 0);
    if (fd < 0)
    {
        perror("Can't open button device...\n");
    }
    while(!is_stopflag)
    {

        num = read(fd, &event, sizeof(INPUT_EVENT));
        if (sizeof(INPUT_EVENT) != num)
        {
            printf("read data error\n");
            return;
        }
        if(keycheck_active)
        {
            if(event.type == EV_KEY)
            {
                switch (event.code)
                {
                    case KEY_LEFT:
                        if(event.value)
                        {
                            emit is_keypressSignal(Qt::Key_Left);
                            emit is_keyreleaseSignal(Qt::Key_Left);
                        }
                        break;
                    case KEY_RIGHT:
                        if(event.value)
                        {
                            emit is_keypressSignal(Qt::Key_Tab);
                            emit is_keyreleaseSignal(Qt::Key_Tab);
                        }
                        break;
                    case KEY_UP:
                        if(event.value)
                        {
                            emit is_keypressSignal(Qt::Key_Up);
                            emit is_keyreleaseSignal(Qt::Key_Up);
                        }
                        break;
                    case KEY_DOWN:
                        if(event.value)
                        {
                            emit is_keypressSignal(Qt::Key_Down);
                            emit is_keyreleaseSignal(Qt::Key_Down);
                        }
                        break;
                    case KEY_ENTER:
                        QThread::msleep(200);
                        if(event.value)
                        { 
                            emit is_keypressSignal(Qt::Key_Space);//maybe space
                            emit is_keyreleaseSignal(Qt::Key_Space);
                        }
                        break;
                case KEY_DELETE://edit
                    if(event.value)
                    {
                            emit is_keypressSignal(Qt::Key_Enter);//edit
                            emit is_keyreleaseSignal(Qt::Key_Enter);
                    }
                    break;
                default:
                    qDebug() << "no match!!!";
                    break;
                }
            }
        }
        QThread::msleep(200);
    }
}


void LinuxKeyEvent::dealtimerout()
{
    m_timer->stop();
    Enable_Keycheck();
    qDebug() << "is thread timeout!";
}

void LinuxKeyEvent::Enable_Keycheck(bool flag)
{
    keycheck_active = flag;
}
