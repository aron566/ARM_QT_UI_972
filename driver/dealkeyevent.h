#ifndef DEALKEYEVENT_H
#define DEALKEYEVENT_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QKeyEvent>
class dealKeyEvent : public QObject
{
    Q_OBJECT
public:
    explicit dealKeyEvent(QObject *parent = nullptr);

signals:

public slots:
    void on_rightButton_clicked(); //send virtual right key
    void on_closeButton_clicked(); //send virtual close key
    void on_leftButton_clicked();  //send virtual left key
    void on_tabButton_clicked();
    void on_spaceButton_clicked();
    void on_key_pressed(int);
    void on_key_release(int);
public:

private:
    int timerID;
private:
    void timerEvent(QTimerEvent *event) override;
};

#endif // DEALKEYEVENT_H
