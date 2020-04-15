#ifndef COMPANY_H
#define COMPANY_H

#include <QWidget>

namespace Ui {
class company;
}

class company : public QWidget
{
    Q_OBJECT

public:
    explicit company(QWidget *parent = nullptr);
    ~company();
signals:
    void deviceinfo_return_is_true();
private slots:
    void on_returnButton_clicked();

private:
    Ui::company *ui;
};

#endif // COMPANY_H
