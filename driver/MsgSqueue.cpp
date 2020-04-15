#include <QDebug>
#include <QThread>

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include<sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include "MsgSqueue.h"

#define SEND_MSG_KEY_PATH     "/dev/shm/GNNC_Device_info.db"
//#define SEND_MSG_KEY_PATH     "/home/aron566/Downloads/Msgqueue/send"
#define RECE_MSG_KEY_PATH     "/root/GNNC_cnf.ini"
//#define RECE_MSG_KEY_PATH     "/home/aron566/Downloads/Msgqueue/rec"
MsgRS_Squeue::MsgRS_Squeue(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<MsgSqueue_t>("MsgSqueue_t&");

    stop_task(false);
    Init_MsgSqueue();
    Init_Circular();
}
/*停止线程运行*/
void MsgRS_Squeue::stop_task(bool flag)
{
    is_stopflag = flag;
}
void MsgRS_Squeue::Init_MsgSqueue()
{
    Rmsgid = Smsgid = 0;
    key_t key = ftok(SEND_MSG_KEY_PATH,'6');/*ftok只是根据文件inode在系统内的唯一性来取一个数值，和文件的权限无关 project ID它的取值是1到255*/
    std::cout << "UISENDkey:0x" << std::hex << key << std::endl;

    Smsgid = msgget(key,IPC_CREAT|O_RDWR|0664);

    key = ftok(RECE_MSG_KEY_PATH,'6');
    Rmsgid = msgget(key,O_RDONLY);
    if(Smsgid<0 || Rmsgid <0)
    {
        perror("msgget error!");
        std::cout << "error" << std::endl;
    }
    std::cout << "UIRECkey:0x" << std::hex << key << std::endl;

}
void MsgRS_Squeue::Init_Circular()
{
    //发送缓冲区 init
    Squeue_SendBuff.databuff = senddata;
    Squeue_SendBuff.exit = 0;
    Squeue_SendBuff.count = SQUEUENUM_MAX;
    Squeue_SendBuff.entrance = 0;
}
//缓冲区已空
bool MsgRS_Squeue::SqueueBuff_is_empty(circular_t *cb)
{
    if (cb->entrance == cb->exit)
        return true;
    else
        return false;
}
//缓冲区已满
bool MsgRS_Squeue::SqueueBuff_isFull(circular_t *cb)
{
    if ((cb->entrance - cb->exit) == cb->count)
        return true;
    else
        return false;
}
//获取缓冲区数据
bool MsgRS_Squeue::GetSqueueBuff_data(MsgSqueue_t &data)
{
    if(!SqueueBuff_is_empty(&Squeue_SendBuff))
    {
        memcpy(&data,&Squeue_SendBuff.databuff[Squeue_SendBuff.exit],sizeof(MsgSqueue_t));
        Squeue_SendBuff.exit = (Squeue_SendBuff.exit+1)%SQUEUENUM_MAX;
        return true;
    }
    return false;
}
void MsgRS_Squeue::SqueueBuff_emptydata(circular_t *cb)
{
    cb->entrance = cb->exit = 0;
    memset(cb->databuff, 0, cb->count);
}
int MsgRS_Squeue::MsgRecFormSqueues(MsgSqueue_t *data ,long msgtype)
{
    int ret = -1;

    ret = msgrcv(Rmsgid,data,sizeof(MsgSqueue_t)-sizeof(data->type),msgtype,IPC_NOWAIT);
    if(-1 != ret)
    {
//      std::cout << "rcv--type:0x" << std::hex << data->type <<";databuff:" << data->databuff << std::endl;
    }
    return ret;
}
void MsgRS_Squeue::MsgRSendTask(const QString &str)
{
    qDebug() << str;
    MsgSqueue_t sendtempdata;

    while(!is_stopflag)
    {
//        qDebug() << "Msg Send is running";
        if(GetSqueueBuff_data(sendtempdata))
        {
            qDebug() << "send type:" << sendtempdata.type << "data:" << sendtempdata.databuff;
            MsgSendToSqueues(&sendtempdata);
        }
        MsgRecTask();

        QThread::msleep(1);
    }
}


int MsgRS_Squeue::MsgSendToSqueues(MsgSqueue_t *data)
{
    int ret = 0;
    QString str;
    qDebug() << "msgtype:" << data->type << "***DATA***:" << str.sprintf("%s",data->databuff);
    ret = msgsnd(Smsgid,data,sizeof(MsgSqueue_t)-sizeof(data->type),0);
    if(ret == -1)
    {
        qDebug() << "send msg type " << data->type << " error!";
    }
    return ret;
}

void MsgRS_Squeue::addMsgToSendBuff(const MsgSqueue_t &data)
{
    int index = Squeue_SendBuff.entrance;

    if(SqueueBuff_isFull(&Squeue_SendBuff))
    {
        return;
    }
    else
    {
        memcpy(&Squeue_SendBuff.databuff[index],&data,sizeof(MsgSqueue_t));
        qDebug() << "msgtype:" << data.type << "msg:" << data.databuff;
        Squeue_SendBuff.entrance = (index+1)%Squeue_SendBuff.count;
    }
}
void MsgRS_Squeue::MsgRecTask()
{
    static MsgSqueue_t recetempdata;
    static long msgtype = 0;
    int ret = 0;

    msgtype = msgtype >= SQUEUE_TYPE_MAX?0:msgtype;
//    qDebug() << "Msg Rece is running REC TYPE:" << msgtype;
    ret = MsgRecFormSqueues(&recetempdata ,msgtype);
    if(ret != -1)
    {
        //接收消息成功，处理消息
        Process_Com_Msg(recetempdata);
    }
    msgtype++;
}


/*处理通讯接收的消息*/
int MsgRS_Squeue::Process_Com_Msg(MsgSqueue_t &data)
{
    int ret = 0;
    switch(data.type)
    {
    case SET_BOARD_NAME_MSG:
    case SET_DATE_TIME_MSG:
    case SET_SERVER_IP_MSG:
    case SET_CLIENT_IP_MSG:
    case SET_PORT_NUM_MSG:
    case SET_MQTT_IP_MSG:
    case READ_HISTRORY_DATA://wave
    case READ_WORK_MODE_MSG:
    case READ_MAX_AD_DATA:
    case READ_MIN_AD_DATA:
    case READ_AVERAGE_AD_DATA:
    case READ_REALTIME_AD_DATA:
        emit RecDataSignal(data);
        break;
    default:
        return -1;
    }
    return ret;
}
