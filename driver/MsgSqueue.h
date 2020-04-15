#ifndef MSGSQUEUE_H
#define MSGSQUEUE_H
#include <QObject>
#include <QTimer>

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <errno.h>
#include<sys/stat.h>
#include <fcntl.h>
/*ftok函数*/
#include <sys/types.h>
#include <sys/ipc.h>
/*消息ID获取 -->消息收发 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SQUEUENUM_MAX   10U//SEND MSG BUFF
#define SQUEUE_TYPE_MAX 15L

typedef enum
{
    MSG_MIN_CNT = 0,
    SET_BOARD_NAME_MSG,
    SET_DATE_TIME_MSG,
    SET_SERVER_IP_MSG,
    SET_CLIENT_IP_MSG,
    SET_PORT_NUM_MSG,
    SET_MQTT_IP_MSG,
    SET_WIFI_SSID,
    SET_WIFI_KEY,
    READ_WIFI_SSID,
    READ_WORK_MODE_MSG, /*透传 配置中 标定中 升级中*/
    READ_HISTRORY_DATA,
    READ_MAX_AD_DATA,
    READ_MIN_AD_DATA,
    READ_AVERAGE_AD_DATA,
    READ_REALTIME_AD_DATA,
    MSG_MAX_CNT         /*最大消息类型数量*/
}MSGQueueTYPE_t;

typedef struct{
    long type;
    int channel;
    char databuff[128];
}MsgSqueue_t;

typedef struct
{
      MsgSqueue_t* databuff;
      int entrance;
      int exit;
      int count;
}circular_t;

class MsgRS_Squeue :public QObject
{
    Q_OBJECT
public:
    explicit MsgRS_Squeue(QObject *parent = nullptr);
    void Init_MsgSqueue();
    void Init_Circular();
    void addMsgToSendBuff(const MsgSqueue_t &data);
    //send msg
    int MsgSendToSqueues(MsgSqueue_t *data);
    //rev msg
    int MsgRecFormSqueues(MsgSqueue_t *data,long msgtype = 1);
    int Process_Com_Msg(MsgSqueue_t &data);
    void MsgRecTask();
signals:
    void RecDataSignal(MsgSqueue_t &data);
public slots:
    void MsgRSendTask(const QString &);

private:
    bool SqueueBuff_is_empty(circular_t *cb);
    void SqueueBuff_emptydata(circular_t *cb);
    bool SqueueBuff_isFull(circular_t *cb);
    bool GetSqueueBuff_data(MsgSqueue_t &data);
private:
    //发送缓冲区
    circular_t  Squeue_SendBuff;
    MsgSqueue_t senddata[SQUEUENUM_MAX];
    //接收
    MsgSqueue_t recdata;
    //Squeue ID
    int Smsgid;
    int Rmsgid;
private:
    bool is_stopflag;
public:
    void stop_task(bool flag = true);
};

#endif
