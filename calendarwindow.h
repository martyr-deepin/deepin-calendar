#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <DWindow>

#include "calendartitlebarwidget.h"
DWIDGET_USE_NAMESPACE

class CalendarView;
class CalendarWindow : public DWindow
{
public:
    CalendarWindow();

private:
    CalendarView * m_calendarView = nullptr;
    CalendarTitleBarWidget* m_calendarTitleBarWidget = nullptr;
    void initUI();
};

#endif // CALENDARWINDOW_H
