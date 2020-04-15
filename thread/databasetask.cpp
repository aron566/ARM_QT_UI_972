#include "databasetask.h"
#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include "wavewindow.h"

#define DBFILE_PATH    "/dev/shm/GNNC_Device_info.db"
//#define  DBFILE_PATH    "/home/aron566/QtProject/wave/GNNC_Device_info.db"
databasetask::databasetask(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QList<PontData_t>>("QList<PontData_t>");
    stop_task(false);

    Init_Data();
    semaphore_UI_Init();
    //打印QT支持的数据库驱动
    qDebug() << QSqlDatabase::drivers();
    //添加数据库支持
    db = QSqlDatabase::addDatabase("QSQLITE","a");
    //connect
    //数据库ip
//    db.setHostName("127.0.0.1");
//    db.setUserName("root");
//    db.setPassword("123456");
    //是使用哪个数据库
    db.setDatabaseName(DBFILE_PATH);
    //打开数据库
    if(!db.open())
    {
        qDebug() << db.lastError().text() << __LINE__;
        return;
    }
    query = QSqlQuery(db);

}


void databasetask::start_databaseTask(const QString & parameter)
{
    databaseTaskthread(parameter);
}

void databasetask::databaseTaskthread(const QString &parameter)
{
    qDebug() << parameter;

    PontData_t tempdata;
    memset(&tempdata,0,sizeof(PontData_t));
    QString sqlcmd = "";
    time_t timestamp = 0;
    time_t last_timestamp[CHANNEL_NUM_MAX+1] = {0};
    quint32 dataCnt = 0;
    quint32 lastdataCnt = 0;
    while(!is_stopflag)
    {
        QThread::msleep(100);
//        qDebug() << "database is running ";
        dataCnt = 0;
        //经过的秒数
        timestamp = time((time_t *) NULL);
        timestamp -= X_MAX_SEC;

        if(timestamp != last_timestamp[current_read_channel])
        {
            last_timestamp[current_read_channel] = timestamp;
            sqlcmd = QString("select * from device_channel_%1 where update_time >= %2").arg(current_read_channel).arg(timestamp);
            //阻塞其他进程
            To_Block_Opt_Database();
            if(!query.exec(sqlcmd))
            {
                qDebug() << "query:" << query.lastError();
            }
//            qDebug() << "query:" << sqlcmd;
//            qDebug() << "UI Opt Databse";

            //一行一行遍历
            while(query.next())
            {
                tempdata.advalue = query.value("value_L").toInt();
                tempdata.time = query.value("update_time").toInt();
                /*计算是否追加数据进入队列*/
                if(lastdataCnt <= dataCnt || lastdataCnt == 0)
                {
    //                qDebug() << "append" << dataCnt;
                    tempdatabuff.append(tempdata);
                }
                else
                {
//                    qDebug() << "rplace" << dataCnt;
                    tempdatabuff.replace(dataCnt,tempdata);
                }
                dataCnt++;

                if(dataCnt >= X_MAX_SEC)
                {
                    break;
                }
            }
            query.clear();
            //释放使用
            To_Release_Opt_Databse();
        }

        /*更新链表存入的数据量,取最大值*/
        lastdataCnt = lastdataCnt<dataCnt?dataCnt:lastdataCnt;

        //查询结果不为0则发送信号
        if(dataCnt > 0)
        {
            /*加入所有查询到的数据，进行处理*/
            push_dataTolist(tempdatabuff,dataCnt);
            /*发送当前通道数据*/
            emit Send_Database_AD_Value_Signal(channel_data[current_read_channel],current_read_channel ,channel_datacnt[current_read_channel]);
        }
        //更换查询通道
        current_read_channel++;
        current_read_channel = static_cast<uint32_t>(current_read_channel) > CHANNEL_NUM_MAX?1:current_read_channel;
    }
}

void databasetask::Init_Data()
{
    current_read_channel = 1;
}
/*停止线程运行*/
void databasetask::stop_task(bool flag)
{
    is_stopflag = flag;
}

void databasetask::Database_qsort(QList<PontData_t>& data ,quint32 resultcnt)
{
    QList<PontData_t>::iterator it;
    it = data.begin();
    for(;it != data.end();)
    {
        if(it->time == data.value(resultcnt-1).time)
        {
            break;
        }
        ++it;
    }

    std::sort(data.begin(),it,[](const PontData_t &infoA,const PontData_t &infoB){return infoA.time < infoB.time;});
}

/*加入数据进入链表*/
void databasetask::push_dataTolist(QList<PontData_t>& data,quint32 dataCnt)
{
    //排序处理
//    Database_qsort(data ,dataCnt);

    //数据更新处理
    ChannelDataUpdate(data,dataCnt);
}

void databasetask::ChannelDataUpdate(QList<PontData_t>& data,quint32 dataCnt)
{
    bool firstentry_flag = true;
    //更新数据处理
    for(quint32 i = 0;i < dataCnt;i++)
    {
        //首数据秒更新则算有效，刷掉图表中最左边数据
        if(channel_lasttimestamp[current_read_channel] < data.value(i).time || channel_lasttimestamp[current_read_channel]==0)
        {
            if(firstentry_flag == true)
            {
                //更新首次时间
                channel_lasttimestamp[current_read_channel] = data.value(i).time;
                firstentry_flag = false;
            }
            //临界保护数据长度,到达指定长度，数据只替换不追加
            if(channel_datacnt[current_read_channel] >= X_MAX_SEC)
            {
                //去除旧数据，X轴左移，Y值不变，再追加一条新数据
                channel_data[current_read_channel].removeFirst();
                for(quint32 x = 0;x < X_MAX_SEC-1;x++)
                {
                    channel_data[current_read_channel].replace(x,channel_data[current_read_channel].value(x));
                }
                //追加数据
                channel_data[current_read_channel].append(data.value(i));
            }
            else
            {
                channel_data[current_read_channel].append(data.value(i));
                channel_datacnt[current_read_channel]++;
            }
        }
    }
}

/*
 * 初始化UI进程通讯信号量
 * */
int databasetask::semaphore_UI_Init(void)
{
    //创建信号量
    database_sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

    if(database_sem_id == -1)
    {
        qDebug() << "sem_id failed";
        return -1;
    }

    //用于初始化信号量，在使用信号量前必须这样做
    union semun sem_union;

    sem_union.val = 1;
    if(semctl(database_sem_id, 0, SETVAL, sem_union) == -1)//0为信号量下标
    {
        qDebug() << "sem_init failed";
        return -1;
    }
    return 0;
}

/*
 * 检查是否可以操作数据库，不可以则返回false,可以返回true
 * */
bool databasetask::To_Block_Opt_Database(void)
{
    //对信号量做减1操作，即等待P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;//信号量下标
    sem_b.sem_op = -1;//P()
    sem_b.sem_flg = SEM_UNDO;//设置在进程出现错误时(异常退出)信号量值自动恢复，防止一个进程占着信号量
    if(semop(database_sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_p failed\n");
        return false;
    }
    return true;
}

/*
 * 允许其他进程操作数据库
 * */
bool databasetask::To_Release_Opt_Databse(void)
{
    //这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;//信号量下标
    sem_b.sem_op = 1;//V()
    sem_b.sem_flg = SEM_UNDO;//设置在进程出现错误时(异常退出)信号量值自动恢复，防止一个进程占着信号量
    if(semop(database_sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "semaphore_v failed\n");
        return false;
    }
    return true;
}
