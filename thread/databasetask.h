#ifndef DATABASETASK_H
#define DATABASETASK_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
/*信号量所需*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>

#define CHANNEL_NUM_MAX     10UL
typedef struct
{
    int     time;
    int     advalue;
}PontData_t;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

class databasetask:public QObject
{
    Q_OBJECT
public:
      explicit databasetask(QObject *parent = nullptr);
signals:
    void Send_Database_AD_Value_Signal(const QList<PontData_t>&queryData,const int current_read_channel,int resultcnt);
public slots:
    void start_databaseTask(const QString &);
public:
    void databaseTaskthread(const QString &);
private:
      bool is_stopflag;
      QSqlDatabase db;
      QSqlQuery query;
private:
      void Init_Data();
      void Database_qsort(QList<PontData_t>& data ,quint32);
      void push_dataTolist(QList<PontData_t>& data,quint32 dataCnt);
      void ChannelDataUpdate(QList<PontData_t>& data,quint32 dataCnt);
public:
      void stop_task(bool flag = true);
private:
      int current_read_channel;
      QList<PontData_t>channel_data[CHANNEL_NUM_MAX+1];
      QList<PontData_t>tempdatabuff;
      int channel_lasttimestamp[CHANNEL_NUM_MAX] = {0};
      quint32 channel_datacnt[CHANNEL_NUM_MAX] = {0};
private:
      /*信号量*/
      int database_sem_id;
      int semaphore_UI_Init();
      bool To_Block_Opt_Database();
      bool To_Release_Opt_Databse();
};

#endif // DATABASETASK_H
