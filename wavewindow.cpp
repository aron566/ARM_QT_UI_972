//历史曲线页面
#include <QDebug>
#include "wavewindow.h"
#include "ui_wavewindow.h"
#include <QTimer>
#include <QFont>


waveWindow::waveWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waveWindow)
{
    ui->setupUi(this);

    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);

    //初始化聚焦顺序
    InitTabOrder();

    Init_Data();

    //限制本页面跳转
    currentpage = RUNMODE_PAGE_1;

    //create wave
    //构建折线系列
//    m_linSerie = new QLineSeries();
    splineSeries = new QSplineSeries(this);

//    //点阵图
//    scatterSeries1 = new QScatterSeries();
//    scatterSeries2 = new QScatterSeries();

//    //散点图(用于边框)
//    scatterSeries1->setMarkerShape(QScatterSeries::MarkerShapeCircle);//圆形的点
//    scatterSeries1->setBorderColor(QColor(21, 100, 255)); //离散点边框颜色
//    scatterSeries1->setBrush(QBrush(QColor(21, 100, 255)));//离散点背景色
//    scatterSeries1->setMarkerSize(12); //离散点大小

//    //散点图(用于中心)
//    scatterSeries2->setMarkerShape(QScatterSeries::MarkerShapeCircle);//圆形的点
//    scatterSeries2->setBorderColor(Qt::white);//边框颜色
//    scatterSeries2->setBrush(QBrush(Qt::white));//背景颜色
//    scatterSeries2->setMarkerSize(6);//点大小


    //为折线添加数据

    //构建图表对象
    m_chart = new QChart();

    //调节比列
    m_chart->zoom(1);

    //设置page&图表标题
    set_pagetitle();

    //将折线系列添加到图表
//    m_chart->addSeries(m_linSerie);
    m_chart->addSeries(splineSeries);

    //构建坐标轴
    //为图表添加默认的坐标轴
//    m_chart->createDefaultAxes();

    //设置坐标轴范围
//    m_axisX = new QValueAxis();
//    m_axisX->setRange(0 ,X_MAX_SEC);
    m_axisY = new QValueAxis(this);
    m_axisY->setRange(0 ,3000);

    //时间坐标轴X
    m_axisX = new QDateTimeAxis(this);
    m_axisX->setFormat("mm:ss");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    //初始化设置时间坐标轴范围
    m_axisX->setRange(GetDateTimeFromSec(static_cast<qint32>(QDateTime::currentSecsSinceEpoch()))
                      ,GetDateTimeFromSec(static_cast<qint32>(QDateTime::currentSecsSinceEpoch()+X_MAX_SEC)));

    //添加虚线
    m_axisY->setMinorTickCount(4);
//    m_axisX->setMinorTickCount(8);

    //设置XY轴标题
//    m_axisX->setTitleText("Time");
//    m_axisY->setTitleText("AD_Value");
    QFont font;
    font.setPointSize(20);//文字大小
    m_axisX->setTitleFont(font);
    m_axisY->setTitleFont(font);

    //标签格式：每个单位保留几位小数
    m_axisY->setLabelFormat("%.0f");
//    m_axisX->setLabelFormat("%.0f");

    //设置网格数量
    m_axisX->setTickCount(7);
    m_axisY->setTickCount(5);

    //将坐标轴与系列绑定
    m_chart->setAxisX(m_axisX,splineSeries);//obsolete code
    m_chart->setAxisY(m_axisY,splineSeries);//obsolete code
//    m_chart->addAxis(m_axisY,Qt::AlignLeft);//new style

    //隐藏图列
    m_chart->legend()->hide();

    //设置图表的主题
    m_chart->setTheme(QtCharts::QChart::ChartThemeBlueCerulean);

    //将图表绑定为视图
    ui->widget->setChart(m_chart);

    //优化锯齿
    ui->widget->setRenderHint(QPainter::Antialiasing,true);

    //初始化定时器
    m_timer = new QTimer(this);
    //连接信号槽
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timer_timeout()));
    //启动定时器
    m_timer->start(1000);//每1000ms触发一次
}

waveWindow::~waveWindow()
{
    delete ui;
}

void waveWindow::InitTabOrder(void)
{
    waveWindow::setTabOrder(ui->widget, ui->runmodeButton);
    waveWindow::setTabOrder(ui->runmodeButton, ui->lastButton);
    waveWindow::setTabOrder(ui->lastButton, ui->nextButton);
    waveWindow::setTabOrder(ui->nextButton, ui->return_Button);
}

void waveWindow::Init_Data(void)
{
    mLasttime = 0;
    /*每通道已记录数据总数，映射初始化*/
    for(uint32_t i = 1;i < WAVECHANNEL_NUM_MAX+1;i++)
    {
        memset(&ChannelpointCnt[i],0,sizeof(Channel_Record_t));
    }
}
void waveWindow::MsgRecDataSlot(MsgSqueue_t &data)
{
    static_cast<void>(data);
}

