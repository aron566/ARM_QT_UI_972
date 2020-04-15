//实时状态页面
#include <QDebug>
#include "history.h"
#include "ui_history.h"
#include "widget.h"
history::history(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);
    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);

    //初始化变量
    timer = new QTimer(this);
    //连接信号槽
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
    //启动定时器
    timer->start(100);//每100ms触发一次

    //限制本页面跳转
    currentpage = RUNMODE_PAGE_1;
}

history::~history()
{
    delete timer;
    delete ui;
}


void history::timer_timeout()
{
    timer->stop();
}

/*接收消息队列信号槽*/
void history::MsgRecDataSlot(MsgSqueue_t &data)
{
    //匹配消息类型-->匹配消息通道号-->设置显示
    switch(data.type)
    {
    case READ_MAX_AD_DATA:
        Set_Max_ADlabel(data);
        break;
    case READ_MIN_AD_DATA:
        Set_Min_ADlabel(data);
        break;
    case READ_AVERAGE_AD_DATA:
        Set_Average_ADlabel(data);
        break;
    case READ_REALTIME_AD_DATA:
        Set_Realtime_ADlabel(data);
        break;
    case READ_WORK_MODE_MSG:
        Set_Workmode(data);
        break;
    default:
        break;
    }
}
/*当前最大AD值显示*/
void history::Set_Max_ADlabel(MsgSqueue_t &data)
{
     QString str;
     str.sprintf("%s",data.databuff);
     //通知曲线界面最大AD已改变
     if(str.toInt() != Ad_Max_Limit)
     {
        Ad_Max_Limit = str.toUInt();
        emit Nortify_Max_AD_Is_Changed(Ad_Max_Limit);
     }

     switch(data.channel)
     {
         case 0x01:
            ui->maxad_1->setText(str);
             break;
         case 0x02:
             ui->maxad_2->setText(str);
             break;
         case 0x03:
             ui->maxad_3->setText(str);
             break;
         case 0x04:
             ui->maxad_4->setText(str);
             break;
         case 0x05:
             ui->maxad_5->setText(str);
             break;
         case 0x06:
             ui->maxad_6->setText(str);
             break;
         case 0x07:
             ui->maxad_7->setText(str);
             break;
         case 0x08:
             ui->maxad_8->setText(str);
             break;
         case 0x09:
             ui->maxad_9->setText(str);
             break;
         case 0x0A:
             ui->maxad_10->setText(str);
             break;
         default:
             break;
     }
}

