#ifndef WAVEWINDOW_H
#define WAVEWINDOW_H

#include <QWidget>
#include <QChar>
#include <QtCharts>
#include "driver/MsgSqueue.h"
#include <QTimer>
#include "thread/databasetask.h"

#define WAVECHANNEL_NUM_MAX 10U
#define X_MAX_SEC           360UL
#define X_MAX_MIN_SEC       X_MAX_SEC*60UL
typedef enum
{
    MAIN_PAGE_NUM = 0,
    RUNMODE_PAGE_1,
    RUNMODE_PAGE_2,
    RUNMODE_PAGE_3,
    RUNMODE_PAGE_4,
    RUNMODE_PAGE_5,
    RUNMODE_PAGE_6,
    RUNMODE_PAGE_7,
    RUNMODE_PAGE_8,
    RUNMODE_PAGE_9,
    RUNMODE_PAGE10
}CALL_PAGE_NUM_t;

typedef struct
{
    long int last_time;         //记录最后录入时间
    long int first_time;        //记录最先录入时间
    quint32 recordCnt_min;
    quint32 recordCnt_sec;
}Channel_Record_t;

namespace Ui {
class waveWindow;
}

class waveWindow : public QWidget
{
    Q_OBJECT

public:
    explicit waveWindow(QWidget *parent = nullptr);
    ~waveWindow();
signals:
    void wave_return_is_true();
    void show_runmodeSignal(int);
public slots:
    void Rec_Max_AD_Is_ChangedSlot(qint32&);
    void Send_Database_AD_Value_slot(const QList<PontData_t>&queryData,const int current_read_channel,int resultcnt);
    void pagenum_changed(int);
    void MsgRecDataSlot(MsgSqueue_t &data);

public slots:
    //定时器处理函数
    void timer_timeout(void);
private slots:
    void on_runmodeButton_clicked();

    void on_lastButton_clicked();

    void on_nextButton_clicked();

    void on_return_Button_clicked();
private:
    void InitTabOrder(void);
    void Init_Data(void);
    void set_pagetitle(int page = RUNMODE_PAGE_1);
    void update_pagewavedata();
    void update_pagewavedata_min();
    QDateTime GetDateTimeFromSec(int secondCount);
    void addOnePointToChart(QSplineSeries *targetSeries, int secondCount, int y_value);
    void addOnePointToVector(QVector<QPointF> *targetList, int secondCount, int y_value);
private:
    Ui::waveWindow *ui;
private:
    int currentpage = 1;
    QChart *m_chart;
    QTimer *m_timer;
//    QLineSeries *m_linSerie;

    QSplineSeries *splineSeries;

//    QValueAxis *m_axisX;//X坐标轴的一种，显示值，显示日期使用其他坐标轴
    QValueAxis *m_axisY;//Y

    //时间坐标轴
    QDateTimeAxis *m_axisX;

    qint64 mLasttime;

    QVector<QPointF> channelpoint_buff_sec[WAVECHANNEL_NUM_MAX+1];
    QVector<QPointF> channelpoint_buff_min[WAVECHANNEL_NUM_MAX+1];

    Channel_Record_t ChannelpointCnt[WAVECHANNEL_NUM_MAX+1];
public:
    quint64 accumulative_sec = 0;
private:
    qint32 Current_Coordinate_MAX_Y = 0;
};

#endif // WAVEWINDOW_H