void waveWindow::Send_Database_AD_Value_slot(const QList<PontData_t>&queryData,const int current_read_channel,int resultcnt)
{
    //首数据时间
    long int current_last_time = queryData.last().time;
    qDebug() << "add data...." << current_read_channel << "time:"<< current_last_time << "cnt:" << resultcnt;
    struct tm *p_tm;
    p_tm = localtime(&current_last_time);
    printf( "UI--AD值记录时间:%02d-%02d-%02d-%02d-%02d-%02dS\n", p_tm->tm_year + 1900,
                    p_tm->tm_mon + 1, p_tm->tm_mday,p_tm->tm_hour,p_tm->tm_min,p_tm->tm_sec);
    //360秒数据记录，可能不足360个数据，可能重复359个数据只有1个新数据
    int i = 0;
    for(; i < resultcnt;i++)
    {
        //录入360分钟的不允许有重复秒的数据,上次时间小于当前录入时间说明是新数据
        if(ChannelpointCnt[current_read_channel].first_time < queryData.value(i).time || ChannelpointCnt[current_read_channel].first_time == 0)
        {
            if(ChannelpointCnt[current_read_channel].recordCnt_min >= X_MAX_MIN_SEC)
            {
                //去除旧数据，再追加一条新数据
                channelpoint_buff_min[current_read_channel].removeFirst();
                for(quint32 x = 0;x < X_MAX_MIN_SEC-1;x++)
                {
//非时间轴                    channelpoint_buff_min[current_read_channel].replace(x,QPointF(x,channelpoint_buff_min[current_read_channel].value(x).y()));
                }
                //追加数据
//非时间轴                channelpoint_buff_min[current_read_channel].append(QPointF(360*60-1,queryData.value(i).advalue));
                addOnePointToVector(&channelpoint_buff_min[current_read_channel], queryData.value(i).time, queryData.value(i).advalue);
            }
            else
            {
                if(ChannelpointCnt[current_read_channel].last_time < queryData.value(i).time)
                {
//非时间轴                channelpoint_buff_min[current_read_channel].append(QPointF(ChannelpointCnt[current_read_channel].recordCnt_min,queryData.value(i).advalue));
                    addOnePointToVector(&channelpoint_buff_min[current_read_channel], queryData.value(i).time, queryData.value(i).advalue);
                    //当前分钟通道记录数++
                    ChannelpointCnt[current_read_channel].recordCnt_min++;
                }
            }
        }
        //录入360秒数据，首次记录时间增加了1秒，则更新数据
        if(ChannelpointCnt[current_read_channel].first_time < queryData.value(i).time || ChannelpointCnt[current_read_channel].first_time == 0)
        {
            //是否已经记录了360秒数据，需要进行坐标轴扩大操作
            if(ChannelpointCnt[current_read_channel].recordCnt_sec >= X_MAX_SEC)
            {

                //去除旧数据，再追加一条新数据
                channelpoint_buff_sec[current_read_channel].removeFirst();
                for(quint32 x = 0;x < X_MAX_SEC-1;x++)
                {
//非时间轴                    channelpoint_buff_sec[current_read_channel].replace(x,QPointF(x,channelpoint_buff_sec[current_read_channel].at(x).y()));
                }
                //追加数据
//非时间轴                channelpoint_buff_sec[current_read_channel].append(QPointF(359,queryData.value(i).advalue));
                addOnePointToVector(&channelpoint_buff_sec[current_read_channel], queryData.value(i).time, queryData.value(i).advalue);
            }
            else
            {
                if(ChannelpointCnt[current_read_channel].last_time < queryData.value(i).time)
                {
//非时间轴                    channelpoint_buff_sec[current_read_channel].append(QPointF(ChannelpointCnt[current_read_channel].recordCnt_sec,queryData.value(i).advalue));
                    addOnePointToVector(&channelpoint_buff_sec[current_read_channel], queryData.value(i).time, queryData.value(i).advalue);
//                    qDebug() << "+++++join new data " << ChannelpointCnt[current_read_channel].recordCnt_sec << "date:" << queryData.value(i).time << "AD " << queryData.value(i).advalue;
                    //当前秒通道记录数++
                    ChannelpointCnt[current_read_channel].recordCnt_sec++;
                }
            }
        }
    }

    //更新首次记录时间
    ChannelpointCnt[current_read_channel].first_time = queryData.value(0).time;
    //更新最后一次时间，为最新数据时间
    ChannelpointCnt[current_read_channel].last_time = queryData.value(resultcnt-1).time;
}
void waveWindow::on_runmodeButton_clicked()
{
    this->hide();
    emit show_runmodeSignal(currentpage);
}

void waveWindow::on_lastButton_clicked()
{
    if(currentpage <= RUNMODE_PAGE_1)
    {
        currentpage = RUNMODE_PAGE10;
    }
    else
    {
        currentpage--;
    }
    qDebug() << "page:" << currentpage;
    set_pagetitle(currentpage);
}

