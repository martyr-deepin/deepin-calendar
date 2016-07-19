#include "calendarwindow.h"

#include <QFile>
#include <QDebug>
#include <QDBusConnection>

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
    if (!a.setSingleInstance("dde-calendar")) {
        qDebug() << "there's an dde-calendar instance running.";
        QProcess::execute("dbus-send --print-reply --dest=com.deepin.dde.Calendar "
                          "/com/deepin/dde/Calendar com.deepin.dde.Calendar.RaiseWindow");

        return 0;
    }

    a.setTheme("light");
    a.setStyleSheet(GetStyleSheetContent());

    CalendarWindow cw;
    cw.show();

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (dbus.registerService("com.deepin.dde.Calendar")) {
        dbus.registerObject("/com/deepin/dde/Calendar", &cw);
    }

    return a.exec();
}
