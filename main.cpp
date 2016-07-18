#include "calendarwindow.h"

#include <QFile>
#include <QDebug>

#include "dapplication.h"

DWIDGET_USE_NAMESPACE

QString GetStyleSheetContent() {
    QFile file(":/resources/dde-calendar.qss");
    bool result = file.open(QFile::ReadOnly);
    if (result) {
        QString content(file.readAll());
        file.close();
        return content;
    } else {
        return "";
    }
}

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setTheme("light");
    a.setStyleSheet(GetStyleSheetContent());

    CalendarWindow cw;
    cw.show();

    return a.exec();
}
