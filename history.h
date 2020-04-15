#ifndef HISTRORY_H
#define HISTRORY_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPainter>
#include <wavewindow.h>
#include "driver/MsgSqueue.h"
#define AD_VALUE_DEFAULT_MAX_LIMIT  2000
namespace Ui {
class history;
}

class history : public QWidget
{
    Q_OBJECT

public:
    explicit history(QWidget *parent = nullptr);
    ~history();
signals:
    void history_return_is_true();
    void show_waveSignal(int);
private:
    Ui::history *ui;
    QTimer *timer;
public slots:
    void pagenum_changed(int);
    void MsgRecDataSlot(MsgSqueue_t &data);
private slots:
    //定时器处理函数
    void timer_timeout(void);
    void on_return_Button_clicked();

    void on_CHANNEL_1Button_clicked();

    void on_CHANNEL_2Button_clicked();

    void on_CHANNEL_3Button_clicked();

    void on_CHANNEL_4Button_clicked();

    void on_CHANNEL_5Button_clicked();

    void on_CHANNEL_6Button_clicked();

    void on_CHANNEL_7Button_clicked();

    void on_CHANNEL_8Button_clicked();

    void on_CHANNEL_9Button_clicked();

    void on_CHANNEL_10Button_clicked();

private:
    void set_pagetitle(int page = RUNMODE_PAGE_1);
    void Set_Max_ADlabel(MsgSqueue_t &data);
    void Set_Min_ADlabel(MsgSqueue_t &data);
    void Set_Average_ADlabel(MsgSqueue_t &data);
    void Set_Realtime_ADlabel(MsgSqueue_t &data);
    void Set_Workmode(MsgSqueue_t &data);
private:
    int currentpage = RUNMODE_PAGE_1;
private:
    qint32 Ad_Max_Limit = AD_VALUE_DEFAULT_MAX_LIMIT;
signals:
    void Nortify_Max_AD_Is_Changed(qint32&);
};

#endif // HISTRORY_H
