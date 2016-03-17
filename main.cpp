#include "calendarwindow.h"

#include "dapplication.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);

    CalendarWindow cw;
    cw.show();

    return a.exec();
}
