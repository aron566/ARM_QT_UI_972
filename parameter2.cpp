#include "parameter2.h"
#include "ui_parameter2.h"
#include <QDebug>
parameter2::parameter2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::parameter2)
{
    ui->setupUi(this);
    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);
    //设置聚焦
    InitTabOrder();
    //密文输入
    ui->wifikey->setEchoMode(QLineEdit::Password);
    //tips
    ui->wifikey->setPlaceholderText("Please enter >= 8 characters");
}

parameter2::~parameter2()
{
    delete ui;
}
void parameter2::InitTabOrder(void)
{
    parameter2::setTabOrder(ui->boardname, ui->wifissid);
    parameter2::setTabOrder(ui->wifissid, ui->scanpushButton);

    parameter2::setTabOrder(ui->scanpushButton, ui->wifikey);
    parameter2::setTabOrder(ui->wifikey, ui->comfirm_Button);
    parameter2::setTabOrder(ui->comfirm_Button, ui->return_Button);
}
void parameter2::on_return_Button_clicked()
{
    /*close self*/
    this->close();
    /*send signal to main window*/
    emit parameter2_return_is_true();
}

void parameter2::on_comfirm_Button_clicked()
{
    set_parameter();
}
/*处理收集当前参数*/
void parameter2::set_parameter()
{
    ui->par_tips->setText("");
    boardnameFormatConv();
//    wifissidFormatConv();
//    wifikeyFormatConv();
}
void parameter2::SendMsgToSqueue(MSGQueueTYPE_t type,QString data)
{
    parameterSet.type = type;
    strcpy(parameterSet.databuff,data.toUtf8().data());
//    memcpy(parameterSet.databuff,data.toUtf8().data(),datePar.size());
    emit SendMsgToSqueueSignal(parameterSet);
}

void parameter2::MsgRecDataSlot(MsgSqueue_t &data)
{
    QString par_tips = "";
    switch(data.type)
    {
    case READ_WIFI_SSID:
        Add_wifissid_ToItem(data);
        break;
    case SET_WIFI_SSID:
    case SET_WIFI_KEY:
        par_tips = "set wifi ok\n";
        break;
    case SET_BOARD_NAME_MSG:
        par_tips = "set name ok\n";
        break;
    default:
        break;
    }
    ui->par_tips->setText(ui->par_tips->text()+par_tips);
}

void parameter2::Add_wifissid_ToItem(MsgSqueue_t &data)
{
    QString str;
    str.sprintf("%s",data.databuff);
    ui->wifissid->clear();
    ui->scanpushButton->setText("扫描");
    QStringList strList = str.split(" ", QString::SkipEmptyParts);
    ui->wifissid->addItems(strList);
    strList.clear();
    str.clear();
}
void parameter2::boardnameFormatConv()
{
    int len = 0;
    QString boardnamePar;
    QString par_tips = "";
    len = ui->boardname->text().size();
    if(len > 63 || len == 0)
    {
        par_tips = "set name len err\n";
        ui->par_tips->setText(ui->par_tips->text()+par_tips);
        return;
    }
    else
    {
        boardnamePar = ui->boardname->text();
    }

    SendMsgToSqueue(SET_BOARD_NAME_MSG,boardnamePar);
}
void parameter2::wifissidFormatConv()
{
    int len = 0;
    QString wifissid;
    QString par_tips = "";
    len = ui->wifissid->currentText().size();
    if(len > 63 || len == 0)
    {
        par_tips = "set wifi ssid err\n";
        ui->par_tips->setText(ui->par_tips->text()+par_tips);
        return;
    }
    else
    {
        wifissid = ui->wifissid->currentText();
    }

    SendMsgToSqueue(SET_WIFI_SSID,wifissid);
}
void parameter2::wifikeyFormatConv()
{
    int len = 0;
    QString wifikey;
    QString par_tips = "";
    len = ui->wifikey->text().size();
    if(len > 63 || len == 0)
    {
        par_tips = "set wifi key err\n";
        ui->par_tips->setText(ui->par_tips->text()+par_tips);
        return;
    }
    else
    {
        wifikey = ui->wifikey->text();
    }

    SendMsgToSqueue(SET_WIFI_KEY,wifikey);
}
void parameter2::on_boardname_returnPressed()
{
    emit toEditBox(this ,this->ui->boardname,"输入工装板名称",this->ui->boardname->text());
    this->hide();
}

void parameter2::on_wifikey_returnPressed()
{
    emit toEditBox(this ,this->ui->wifikey,"输入WIFI密码",this->ui->wifikey->text());
    this->hide();
}

void parameter2::on_scanpushButton_clicked()
{
    QString str = "scan";
    ui->scanpushButton->setText("扫描中");
    SendMsgToSqueue(READ_WIFI_SSID,str);
}
