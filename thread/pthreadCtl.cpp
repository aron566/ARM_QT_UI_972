#include <QDebug>
#include "pthreadCtl.h"

//coordinator widget
ThreadController::ThreadController(QObject *parent) : QObject(parent)
{
    //创建数据库查询
    databasetaskobj = new databasetask;
    databasetaskobj->moveToThread(&threadmanagerDatabase);
    connect(this,&ThreadController::start_databaseSignal,databasetaskobj,&databasetask::start_databaseTask);
    connect(databasetaskobj,&databasetask::Send_Database_AD_Value_Signal,this,&ThreadController::Send_Database_AD_Value_slot);

    //创建消息队列发送
    pmsgsqueue = new MsgRS_Squeue();
    pmsgsqueue->moveToThread(&threadmanagerMsg);
    connect(this,&ThreadController::start_MsgSendSignal,pmsgsqueue,&MsgRS_Squeue::MsgRSendTask);
    //转发接收的数据
    connect(pmsgsqueue,&MsgRS_Squeue::RecDataSignal,this,&ThreadController::MsgRecDataSlot);

    //创建按键事件检测
    plinuxkeytaskobj = new LinuxKeyEvent;
    plinuxkeytaskobj->moveToThread(&threadmanagerKeycheck);
    connect(this,&ThreadController::start_keycheckSignal,plinuxkeytaskobj,&LinuxKeyEvent::key_check_Task);
    connect(plinuxkeytaskobj,&LinuxKeyEvent::is_keypressSignal,this,&ThreadController::is_keypressSlot);
    connect(plinuxkeytaskobj,&LinuxKeyEvent::is_keyreleaseSignal,this,&ThreadController::is_keyreleaseSlot);
}
ThreadController::~ThreadController()
{
    plinuxkeytaskobj->stop_task(true);
    databasetaskobj->stop_task(true);
    pmsgsqueue->stop_task(true);
    qDebug() << "****回收线程资源中****";
    threadmanagerKeycheck.quit();
    threadmanagerKeycheck.wait();
    threadmanagerDatabase.quit();
    threadmanagerDatabase.wait();
    threadmanagerMsg.quit();
    threadmanagerMsg.wait();
    delete plinuxkeytaskobj;
    delete databasetaskobj;
    delete pmsgsqueue;
}
//启动按键检测线程
void ThreadController::start_keycheckTask()
{
    QString str = "start keycheck task now!";
    threadmanagerKeycheck.start();
    emit start_databaseSignal(str);
}

//启动数据库查询线程
void ThreadController::start_databaseTask()
{
    QString str = "start database task now!";
    threadmanagerDatabase.start();
    emit start_keycheckSignal(str);
}

//启动消息队列收发线程
void ThreadController::start_SendMsgTask()
{
    QString str = "start Send Msg task now!";
    threadmanagerMsg.start();
    emit start_MsgSendSignal(str);
}

//slot func
void ThreadController::Send_MsgSqueueToBuff(const MsgSqueue_t &data)
{
    pmsgsqueue->addMsgToSendBuff(data);
}


void ThreadController::MsgRecDataSlot(MsgSqueue_t &data)
{
    emit RecDataSignal(data);
}

void ThreadController::Send_Database_AD_Value_slot(const QList<PontData_t>&queryData,const int current_read_channel ,int resultcnt)
{
    emit Send_Database_AD_Value_Signal(queryData ,current_read_channel,resultcnt);
}

void ThreadController::is_keypressSlot(int key)
{
    emit is_keypressSignal(key);
}
void ThreadController::is_keyreleaseSlot(int key)
{
    emit is_keyreleaseSignal(key);
}
