#ifndef PTHREADCTL_H
#define PTHREADCTL_H

#include <QObject>
#include <QThread>
#include <QString>
#include "databasetask.h"
#include "driver/MsgSqueue.h"
#include "driver/linuxkeyevent.h"

class ThreadController:public QObject
{
  Q_OBJECT
public:
    explicit ThreadController(QObject *parent = nullptr);
    ~ThreadController();
signals:
    void is_keypressSignal(int);
    void is_keyreleaseSignal(int);

    void start_keycheckSignal(const QString &);
    void start_databaseSignal(const QString &);
    void start_MsgSendSignal(const QString &);
    void RecDataSignal(MsgSqueue_t &data);
    void Send_Database_AD_Value_Signal(const QList<PontData_t>&queryData,const int current_read_channel,int resultcnt);

public slots:
    void is_keypressSlot(int);
    void is_keyreleaseSlot(int);

    void Send_MsgSqueueToBuff(const MsgSqueue_t &data);
    void MsgRecDataSlot(MsgSqueue_t &data);
    void Send_Database_AD_Value_slot(const QList<PontData_t>&queryData,const int current_read_channel ,int resultcnt);
public:
    void start_keycheckTask();
    void start_databaseTask();
    void start_SendMsgTask();

private:
    QThread threadmanagerDatabase;
    QThread threadmanagerMsg;
    QThread threadmanagerKeycheck;
    databasetask *databasetaskobj;
    MsgRS_Squeue *pmsgsqueue;
    LinuxKeyEvent *plinuxkeytaskobj;
};



#endif
