#include "widget.h"
#include <QApplication>
#include <QFontDatabase>
#include <qtextcodec.h>
int main(int argc, char *argv[])
{
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication a(argc, argv);

    int id = QFontDatabase::addApplicationFont("/usr/5.9qtlib/fonts/Ubuntu-R.ttf");
//    int id = QFontDatabase::addApplicationFont("/usr/5.9qtlib/fonts/song.ttf");

    QString msyh = QFontDatabase::applicationFontFamilies(id).value(0);
    QFont font(msyh,10);
    qDebug()<<msyh<<endl;

    font.setPointSize(36);
    a.setFont(font);

    Widget w;
    w.show();
    return a.exec();
}