void waveWindow::on_nextButton_clicked()
{
    if(currentpage >= RUNMODE_PAGE10)
    {
        currentpage = RUNMODE_PAGE_1;
    }
    else
    {
        currentpage++;
    }
    qDebug() << "page:" << currentpage;
    set_pagetitle(currentpage);
}

void waveWindow::on_return_Button_clicked()
{
    this->hide();
    emit wave_return_is_true();
}

void waveWindow::pagenum_changed(int page)
{
    set_pagetitle(page);
    if(ui->widget->hasFocus())
    {
        qDebug() << "widget IS FOUCUS";
    }
}
void waveWindow::set_pagetitle(int page)
{
    currentpage = page > 0?page:RUNMODE_PAGE_1;
    qDebug() << "WAVE RecSignal: " << currentpage;
    QString title = QString("通道%1信息").arg(currentpage);
    ui->wavechannellabel->setText(title);
    ui->wavechannellabel->setAlignment(Qt::AlignCenter);
    //设置图表标题
//    m_chart->setTitle(title);
}

//360sec
void waveWindow::update_pagewavedata()
{
    //当前的页面当前的曲线
    splineSeries->replace(channelpoint_buff_sec[currentpage]);
}
//360min
void waveWindow::update_pagewavedata_min()
{
    //当前的页面当前的曲线
    splineSeries->replace(channelpoint_buff_min[currentpage]);
}
void waveWindow::addOnePointToChart(QSplineSeries *targetSeries, int secondCount, int y_value)
{
    //将秒转换为QDateTime格式
    QDateTime temp_AddTimePos(QDate(2000, 1, 1), QTime(0, 0, 0).addSecs(secondCount));//生成一个坐标轴时间QDateTime
    //在曲线上，添加一个点数据
    targetSeries->append(temp_AddTimePos.toMSecsSinceEpoch(), y_value);
}

void waveWindow::addOnePointToVector(QVector<QPointF> *targetList, int secondCount, int y_value)
{
    //将秒转换为QDateTime格式
    QDateTime temp_AddTimePos(QDate(2000, 1, 1), QTime(0, 0, 0).addSecs(secondCount));//生成一个坐标轴时间QDateTime
    //在向量，添加一个点数据
    targetList->append(QPointF(temp_AddTimePos.toMSecsSinceEpoch(), y_value));
}

QDateTime waveWindow::GetDateTimeFromSec(int secondCount)
{
    //将秒转换为QDateTime格式
    QDateTime temp_AddTimePos(QDate(2000, 1, 1), QTime(0, 0, 0).addSecs(secondCount));//生成一个坐标轴时间QDateTime
    return temp_AddTimePos;
}
void waveWindow::timer_timeout()
{
//    static QTime time_begin = QTime::currentTime();
//    //经过的时间
//    qint64 elapsed_time = time_begin.elapsed();

    accumulative_sec++;
    //360分钟内只更新秒数
    if(accumulative_sec < X_MAX_MIN_SEC)
    {
        if(accumulative_sec >= X_MAX_SEC && accumulative_sec < X_MAX_MIN_SEC)
        {
            m_axisX->setRange(GetDateTimeFromSec(static_cast<qint32>(QDateTime::currentSecsSinceEpoch()-X_MAX_SEC))
                            ,GetDateTimeFromSec(static_cast<qint32>(QDateTime::currentSecsSinceEpoch())));
        }
        //当前sec的页面当前的曲线
        update_pagewavedata();
    }
    //大于360分钟了，则设定X范围为360*60 秒的范围
    else if(accumulative_sec >= X_MAX_MIN_SEC)
    {
        m_axisX->setRange(GetDateTimeFromSec(static_cast<qint32>(QDateTime::currentSecsSinceEpoch()-X_MAX_MIN_SEC))
                          ,GetDateTimeFromSec(static_cast<qint32>(QDateTime::currentSecsSinceEpoch())));
        //当前min的页面当前的曲线
        update_pagewavedata_min();
    }

}

//获取Y轴当前最大值,决定是否更新坐标
void waveWindow::Rec_Max_AD_Is_ChangedSlot(qint32 &Max_Ad)
{
    if(Max_Ad > Current_Coordinate_MAX_Y)
    {
        Current_Coordinate_MAX_Y = 2*Max_Ad;
        m_axisY->setRange(0 ,Current_Coordinate_MAX_Y);
    }
}

/*移动坐标轴的方式：
1、X轴不变：X固定为0-359区间，第一次数据在X轴溢出，则踢掉X轴0点的数据和对应的Y轴数据
           后面的数据：原X轴1-358点的数据，其X坐标点减1操作，Y不变
           最后新增一条数据在359点上，实现数据的曲线的平移效果
2、X轴动态改变：X固定在360个点的范围，第一次数据在X轴溢出，则踢掉X轴最小点的数据和对应的Y轴数据
           更改曲线坐标X轴范围：X原点应为原坐标轴第二个点X坐标，X轴终点应为X原点确定后+359
           最后新增一条数据在X轴终点
*/
