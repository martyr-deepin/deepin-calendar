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

    void previousMonth();
    void nextMonth();

protected:
    void wheelEvent(QWheelEvent *);

private:
    CalendarView * m_calendarView = nullptr;
    CalendarTitleBarWidget* m_calendarTitleBarWidget = nullptr;
    QFrame * m_contentBackground = nullptr;

    QFrame * m_animationContainer = nullptr;
    QLabel * m_fakeContent = nullptr;

    QPropertyAnimation * m_scrollAnimation = nullptr;

    void initUI();
    void initAnimation();
    void setupMenu();
    void slideMonth(bool next);
    QPixmap getCalendarSnapshot() const;
    QPixmap joint(QPixmap & top, QPixmap & bottom) const;
};

#endif // CALENDARWINDOW_H
