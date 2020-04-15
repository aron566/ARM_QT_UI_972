#include <cstring>
#include "parameter.h"
#include "ui_parameter.h"
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
parameter::parameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::parameter)
{
    ui->setupUi(this);
    //聚焦排序
    InitTabOrder();
    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);

    //create 管理下级对象
    /*二级设置页面*/
    parameter2Ui = new parameter2;
    connect(parameter2Ui,&parameter2::parameter2_return_is_true,this,&parameter::show_this_window);
    connect(parameter2Ui,&parameter2::toEditBox ,this ,&parameter::processedit);

    //接收下级发送消息信号-->转发下级消息
    connect(parameter2Ui,&parameter2::SendMsgToSqueueSignal,this,&parameter::Send_MsgSqueueToBuff);
    //转发接收消息队列的数据
    connect(this,&parameter::RecDataSignal,parameter2Ui,&parameter2::MsgRecDataSlot);

    //    ui->serverip_1->setValidator(new QIntValidator(1, 1000, this));
    //    ui->serverip_1->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
}

parameter::~parameter()
{
    delete parameter2Ui;
    delete ui;
}
void parameter::InitTabOrder(void)
{
    parameter::setTabOrder(ui->clientip_1, ui->clientip_2);
    parameter::setTabOrder(ui->clientip_2, ui->clientip_3);
    parameter::setTabOrder(ui->clientip_3, ui->clientip_4);
    parameter::setTabOrder(ui->clientip_4, ui->year);
    parameter::setTabOrder(ui->year, ui->month);
    parameter::setTabOrder(ui->month, ui->day);
    parameter::setTabOrder(ui->day, ui->hour);
    parameter::setTabOrder(ui->hour, ui->min);
    parameter::setTabOrder(ui->min, ui->serverip_1);
    parameter::setTabOrder(ui->serverip_1, ui->serverip_2);
    parameter::setTabOrder(ui->serverip_2, ui->serverip_3);
    parameter::setTabOrder(ui->serverip_3, ui->serverip_4);
    parameter::setTabOrder(ui->serverip_4, ui->commport);
    parameter::setTabOrder(ui->commport, ui->comfirm_Button);
    parameter::setTabOrder(ui->comfirm_Button, ui->nextpage_Button);
    parameter::setTabOrder(ui->nextpage_Button, ui->return_Button);

}
/*solt*/
void parameter::on_return_Button_clicked()
{
    /*close self*/
    this->close();
    /*send signal to main window*/
    emit parameter_return_is_true();
}
void parameter::processedit(QWidget *pObject ,QLineEdit *pwidget,QString title ,QString edittext)
{
    emit toEditBox(pObject ,pwidget,title,edittext);
}
/*确认设置参数*/
void parameter::on_comfirm_Button_clicked()
{
    set_parameter();
}
/*处理收集当前参数*/
void parameter::set_parameter()
{
    ui->par_tips->setText("");
    clientipFormatConv();
    DateFormatConv();
    serveripFormatConv();
    commportFormatConv();
}

void parameter::SendMsgToSqueue(MSGQueueTYPE_t type,QString data)
{
    parameterSet.type = type;
    strcpy(parameterSet.databuff,data.toUtf8().data());
//    memcpy(parameterSet.databuff,data.toUtf8().data(),datePar.size());
    emit SendMsgToSqueueSignal(parameterSet);
}

