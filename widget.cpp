#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDateTime>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    //聚焦排序
    InitTabOrder();
    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);

    //创建下级参数设置界面
    parameterUi = new parameter();
    //创建下级历史数据窗口
    historyUi = new history();
    //创建下级图表显示窗口
    waveWindowui = new waveWindow();
    //创建键盘显示页面(接收其他窗口信号，回调显示)
    mykeyboardUi = new mykeyboard();
    //产品详情显示窗口
    deviceinfoUi = new company();
    //创建线程
    MyThreadController = new ThreadController(this);
    //启动数据遍历线程
    MyThreadController->start_databaseTask();
    //启动消息队列发送线程
    MyThreadController->start_SendMsgTask();
    //启动按键检测线程
    MyThreadController->start_keycheckTask();
    //连接消息队列消息
    connect(MyThreadController,&ThreadController::RecDataSignal,this,&Widget::MsgRecDataSlot);
    //连接AD值到曲线界面
    connect(MyThreadController,&ThreadController::Send_Database_AD_Value_Signal,waveWindowui,&waveWindow::Send_Database_AD_Value_slot);

    dealkeyeventobj = new dealKeyEvent();
    connect(MyThreadController,&ThreadController::is_keypressSignal,dealkeyeventobj,&dealKeyEvent::on_key_pressed);
    connect(MyThreadController,&ThreadController::is_keyreleaseSignal,dealkeyeventobj,&dealKeyEvent::on_key_release);

    //初始化定时器
    timer = new QTimer(this);
    //连接信号槽
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
    //启动定时器
    timer->start(500);//每200ms触发一次
    //时间显示更新定时器
    Sectimer = new QTimer(this);
    //连接信号槽
    connect(Sectimer,SIGNAL(timeout()),this,SLOT(updateTimeNow()));
    //启动定时器
    Sectimer->start(1000);//每1000ms触发一次

    //接收下级发送消息信号
    connect(parameterUi,&parameter::SendMsgToSqueueSignal,this,&Widget::Send_MsgSqueueToBuff);
    //转发下级消息
    connect(this,&Widget::SendMsgToSqueueSignal,MyThreadController,&ThreadController::Send_MsgSqueueToBuff);
    //接收下级收到消息信号，转发相应界面
    connect(this,&Widget::RecPage1_DataSignal,parameterUi,&parameter::MsgRecDataSlot);
    connect(this,&Widget::RecPage2_DataSignal,historyUi,&history::MsgRecDataSlot);
    connect(this,&Widget::RecPage3_DataSignal,waveWindowui,&waveWindow::MsgRecDataSlot);
    connect(historyUi,&history::Nortify_Max_AD_Is_Changed,waveWindowui,&waveWindow::Rec_Max_AD_Is_ChangedSlot);

    //返回主窗口
    connect(historyUi,&history::history_return_is_true,this,&Widget::show_this_window);
    connect(waveWindowui,&waveWindow::wave_return_is_true,this,&Widget::show_this_window);
    connect(parameterUi,&parameter::parameter_return_is_true,this,&Widget::show_this_window);
    connect(deviceinfoUi,&company::deviceinfo_return_is_true,this,&Widget::show_this_window);

    //转发调用曲线窗口需求
    connect(this,&Widget::show_waveSignal,waveWindowui,&waveWindow::pagenum_changed);
    connect(historyUi,&history::show_waveSignal,this,&Widget::show_wavepage);
    //show historyui
    connect(this,&Widget::show_runmodeSignal,historyUi,&history::pagenum_changed);
    connect(waveWindowui,&waveWindow::show_runmodeSignal,this,&Widget::show_runmodepage);

    //编辑模式-->调用键盘
    connect(parameterUi,&parameter::toEditBox ,this ,&Widget::processedit);
    //退出编辑模式-->显示上级页面
    connect(mykeyboardUi,&mykeyboard::editisModifiedok,this,&Widget::show_parameterUi);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitTabOrder(void)
{
    Widget::setTabOrder(ui->Par_Button, ui->Monitor_Button);
    Widget::setTabOrder(ui->Monitor_Button, ui->HistroryButton);
    Widget::setTabOrder(ui->HistroryButton, ui->Reboot_Button);
    Widget::setTabOrder(ui->Reboot_Button, ui->About_Button);
}
void Widget::paintEvent(QPaintEvent *event)
{
    //clear alarm
    static_cast<void>(event);
    //界面刷新时执行任务，将点连成线
    //准备一支笔
    QPainter painter(this);
    //设置属性
    QPen pen;
    //属性：2像素宽
    pen.setWidth(2);
    //属性：红颜色
    pen.setBrush(QBrush(Qt::red));
    painter.setPen(pen);

    painter.setRenderHint(QPainter::Antialiasing);
    int i;
    for(i = 1;i < Point_array.size();i++)
    {
        QPoint p1 = Point_array[i-1];
        QPoint p2 = Point_array[i];
        //连线
        painter.drawLine(p1,p2);
    }
}

