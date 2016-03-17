#include "calendarwindow.h"
#include "calendarview.h"

#include <QDate>
#include <QVBoxLayout>

CalendarWindow::CalendarWindow() :
    DWindow(nullptr)
{
    initUI();
}

void CalendarWindow::initUI()
{
    QFrame * contentBg = new QFrame;
    contentBg->setObjectName("CalendarBackground");
    contentBg->setStyleSheet("QFrame#CalendarBackground { background-color: rgba(220, 220, 220, 200) }");
    contentBg->setFixedSize(480, 360);

    m_calendarView = new CalendarView(contentBg);
    m_calendarView->setFixedSize(contentBg->size());
    m_calendarView->setCurrentDate(QDate::currentDate());

    QLayout * mainLayout = layout();
    mainLayout->addWidget(contentBg);
}