/*当前最小AD值显示*/
void history::Set_Min_ADlabel(MsgSqueue_t &data)
{
     QString str;
     str.sprintf("%s",data.databuff);
     switch(data.channel)
     {
         case 0x01:
             ui->minad_1->setText(str);
             break;
         case 0x02:
             ui->minad_2->setText(str);
             break;
         case 0x03:
             ui->minad_3->setText(str);
             break;
         case 0x04:
             ui->minad_4->setText(str);
             break;
         case 0x05:
             ui->minad_5->setText(str);
             break;
         case 0x06:
             ui->minad_6->setText(str);
             break;
         case 0x07:
             ui->minad_7->setText(str);
             break;
         case 0x08:
             ui->minad_8->setText(str);
             break;
         case 0x09:
             ui->minad_9->setText(str);
             break;
         case 0x0A:
             ui->minad_10->setText(str);
             break;
         default:
             break;
     }
}
/*当前平均AD值显示*/
void history::Set_Average_ADlabel(MsgSqueue_t &data)
{
     QString str;
     str.sprintf("%s",data.databuff);
     switch(data.channel)
     {
         case 0x01:
             ui->averagead_1->setText(str);
             break;
         case 0x02:
             ui->averagead_2->setText(str);
             break;
         case 0x03:
             ui->averagead_3->setText(str);
             break;
         case 0x04:
             ui->averagead_4->setText(str);
             break;
         case 0x05:
             ui->averagead_5->setText(str);
             break;
         case 0x06:
             ui->averagead_6->setText(str);
             break;
         case 0x07:
             ui->averagead_7->setText(str);
             break;
         case 0x08:
             ui->averagead_8->setText(str);
             break;
         case 0x09:
             ui->averagead_9->setText(str);
             break;
         case 0x0A:
             ui->averagead_10->setText(str);
             break;
         default:
             break;
     }
}
/*当前实时AD值显示*/
void history::Set_Realtime_ADlabel(MsgSqueue_t &data)
{
     QString str;
     str.sprintf("%s",data.databuff);
     switch(data.channel)
     {
         case 0x01:
             ui->realtimead_1->setText(str);
             break;
         case 0x02:
             ui->realtimead_2->setText(str);
             break;
         case 0x03:
             ui->realtimead_3->setText(str);
             break;
         case 0x04:
             ui->realtimead_4->setText(str);
             break;
         case 0x05:
             ui->realtimead_5->setText(str);
             break;
         case 0x06:
             ui->realtimead_6->setText(str);
             break;
         case 0x07:
             ui->realtimead_7->setText(str);
             break;
         case 0x08:
             ui->realtimead_8->setText(str);
             break;
         case 0x09:
             ui->realtimead_9->setText(str);
             break;
         case 0x0A:
             ui->realtimead_10->setText(str);
             break;
         default:
             break;
     }
}
/*当前工作模式显示*/
void history::Set_Workmode(MsgSqueue_t &data)
{
    QString str;
    str.sprintf("%s",data.databuff);
    QStringList strList = str.split(" ", QString::SkipEmptyParts);
    str.sprintf("%s",strList.value(0).toUtf8().data());
    ui->configstate->setText(str);
    str.sprintf("%s",strList.value(1).toUtf8().data());
    ui->calibrationstate->setText(str);
    str.sprintf("%s",strList.value(2).toUtf8().data());
    ui->updatestate->setText(str);
    str.sprintf("%s",strList.value(3).toUtf8().data());
    ui->ageingstate->setText(str);
    strList.clear();
    str.clear();
}
void history::on_return_Button_clicked()
{
    this->hide();
    emit history_return_is_true();
}


//update call page
void history::pagenum_changed(int page)
{
    set_pagetitle(page);
    switch(page)
    {
    case RUNMODE_PAGE_1:
        ui->CHANNEL_1Button->setFocus();
        break;
    case RUNMODE_PAGE_2:
        ui->CHANNEL_2Button->setFocus();
        break;
    case RUNMODE_PAGE_3:
        ui->CHANNEL_3Button->setFocus();
        break;
    case RUNMODE_PAGE_4:
        ui->CHANNEL_4Button->setFocus();
        break;
    case RUNMODE_PAGE_5:
        ui->CHANNEL_5Button->setFocus();
        break;
    case RUNMODE_PAGE_6:
        ui->CHANNEL_6Button->setFocus();
        break;
    case RUNMODE_PAGE_7:
        ui->CHANNEL_7Button->setFocus();
        break;
    case RUNMODE_PAGE_8:
        ui->CHANNEL_8Button->setFocus();
        break;
    case RUNMODE_PAGE_9:
        ui->CHANNEL_9Button->setFocus();
        break;
    case RUNMODE_PAGE10:
        ui->CHANNEL_10Button->setFocus();
        break;
    }
}

void history::set_pagetitle(int page)
{
    currentpage = page > 0?page:RUNMODE_PAGE_1;
    qDebug() << "History RecSignal: " << currentpage;
}

void history::on_CHANNEL_1Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_1);
}

void history::on_CHANNEL_2Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_2);
}

void history::on_CHANNEL_3Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_3);
}

void history::on_CHANNEL_4Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_4);
}

void history::on_CHANNEL_5Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_5);
}

void history::on_CHANNEL_6Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_6);
}

void history::on_CHANNEL_7Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_7);
}

void history::on_CHANNEL_8Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_8);
}

void history::on_CHANNEL_9Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE_9);
}

void history::on_CHANNEL_10Button_clicked()
{
    this->hide();
    emit show_waveSignal(RUNMODE_PAGE10);
}
