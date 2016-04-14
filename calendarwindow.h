#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <DWindow>

DWIDGET_USE_NAMESPACE

class CalendarView;
class CalendarWindow : public DWindow
{
public:
    CalendarWindow();

private:
    CalendarView * m_calendarView = nullptr;

    void initUI();
};

#endif // CALENDARWINDOW_H
