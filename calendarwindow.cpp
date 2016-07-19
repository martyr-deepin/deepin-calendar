#include "calendarwindow.h"
#include "dbuscalendar_adaptor.h"

#include <QDate>
#include <QVBoxLayout>
#include <QLabel>

#include <DMenu>
#include <DAboutDialog>

CalendarWindow::CalendarWindow() :
    DWindow(nullptr)
{
    initUI();
    new CalendarAdaptor(this);
}

void CalendarWindow::handleTodayButtonClicked()
{
    m_calendarView->setCurrentDate(QDate::currentDate());
}

void CalendarWindow::handleDateSelected(const QDate &date, const CaLunarDayInfo &)
{
    if (date == QDate::currentDate()) {
        m_calendarTitleBarWidget->hideCalendarIcon();
    } else {
        m_calendarTitleBarWidget->showCalendarIcon();
    }
}

void CalendarWindow::handleCurrentYearMonthChanged(int year, int month)
{
    QDate changedDate;
    changedDate.setDate(year, month,  QDate::currentDate().day());
    m_calendarView->setCurrentDate(changedDate);
}

void CalendarWindow::initUI()
{
    QFrame * contentBg = new QFrame;
    contentBg->setObjectName("CalendarBackground");
    contentBg->setStyleSheet("QFrame#CalendarBackground { "
                             "background:white;"
                             "}");
    contentBg->setFixedSize(700, 500);

    QLabel * sep = new QLabel(this);
    sep->setFixedSize(contentBg->width() - 6, 1);
    sep->setStyleSheet("QLabel { background : rgba(0, 0, 0, 20); }");

    m_calendarView = new CalendarView(contentBg);
    m_calendarView->setFixedSize(contentBg->size());

    m_calendarView->setCurrentDate(QDate::currentDate());

    // TODO: API change cause compile error, replace with equivalent API
    setTitlebarFixedHeight(40);
    setContentWidget(contentBg);
    setContentsMargins(QMargins(0, 0, 0, 0));

    m_calendarTitleBarWidget = new CalendarTitleBarWidget(this);
    m_calendarTitleBarWidget->setCurrentYearMonth(QDate::currentDate().year(),
                                                  QDate::currentDate().month());
    setTitlebarWidget(m_calendarTitleBarWidget);

    QHBoxLayout * mainLayout = qobject_cast<QHBoxLayout*>(layout());
    QVBoxLayout * contentLayout = new QVBoxLayout(this);
    contentLayout->addWidget(sep, 0, Qt::AlignHCenter);
    contentLayout->addWidget(contentBg);
    mainLayout->addLayout(contentLayout);

    connect(m_calendarView, &CalendarView::currentDateChanged,
            m_calendarTitleBarWidget, &CalendarTitleBarWidget::setCurrentYearMonth);
    connect(m_calendarView, &CalendarView::currentFestivalChanged,
            m_calendarTitleBarWidget, &CalendarTitleBarWidget::setFestival);
    connect(m_calendarView, &CalendarView::dateSelected,
            this, &CalendarWindow::handleDateSelected);
    connect(m_calendarTitleBarWidget, &CalendarTitleBarWidget::currentYearMonthChanged,
            this, &CalendarWindow::handleCurrentYearMonthChanged);
    connect(m_calendarTitleBarWidget, &CalendarTitleBarWidget::todayButtonClicked,
            this, &CalendarWindow::handleTodayButtonClicked);

    setupMenu();
}

void CalendarWindow::setupMenu()
{
    DMenu * menu = dbusMenu();
    DAction * aboutAction = menu->addAction(tr("About"));
    DAction * quitAction = menu->addAction(tr("Quit"));

    connect(menu, &DMenu::triggered, [this, aboutAction, quitAction](DAction *action){
        if (aboutAction == action) {
            DAboutDialog *about = new DAboutDialog(tr("Deepin Calendar"),
                                                   ":/resources/icon/deepin-calendar.svg",
                                                   ":/resource/icon/deepin-calendar.svg",
                                                   tr("Deepin Calendar"),
                                                   tr("Version:") + " 1.0",
                                                   "description.",
                                                   this);
            about->show();
        } else if (quitAction == action) {
            qApp->quit();
        }
    });
}