//转发下级消息
void parameter::Send_MsgSqueueToBuff(const MsgSqueue_t &data)
{
    emit SendMsgToSqueueSignal(data);
}
void parameter::MsgRecDataSlot(MsgSqueue_t &data)
{
    QString par_tips = "";
    //队列消息-->是哪个页面的
    switch(data.type)
    {
    case READ_WIFI_SSID:
    case SET_WIFI_SSID:
    case SET_WIFI_KEY:
    case SET_BOARD_NAME_MSG:
        emit RecDataSignal(data);
        return;
    case SET_DATE_TIME_MSG:
        par_tips = "set Date ok\n";
        break;
    case SET_SERVER_IP_MSG:
        par_tips = "set serIP ok\n";
        break;
    case SET_CLIENT_IP_MSG:
        par_tips = "set cliIP ok\n";
        break;
    case SET_PORT_NUM_MSG:
        par_tips = "set port ok\n";
        break;
    default:
        break;
    }
    ui->par_tips->setText(ui->par_tips->text()+par_tips);
}
void parameter::clientipFormatConv()
{
    int temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0;
    bool errflag = false;
    QString par_tips = "";
    temp1 = ui->clientip_1->text().toInt();
    temp2 = ui->clientip_2->text().toInt();
    temp3 = ui->clientip_3->text().toInt();
    temp4 = ui->clientip_4->text().toInt();
    if(temp1 > 999 || temp2 > 999 || temp3 > 999 ||temp4 > 999
            || temp1 < 0 || temp2 < 0 || temp3 < 0|| temp4 < 0)
    {
        errflag = true;
    }
    else
    {
        temp1 = ui->clientip_1->text().size();
        temp2 = ui->clientip_2->text().size();
        temp3 = ui->clientip_3->text().size();
        temp4 = ui->clientip_4->text().size();
        if(temp1 == 0 || temp2 == 0 || temp3 == 0 || temp4 == 0)
        {
            errflag = true;
        }
        else
        {
            clientipPar = ui->clientip_1->text()+"."+ui->clientip_2->text()+".";
            clientipPar += ui->clientip_3->text()+"."+ui->clientip_4->text();
        }
    }
    if(errflag)
    {
        par_tips = "set clienip X\n";
        ui->par_tips->setText(ui->par_tips->text()+par_tips);
        return;
    }

    SendMsgToSqueue(SET_CLIENT_IP_MSG,clientipPar);
}

void parameter::serveripFormatConv()
{
    int temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0;
    bool errflag = false;
    QString par_tips = "";
    temp1 = ui->serverip_1->text().toInt();
    temp2 = ui->serverip_2->text().toInt();
    temp3 = ui->serverip_3->text().toInt();
    temp4 = ui->serverip_4->text().toInt();
    if(temp1 > 999 || temp2 > 999 || temp3 > 999 ||temp4 > 999
            || temp1 < 0 || temp2 < 0 || temp3 < 0|| temp4 < 0)
    {
        errflag = true;
    }
    else
    {
        temp1 = ui->serverip_1->text().size();
        temp2 = ui->serverip_2->text().size();
        temp3 = ui->serverip_3->text().size();
        temp4 = ui->serverip_4->text().size();
        if(temp1 == 0 || temp2 == 0 || temp3 == 0 || temp4 == 0)
        {
            errflag = true;
        }
        else
        {
            serveripPar = ui->serverip_1->text()+"."+ui->serverip_2->text()+".";
            serveripPar += ui->serverip_3->text()+"."+ui->serverip_4->text();
        }
    }
    if(errflag)
    {
        par_tips = "set serip X\n";
        ui->par_tips->setText(ui->par_tips->text()+par_tips);
        return;
    }

    SendMsgToSqueue(SET_SERVER_IP_MSG,serveripPar);
}
void parameter::DateFormatConv()
{
    int temp = 0;
    bool errflag = false;
    char date_temp[20];
    QString par_tips = "";
    //转为数字
    temp = ui->month->text().toInt();
    if(temp > 12 || temp <= 0)
    {
        errflag = true;
        par_tips = "set month X\n";
    }
    sprintf(date_temp,"%02d",temp);
    datePar = date_temp;
    temp = ui->day->text().toInt();
    if(temp > 31 || temp <= 0)
    {
        errflag = true;
        par_tips += "set day X\n";
    }
    sprintf(date_temp,"%02d",temp);
    datePar += date_temp;
    temp = ui->hour->text().toInt();
    if(temp > 24 || temp < 0)
    {
        errflag = true;
        par_tips += "set hour X\n";
    }
    sprintf(date_temp,"%02d",temp);
    datePar += date_temp;
    temp = ui->min->text().toInt();
    if(temp > 60 || temp < 0)
    {
        errflag = true;
        par_tips += "set min X\n";
    }
    sprintf(date_temp,"%02d",temp);
    datePar += date_temp;
    temp = ui->year->text().toInt();
    if(temp <= 0)
    {
        errflag = true;
        par_tips += "set year X\n";
    }
    sprintf(date_temp,"%04d",temp);
    datePar += date_temp;
    //date 040512422019 设置 2019年4月5号12点42分
    ui->par_tips->setText(ui->par_tips->text()+par_tips);
    if(errflag)
    {
        return;
    }

    SendMsgToSqueue(SET_DATE_TIME_MSG,datePar);
}
void parameter::commportFormatConv()
{
    int temp = 0;
    char date_temp[20];
    QString par_tips = "";
    //转为数字
    temp = ui->commport->text().toInt();
    if(temp <= 0)
    {
        par_tips = "set port X\n";
        ui->par_tips->setText(ui->par_tips->text()+par_tips);
        return;
    }
    sprintf(date_temp,"%d",temp);
    serverportPar = date_temp;

    SendMsgToSqueue(SET_PORT_NUM_MSG,serverportPar);
}

