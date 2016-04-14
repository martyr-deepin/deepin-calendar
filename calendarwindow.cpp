#include "calendarwindow.h"
#include "calendarview.h"

#include <QDate>
#include <QVBoxLayout>
#include <QLabel>

CalendarWindow::CalendarWindow() :
    DWindow(nullptr)
{
    initUI();
}

void CalendarWindow::initUI()
{
    QFrame * contentBg = new QFrame;
    contentBg->setObjectName("CalendarBackground");
    contentBg->setFixedSize(700, 500);

    m_calendarView = new CalendarView(contentBg);
    m_calendarView->setFixedSize(contentBg->size());

    m_calendarView->setCurrentDate(QDate::currentDate());

    // TODO: API change cause compile error, replace with equivalent API
    //setTitleFixedHeight(40);
    setContentWidget(contentBg);
    setContentsMargins(QMargins(0, 0, 0, 0));

    m_calendarTitleBarWidget = new CalendarTitleBarWidget(this);
    setTitlebarWidget(m_calendarTitleBarWidget);
    m_calendarTitleBarWidget->setCurrentYearMonth(QDate::currentDate().year(),
                                                  QDate::currentDate().month());
    QLayout * mainLayout = layout();
    mainLayout->addWidget(contentBg);
}