void Widget::timer_timeout()
{
    //定时到执行一次，将后面的y给前面,最后面加入新的y
    static int x = ui->HistroryButton->x()+5;
    int y_offset = this->height()-ui->HistroryButton->height()-ui->HistroryButton->height()-5;
    int y;

    if(x < ui->HistroryButton->width()+ui->HistroryButton->x())
    {
        y = rand()%ui->HistroryButton->height();
        y += y_offset;
        Point_array.push_back(QPoint(x,y));
        x += 2;
    }
    else    //超出界面范围,将新数据给老数据，最右边的加入新的y
    {
        int i;
        for(i = 1;i <Point_array.size();i++)
        {
            Point_array[i-1].setY(Point_array[i].y());
        }
        y = rand()%ui->HistroryButton->height();
        y += y_offset;
        Point_array[Point_array.size()-1].setY(y);
    }
    this->update();
}

/*显示wave窗口*/
void Widget::on_HistroryButton_clicked()
{
    this->setHidden(true);
    //显示wave窗口
    waveWindowui->show();
    emit show_waveSignal(MAIN_PAGE_NUM);
}
/*显示数据窗口*/
void Widget::on_Monitor_Button_clicked()
{
    this->setHidden(true);
    //显示历史数据窗口
    historyUi->show();
    emit show_runmodeSignal(MAIN_PAGE_NUM);
}

/*显示参数界面*/
void Widget::on_Par_Button_clicked()
{
    this->setHidden(true);
    parameterUi->show();
}

/*重启按钮点击*/
void Widget::on_Reboot_Button_clicked()
{
    system("reboot");
}

/*显示硬件信息*/
void Widget::on_About_Button_clicked()
{
    this->hide();
    deviceinfoUi->show();
}

void Widget::show_this_window()
{
    this->setHidden(false);
}


/*响应编辑框点击*/
void Widget::processedit(QWidget *pObject ,QLineEdit *pwidget,QString title ,QString edittext)
{
    qDebug() << "recived the signal is processing now--" << title;
    pLastCallobj = pObject;
    pLastCallwidget = pwidget;
    mykeyboardUi->show();
    mykeyboardUi->set_editBox(edittext);
    mykeyboardUi->set_editTips(title);
}

/*显示参数配置页面*/
void Widget::show_parameterUi(QString str)
{
    pLastCallobj->show();
    pLastCallwidget->setText(str);
}

QPixmap Widget::ChangeImageColor(QPixmap sourcePixmap, QColor origColor, QColor destColor)
{
    QImage image = sourcePixmap.toImage();
    for(int w = 0;w < image.width();++w)
        for(int h = 0; h < image.height();++h)
        {
            QRgb rgb = image.pixel(w,h);
            if(rgb == origColor.rgb())
            {
                //替换颜色
                image.setPixel(w,h,destColor.rgba());
            }
        }
    return QPixmap::fromImage(image);
}

QString Widget::GetupdateTime()
{
    datetimeStr = QDateTime::currentDateTime().toString(Qt::ISODate);
    QDateTime dateTime = QDateTime::fromString(datetimeStr, "yyyy-MM-dd hh:mm:ss");//字符串转时间
    datetimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");//时间转指定格式字符串
    return datetimeStr;
}

//per one second
void Widget::updateTimeNow()
{
    this->ui->date_time->setText(GetupdateTime());
}

void Widget::Send_MsgSqueueToBuff(const MsgSqueue_t &data)
{
    emit SendMsgToSqueueSignal(data);
}

/*切换至曲线界面*/
void Widget::show_wavepage(int page)
{
    emit show_waveSignal(page);
    waveWindowui->show();
}

/*切换至实时数据界面*/
void Widget::show_runmodepage(int page)
{
    emit show_runmodeSignal(page);
    historyUi->show();
}

//队列消息-->是哪个页面的
void Widget::MsgRecDataSlot(MsgSqueue_t &data)
{
    switch(data.type)
    {
    case SET_BOARD_NAME_MSG:
    case SET_DATE_TIME_MSG:
    case SET_SERVER_IP_MSG:
    case SET_CLIENT_IP_MSG:
    case SET_PORT_NUM_MSG:
    case READ_WIFI_SSID:
    case SET_WIFI_SSID:
    case SET_WIFI_KEY:
        emit RecPage1_DataSignal(data);
        break;
    case READ_WORK_MODE_MSG:
    case READ_MAX_AD_DATA:
    case READ_MIN_AD_DATA:
    case READ_AVERAGE_AD_DATA:
    case READ_REALTIME_AD_DATA:
        emit RecPage2_DataSignal(data);
        break;
    case READ_HISTRORY_DATA:
        emit RecPage3_DataSignal(data);
        break;
    default:
        break;
    }
}