void parameter::on_nextpage_Button_clicked()
{
    this->setHidden(true);
    parameter2Ui->show();
}

void parameter::show_this_window()
{
    this->setHidden(false);
}

//输入本机ip调用键盘
void parameter::on_clientip_1_returnPressed()
{
    qDebug() << "send signal is pressed";
    emit toEditBox(this ,this->ui->clientip_1,"输入客户端IP_1",this->ui->clientip_1->text());
    this->hide();
}

void parameter::on_clientip_2_returnPressed()
{
    emit toEditBox(this ,this->ui->clientip_2,"输入客户端IP_2",this->ui->clientip_2->text());
    this->hide();
}

void parameter::on_clientip_3_returnPressed()
{
    emit toEditBox(this ,this->ui->clientip_3,"输入客户端IP_3",this->ui->clientip_3->text());
    this->hide();
}

void parameter::on_clientip_4_returnPressed()
{
    emit toEditBox(this ,this->ui->clientip_4,"输入客户端IP_4",this->ui->clientip_4->text());
    this->hide();
}

void parameter::on_year_returnPressed()
{
    emit toEditBox(this ,this->ui->year,"输入年",this->ui->year->text());
    this->hide();
}

void parameter::on_month_returnPressed()
{
    emit toEditBox(this ,this->ui->month,"输入月",this->ui->month->text());
    this->hide();
}

void parameter::on_day_returnPressed()
{
    emit toEditBox(this ,this->ui->day,"输入天",this->ui->day->text());
    this->hide();
}

void parameter::on_hour_returnPressed()
{
    emit toEditBox(this ,this->ui->hour,"输入时",this->ui->hour->text());
    this->hide();
}

void parameter::on_min_returnPressed()
{
    emit toEditBox(this ,this->ui->min,"输入分",this->ui->min->text());
    this->hide();
}

void parameter::on_serverip_1_returnPressed()
{
    emit toEditBox(this ,this->ui->serverip_1,"输入服务端IP_1",this->ui->serverip_1->text());
    this->hide();
}

void parameter::on_serverip_2_returnPressed()
{
    emit toEditBox(this ,this->ui->serverip_2,"输入服务端IP_2",this->ui->serverip_2->text());
    this->hide();
}

void parameter::on_serverip_3_returnPressed()
{
    emit toEditBox(this ,this->ui->serverip_3,"输入服务端IP_3",this->ui->serverip_3->text());
    this->hide();
}

void parameter::on_serverip_4_returnPressed()
{
    emit toEditBox(this ,this->ui->serverip_4,"输入服务端IP_4",this->ui->serverip_4->text());
    this->hide();
}

void parameter::on_commport_returnPressed()
{
    emit toEditBox(this ,this->ui->commport,"输入通讯端口",this->ui->commport->text());
    this->hide();
}
