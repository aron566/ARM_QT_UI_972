#ifndef PARAMETER_H
#define PARAMETER_H

#include <QWidget>
#include <QLineEdit>
#include "parameter2.h"
#include "driver/MsgSqueue.h"
#include "mykeyboard.h"
namespace Ui {
class parameter;
}

class parameter : public QWidget
{
    Q_OBJECT

public:
    explicit parameter(QWidget *parent = nullptr);
    ~parameter();
    void InitTabOrder();
private:
    void set_parameter();
    void DateFormatConv();
    void serveripFormatConv();
    void clientipFormatConv();
    void commportFormatConv();
    void SendMsgToSqueue(MSGQueueTYPE_t type,QString data);
signals:
    void parameter_return_is_true();
    void toEditBox(QWidget * ,QLineEdit *,QString,QString);
    void SendMsgToSqueueSignal(const MsgSqueue_t &data);
    void RecDataSignal(MsgSqueue_t &data);
public slots:
    void MsgRecDataSlot(MsgSqueue_t &data);
private slots:

    void show_this_window();
    void processedit(QWidget *pObject ,QLineEdit *pwidget,QString title ,QString edittext);

    void Send_MsgSqueueToBuff(const MsgSqueue_t &data);

    void on_return_Button_clicked();
    void on_comfirm_Button_clicked();
    void on_nextpage_Button_clicked();
    void on_clientip_1_returnPressed();

    void on_clientip_2_returnPressed();

    void on_clientip_3_returnPressed();

    void on_clientip_4_returnPressed();

    void on_year_returnPressed();

    void on_month_returnPressed();

    void on_day_returnPressed();

    void on_hour_returnPressed();

    void on_min_returnPressed();

    void on_serverip_1_returnPressed();

    void on_serverip_2_returnPressed();

    void on_serverip_3_returnPressed();

    void on_serverip_4_returnPressed();

    void on_commport_returnPressed();

private:
    Ui::parameter *ui;
    parameter2 *parameter2Ui;
    mykeyboard *mykeyboardUi;
private:
    MsgSqueue_t parameterSet;
    QString clientipPar;
    QString serverportPar;
    QString datePar;
    QString serveripPar;
};

#endif // PARAMETER_H
