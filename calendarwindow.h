#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <DMainWindow>
#include <QLabel>
#include <QSettings>

#include "calendarview.h"

DWIDGET_USE_NAMESPACE

class InfoView;
class QPropertyAnimation;

class CalendarWindow : public DMainWindow
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

protected slots:
    void menuItemInvoked(QAction *action);

private:
    InfoView * m_infoView = nullptr;
    CalendarView * m_calendarView = nullptr;
    QFrame * m_contentBackground = nullptr;

    QFrame * m_animationContainer = nullptr;
    QLabel * m_fakeContent = nullptr;
    QLabel * m_icon;

    QPropertyAnimation * m_scrollAnimation = nullptr;

    void initUI();
    void initAnimation();
    void initDateChangeMonitor();
    void setupMenu();
    void slideMonth(bool next);
    QPixmap getCalendarSnapshot() const;
    QPixmap joint(QPixmap & top, QPixmap & bottom) const;
    void updateTime() const;
    void updateDate() const;

    QAction *m_monAction;
    QAction *m_tueAction;
    QAction *m_wedAction;
    QAction *m_thuAction;
    QAction *m_friAction;
    QAction *m_satAction;
    QAction *m_sunAction;

    QSettings *m_settings;
    QSettings *m_dateSettings;
};

#endif // CALENDARWINDOW_H
