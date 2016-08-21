#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <DWindow>

#include "calendarview.h"
#include "calendartitlebarwidget.h"

DWIDGET_USE_NAMESPACE

class InfoView;
class QPropertyAnimation;

class CalendarWindow : public DWindow
{
    Q_OBJECT
public:
    CalendarWindow();

public slots:
    void handleTodayButtonClicked();
    void handleCurrentYearMonthChanged(int year, int month);

    void previousMonth();
    void nextMonth();

protected:
    void wheelEvent(QWheelEvent *);

private:
    InfoView * m_infoView = nullptr;
    CalendarView * m_calendarView = nullptr;
    CalendarTitleBarWidget* m_calendarTitleBarWidget = nullptr;
    QFrame * m_contentBackground = nullptr;

    QFrame * m_animationContainer = nullptr;
    QLabel * m_fakeContent = nullptr;

    QPropertyAnimation * m_scrollAnimation = nullptr;

    void initUI();
    void initAnimation();
    void initDateChangeMonitor();
    void setupMenu();
    void slideMonth(bool next);
    QPixmap getCalendarSnapshot() const;
    QPixmap joint(QPixmap & top, QPixmap & bottom) const;
    void updateTime() const;
};

#endif // CALENDARWINDOW_H
