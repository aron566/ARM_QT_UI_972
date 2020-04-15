#include "company.h"
#include "ui_company.h"

company::company(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::company)
{
    ui->setupUi(this);
    //去掉windows关闭窗口按钮
    this->setWindowFlag(Qt::FramelessWindowHint);
}

company::~company()
{
    delete ui;
}

void company::on_returnButton_clicked()
{
    this->hide();
    emit deviceinfo_return_is_true();
}
