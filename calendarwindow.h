#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <DWindow>

#include "calendarview.h"
#include "calendartitlebarwidget.h"

DWIDGET_USE_NAMESPACE

class CalendarWindow : public DWindow
{
    Q_OBJECT
public:
    CalendarWindow();

public slots:
    void handleTodayButtonClicked();
    void handleDateSelected(const QDate &date, const CaLunarDayInfo &detail);
    void handleCurrentYearMonthChanged(int year, int month);

private:
    CalendarView * m_calendarView = nullptr;
    CalendarTitleBarWidget* m_calendarTitleBarWidget = nullptr;

    void initUI();
    void setupMenu();
};

#endif // CALENDARWINDOW_H
