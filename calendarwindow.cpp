#include "calendarwindow.h"
#include "dbuscalendar_adaptor.h"
#include "constants.h"
#include "infoview.h"

#include <QDate>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QPainter>

#include <DMenu>
#include <DAboutDialog>

static const int CalendarHeaderHeight = 60;

static const int CalendarWidth = 760;
static const int CalendarHeight = 500;

static const int InfoViewWidth = CalendarWidth - 86;
static const int InfoViewHeight = 90;

static const int ContentLeftRightPadding = 80;

static const int MinYearValue = 1900;

CalendarWindow::CalendarWindow() :
    DWindow(nullptr)
{
    setWindowTitle(tr("Calendar"));
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setDecorationFlags(DWindow::ShowTitlebarSeparator);
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

void CalendarWindow::handleCurrentYearMonthChanged(int year, int month)
{
    QDate changedDate;
    changedDate.setDate(year, month,  1);

    const uint daysInMonth = changedDate.daysInMonth();
    const uint currentDay = QDate::currentDate().day();
    if (currentDay > daysInMonth) {
        changedDate = changedDate.addDays(daysInMonth - 1);
    } else {
        changedDate = changedDate.addDays(currentDay - 1);
    }

    m_calendarView->blockSignals(true);
    m_calendarView->setCurrentDate(changedDate);
    m_calendarView->blockSignals(false);
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
        if (m_infoView->year() > MinYearValue || m_infoView->month() != 1) {
            previousMonth();
        }
    }
}

void CalendarWindow::initUI()
{
    m_contentBackground = new QFrame;
    m_contentBackground->setObjectName("CalendarBackground");
    m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
                             "background:white;"
                             "}");
    m_contentBackground->setFixedSize(CalendarWidth + ContentLeftRightPadding * 2,
                                      InfoViewHeight + CalendarHeight);

    setFixedSize(m_contentBackground->width() + 4,
                 m_contentBackground->height() + titlebarHeight() + 6);
    setContentWidget(m_contentBackground);

    m_calendarTitleBarWidget = new CalendarTitleBarWidget(this);
    setTitlebarWidget(m_calendarTitleBarWidget);

    m_infoView = new InfoView;
    m_infoView->setStyleSheet("QFrame { background: rgba(0, 0, 0, 0) }");
    m_infoView->setFixedSize(InfoViewWidth, InfoViewHeight);
    m_infoView->setYearRange(MinYearValue, INT_MAX);

    m_infoView->setYear(QDate::currentDate().year());
    m_infoView->setMonth(QDate::currentDate().month());

    m_calendarView = new CalendarView;
    m_calendarView->setFixedSize(CalendarWidth, CalendarHeight);
    m_calendarView->setCurrentDate(QDate::currentDate());
    m_calendarView->setLunarVisible(QLocale::system().name().contains("zh"));

    m_animationContainer = new QFrame(m_contentBackground);
    m_animationContainer->setStyleSheet("QFrame { background: rgba(0, 0, 0, 0) }");
    m_animationContainer->setFixedSize(m_calendarView->width(),
                                       m_calendarView->height() - CalendarHeaderHeight);
    m_animationContainer->move(ContentLeftRightPadding, CalendarHeaderHeight + InfoViewHeight);
    m_animationContainer->hide();

    m_fakeContent = new QLabel(m_animationContainer);
    m_fakeContent->setStyleSheet("QLabel { background: rgba(0, 0, 0, 0) }");
    m_fakeContent->setFixedSize(m_animationContainer->width(),
                                m_animationContainer->height() * 2);

    QHBoxLayout * mainLayout = qobject_cast<QHBoxLayout*>(layout());
    QVBoxLayout * contentLayout = new QVBoxLayout(m_contentBackground);
    contentLayout->setMargin(0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(m_infoView, 0, Qt::AlignHCenter);
    contentLayout->addWidget(m_calendarView, 0, Qt::AlignHCenter);
    mainLayout->addLayout(contentLayout);

    connect(m_calendarView, &CalendarView::currentDateChanged, [this](int year, int month){
        qDebug() << "current date changed" << year << month;
        m_infoView->blockSignals(true);
        m_infoView->setYear(year);
        m_infoView->setMonth(month);
        m_infoView->blockSignals(false);
    });
    connect(m_calendarView, &CalendarView::currentFestivalChanged, m_infoView, &InfoView::setFestival);
    connect(m_infoView, &InfoView::todayButtonClicked,
            this, &CalendarWindow::handleTodayButtonClicked);

    connect(m_infoView, &InfoView::yearChanged, [this](int year) {
        const int month = m_infoView->month();
        handleCurrentYearMonthChanged(year, month);
    });
    connect(m_infoView, &InfoView::monthChanged, [this](int month) {
        const int year = m_infoView->year();
        handleCurrentYearMonthChanged(year, month);
    });

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
    updateDate();
    updateTime();

    QTimer * timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, [this] {
        QDate currentDate = QDate::currentDate();
        if (LastCurrentDate != currentDate) {
            LastCurrentDate = currentDate;
            updateDate();
        }
        updateTime();
    });
    timer->start();
}

void CalendarWindow::setupMenu()
{
    DMenu * menu = dbusMenu();
    DAction * aboutAction = menu->addAction(tr("About"));
    DAction * quitAction = menu->addAction(tr("Exit"));

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
    m_animationContainer->raise();

    QPixmap one = getCalendarSnapshot();
    m_infoView->increaseMonth(next);
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

void CalendarWindow::updateTime() const
{
    m_infoView->setTime(QDateTime::currentDateTime().toString("hh:mm"));
}

void CalendarWindow::updateDate() const
{
    QDate currentDate = QDate::currentDate();
    m_calendarView->setCurrentDate(currentDate);
    m_calendarTitleBarWidget->setTitle(currentDate.toString(Qt::SystemLocaleLongDate));
}
