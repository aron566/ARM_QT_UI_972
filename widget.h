#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPainter>

#include "parameter.h"
#include "history.h"
#include "wavewindow.h"
#include "mykeyboard.h"

#include "company.h"
#include "thread/pthreadCtl.h"
#include "driver/dealkeyevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT//声明Q_OBJECT，因为有了这个才可以使用信号槽connect函数。

public:
    Widget(QWidget *parent = nullptr);//构造函数 explicit显示构造
    ~Widget();//析构函数
    void InitTabOrder();

    QString GetupdateTime();
    static QPixmap ChangeImageColor(QPixmap sourcePixmap, QColor origColor, QColor destColor);
public:
    QString datetimeStr;
private:
    Ui::Widget *ui;

    dealKeyEvent *dealkeyeventobj;
    parameter *parameterUi;
    history *historyUi;
    waveWindow *waveWindowui;
    mykeyboard *mykeyboardUi;
    company *deviceinfoUi;
    QTimer *timer;
    QTimer *Sectimer;
    QVector<QPoint> Point_array;//保存所有的坐标点的顺序容器
    //如果要绘图，需要实现如下函数，在界面刷新（移动，缩小，放大）的时候自动被调用
    void paintEvent(QPaintEvent *event);

signals:
    void Send_Database_AD_Value_Signal(const QList<PontData_t>&queryData,const int current_read_channel,int index ,int resultcnt);
    void RecPage1_DataSignal(MsgSqueue_t &data);
    void RecPage2_DataSignal(MsgSqueue_t &data);
    void RecPage3_DataSignal(MsgSqueue_t &data);
    void SendMsgToSqueueSignal(const MsgSqueue_t &data);
    void show_waveSignal(int);
    void show_runmodeSignal(int);

private slots:

    void MsgRecDataSlot(MsgSqueue_t &data);/*队列消息*/
    void processedit(QWidget *,QLineEdit *,QString,QString);
    void show_parameterUi(QString);
    void Send_MsgSqueueToBuff(const MsgSqueue_t &data);
    void show_runmodepage(int);
    void show_wavepage(int);
    //定时器处理函数
    void timer_timeout(void);
    void updateTimeNow(void);
    void on_HistroryButton_clicked();
    void on_Monitor_Button_clicked();
    void on_Par_Button_clicked();
    void show_this_window();
    void on_Reboot_Button_clicked();
    void on_About_Button_clicked();

private:
    QWidget* pLastCallobj;
    QLineEdit *pLastCallwidget;
private:
    ThreadController *MyThreadController;
};
#endif // WIDGET_H
