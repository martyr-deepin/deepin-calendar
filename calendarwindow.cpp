#include "calendarwindow.h"
#include "dbuscalendar_adaptor.h"
#include "constants.h"

#include <QDate>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include <DMenu>
#include <DAboutDialog>

const int CalendarHeaderHeight = 60;

CalendarWindow::CalendarWindow() :
    DWindow(nullptr)
{
    setWindowTitle(tr("Calendar"));
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setTitlebarFixedHeight(40);
    setContentsMargins(QMargins(0, 0, 0, 0));

    initUI();
    initAnimation();
    initDateChangeMonitor();

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

void CalendarWindow::previousMonth()
{
    slideMonth(false);
}

void CalendarWindow::nextMonth()
{
    slideMonth(true);
}

void CalendarWindow::wheelEvent(QWheelEvent * e)
{
    if (e->delta() < 0) {
        nextMonth();
    } else {
        previousMonth();
    }
}

void CalendarWindow::initUI()
{
    m_contentBackground = new QFrame;
    m_contentBackground->setObjectName("CalendarBackground");
    m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
                             "background:white;"
                             "}");
    m_contentBackground->setFixedSize(760, 500);

    setFixedSize(m_contentBackground->width() + 4,
                 m_contentBackground->height() + titlebarHeight() + 6);
    setContentWidget(m_contentBackground);

    m_calendarTitleBarWidget = new CalendarTitleBarWidget(this);
    m_calendarTitleBarWidget->setCurrentYearMonth(QDate::currentDate().year(),
                                                  QDate::currentDate().month());
    setTitlebarWidget(m_calendarTitleBarWidget);

    // Separator between title bar and calendar content.
    QLabel * sep = new QLabel(this);
    sep->setFixedSize(m_contentBackground->width() - 6, 1);
    sep->setStyleSheet("QLabel { background : rgba(0, 0, 0, 20); }");

    m_calendarView = new CalendarView(m_contentBackground);
    m_calendarView->setFixedSize(m_contentBackground->size());
    m_calendarView->setCurrentDate(QDate::currentDate());

    m_animationContainer = new QFrame(m_contentBackground);
    m_animationContainer->setStyleSheet("QFrame { background: rgba(0, 0, 0, 0) }");
    m_animationContainer->setFixedSize(m_calendarView->width(),
                                       m_calendarView->height() - CalendarHeaderHeight);
    m_animationContainer->move(0, CalendarHeaderHeight);
    m_animationContainer->hide();

    m_fakeContent = new QLabel(m_animationContainer);
    m_fakeContent->setStyleSheet("QLabel { background: rgba(0, 0, 0, 0) }");
    m_fakeContent->setFixedSize(m_animationContainer->width(),
                                m_animationContainer->height() * 2);

    QHBoxLayout * mainLayout = qobject_cast<QHBoxLayout*>(layout());
    QVBoxLayout * contentLayout = new QVBoxLayout(this);
    contentLayout->addWidget(sep, 0, Qt::AlignHCenter);
    contentLayout->addWidget(m_contentBackground);
    mainLayout->addLayout(contentLayout);

    connect(m_calendarView, &CalendarView::currentDateChanged,
            m_calendarTitleBarWidget, &CalendarTitleBarWidget::setCurrentYearMonth);
    connect(m_calendarView, &CalendarView::currentFestivalChanged,
            m_calendarTitleBarWidget, &CalendarTitleBarWidget::setFestival);
    connect(m_calendarView, &CalendarView::dateSelected,
            this, &CalendarWindow::handleDateSelected);
    connect(m_calendarTitleBarWidget, &CalendarTitleBarWidget::currentYearMonthChanged,
            this, &CalendarWindow::handleCurrentYearMonthChanged, Qt::DirectConnection);
    connect(m_calendarTitleBarWidget, &CalendarTitleBarWidget::todayButtonClicked,
            this, &CalendarWindow::handleTodayButtonClicked);

    setupMenu();
}

void CalendarWindow::initAnimation()
{
    m_scrollAnimation = new QPropertyAnimation(m_fakeContent, "pos");
    m_scrollAnimation->setDuration(300);

    connect(m_scrollAnimation, &QPropertyAnimation::finished, [this]{
        m_animationContainer->hide();
    });
}

void CalendarWindow::initDateChangeMonitor()
{
    static QDate LastCurrentDate = QDate::currentDate();

    QTimer * timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, [this] {
        QDate currentDate = QDate::currentDate();
        if (LastCurrentDate != currentDate) {
            LastCurrentDate = currentDate;
            m_calendarView->setCurrentDate(currentDate);
        }
    });
    timer->start();
}

void CalendarWindow::setupMenu()
{
    DMenu * menu = dbusMenu();
    DAction * aboutAction = menu->addAction(tr("About"));
    DAction * quitAction = menu->addAction(tr("Quit"));

    connect(menu, &DMenu::triggered, [this, aboutAction, quitAction](DAction *action){
        if (aboutAction == action) {
            DAboutDialog *about = new DAboutDialog(tr("Calendar"),
                                                   QString(":/resources/icon/deepin-calendar_48.png"),
                                                   QString(":/resources/icon/deepin-calendar_96.png"),
                                                   tr("Deepin Calendar"),
                                                   tr("Version: %1").arg(qApp->applicationVersion()),
                                                   tr("Calendar is a date tool."),
                                                   this);
            about->show();
        } else if (quitAction == action) {
            qApp->quit();
        }
    });
}

void CalendarWindow::slideMonth(bool next)
{
    m_animationContainer->show();

    QPixmap one = getCalendarSnapshot();
    m_calendarTitleBarWidget->setMonthIncrease(next);
    QPixmap two = getCalendarSnapshot();
    QPixmap target = next ? joint(one, two) : joint(two, one);
    m_fakeContent->setPixmap(target);

    m_scrollAnimation->setStartValue(QPoint(0, next ? 0 : -one.height()));
    m_scrollAnimation->setEndValue(QPoint(0, next ? -one.height() : 0));

    m_scrollAnimation->start();
}

QPixmap CalendarWindow::getCalendarSnapshot() const
{
    return m_calendarView->grab(m_calendarView->rect().adjusted(0, CalendarHeaderHeight, 0, 0));
}

QPixmap CalendarWindow::joint(QPixmap &top, QPixmap &bottom) const
{
    QPixmap target(qMax(top.width(), bottom.width()),
                   top.height() + bottom.height());

    target.fill(Qt::white);
    QPainter painter;
    painter.begin(&target);
    painter.drawPixmap(0, 0, top);
    painter.drawPixmap(0, top.height(), bottom);
    painter.end();

    return target;
}
