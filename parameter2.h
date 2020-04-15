#ifndef PARAMETER2_H
#define PARAMETER2_H
#include "driver/MsgSqueue.h"
#include <QWidget>
#include <QLineEdit>
namespace Ui {
class parameter2;
}

class parameter2 : public QWidget
{
    Q_OBJECT

public:
    explicit parameter2(QWidget *parent = nullptr);
    ~parameter2();
    void InitTabOrder(void);
public:

private:
    void set_parameter();
    void boardnameFormatConv();
    void wifikeyFormatConv();
    void wifissidFormatConv();
    void Add_wifissid_ToItem(MsgSqueue_t &data);
    void SendMsgToSqueue(MSGQueueTYPE_t type,QString data);
signals:
    void parameter2_return_is_true();
    void toEditBox(QWidget * ,QLineEdit *,QString,QString);
    void SendMsgToSqueueSignal(const MsgSqueue_t &data);
public slots:
    void MsgRecDataSlot(MsgSqueue_t &data);
private slots:

    void on_return_Button_clicked();

    void on_comfirm_Button_clicked();

    void on_boardname_returnPressed();

    void on_wifikey_returnPressed();

    void on_scanpushButton_clicked();

private:
    Ui::parameter2 *ui;
private:
    MsgSqueue_t parameterSet;
};

#endif // PARAMETER2_H
