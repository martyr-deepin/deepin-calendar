#include "calendartitlebarwidget.h"

static const int MinimumYearNumber = 1900;

LeftArrowButton::LeftArrowButton(QWidget *parent)
    : DImageButton(parent) {
    this->setHoverPic(":/resources/icon/previous_hover.png");
    this->setNormalPic(":/resources/icon/previous_normal.png");
    this->setPressPic(":/resources/icon/previous_press.png");

    connect(this, &LeftArrowButton::clicked, this, &LeftArrowButton::setIncrease);
}
void LeftArrowButton::setIncrease() {
    emit leftArrowClicked();
}
LeftArrowButton::~LeftArrowButton(){}

RightArrowButton::RightArrowButton(QWidget *parent)
    : DImageButton(parent) {
    this->setFixedSize(24, 24);
    this->setHoverPic(":/resources/icon/next_hover.png");
    this->setNormalPic(":/resources/icon/next_normal.png");
    this->setPressPic(":/resources/icon/next_press.png");

    connect(this, &RightArrowButton::clicked, this, &RightArrowButton::setIncrease);
}

void RightArrowButton::setIncrease() {
    emit rightArrowclicked();
}
RightArrowButton::~RightArrowButton(){}

CalendarTitleBarWidget::CalendarTitleBarWidget(QWidget *parent)
    : QWidget(parent) {
    setFixedWidth(parent->width() - 120);

    QFrame *iconHolder = new QFrame(this);
    iconHolder->setAttribute(Qt::WA_TranslucentBackground);
    iconHolder->setFixedSize(24, 24);

    m_icon = new QLabel(this);
    m_icon->setFixedSize(24, 24);
    m_icon->setPixmap(QPixmap(":/resources/icon/deepin-calendar_48.png").scaled(m_icon->size()));

    m_calendarIcon = new DImageButton(iconHolder);
    m_calendarIcon->setFixedSize(iconHolder->size());
    m_calendarIcon->setHoverPic(":/resources/icon/today_hover.png");
    m_calendarIcon->setNormalPic(":/resources/icon/today_normal.png");
    m_calendarIcon->setPressPic(":/resources/icon/today_press.png");
    m_calendarIcon->hide();

    m_festivalLabel = new QLabel(this);
    m_festivalLabel->setFixedWidth(190);
    m_yearLabel = new QLabel(this);
    m_monthLabel = new QLabel(this);
    // Give month label a fixed width, so it won't shrink or expand
    // even when the month number changes between single and double.
    m_monthLabel->setFixedWidth(20);
    m_monthLabel->setAlignment(Qt::AlignCenter);

    m_yearLeftBtn = new LeftArrowButton(this);
    m_yearRightBtn = new RightArrowButton(this);

    m_monthLeftBtn = new LeftArrowButton(this);
    m_monthRightBtn = new RightArrowButton(this);

    m_layout = new QHBoxLayout;
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->addSpacing(8);
    m_layout->addWidget(m_icon);
    m_layout->addSpacing(16);
    m_layout->addWidget(iconHolder);
    m_layout->addSpacing(10);
    m_layout->addWidget(m_festivalLabel);
    m_layout->addStretch();
    m_layout->addWidget(m_yearLeftBtn);
    m_layout->addSpacing(5);
    m_layout->addWidget(m_yearLabel);
    m_layout->addSpacing(5);
    m_layout->addWidget(m_yearRightBtn);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_monthLeftBtn);
    m_layout->addSpacing(5);
    m_layout->addWidget(m_monthLabel);
    m_layout->addSpacing(5);
    m_layout->addWidget(m_monthRightBtn);
    m_layout->addSpacing(260);
    setLayout(m_layout);

    setStyleSheet("QLabel{font-family:SourceHanSansCN-Normal;font-size:14px;}");

    connect(m_yearLeftBtn, &LeftArrowButton::leftArrowClicked, this, &CalendarTitleBarWidget::setYearIncrease);
    connect(m_yearRightBtn, &RightArrowButton::rightArrowclicked, this, &CalendarTitleBarWidget::setYearIncrease);
    connect(m_monthLeftBtn, &LeftArrowButton::leftArrowClicked, this, &CalendarTitleBarWidget::setMonthIncrease);
    connect(m_monthRightBtn, &RightArrowButton::rightArrowclicked, this, &CalendarTitleBarWidget::setMonthIncrease);
    connect(m_calendarIcon, &DImageButton::clicked, this, &CalendarTitleBarWidget::todayButtonClicked);
}

void CalendarTitleBarWidget::setFestival(const QString &festival) {
    m_festival = festival;

    const QFontMetrics fm = m_festivalLabel->fontMetrics();
    const QString edlided = fm.elidedText(m_festival, Qt::ElideRight, m_festivalLabel->width());
    m_festivalLabel->setText(edlided);
    m_festivalLabel->setToolTip(festival);
}

void CalendarTitleBarWidget::showCalendarIcon()
{
    m_calendarIcon->show();
}

void CalendarTitleBarWidget::hideCalendarIcon()
{
    m_calendarIcon->hide();
}

void CalendarTitleBarWidget::setCurrentYearMonth(int yearNum, int monthNum) {
    m_year = yearNum;
    m_month = monthNum;
    m_yearLabel->setText(QString::number(m_year));
    m_monthLabel->setText(QString::number(m_month));
}

void CalendarTitleBarWidget::setYearIncrease(bool increase) {
    if (increase) {
        m_year += 1;
    } else if (m_year > MinimumYearNumber){
        m_year -= 1;
    }
    m_yearLabel->setText(QString::number(m_year));

    emit currentYearMonthChanged(m_year, m_month);
}

void CalendarTitleBarWidget::setMonthIncrease(bool increase) {
    if (increase) {
        if (m_month<12) {
            m_month+=1;
        } else {
            m_month=1;
            m_year+=1;
        }
    } else {
        if (m_month==1) {
            m_month=12;
            m_year-=1;
        } else {
            m_month-=1;
        }
    }
    m_yearLabel->setText(QString::number(m_year));
    m_monthLabel->setText(QString::number(m_month));

    emit currentYearMonthChanged(m_year, m_month);
}
CalendarTitleBarWidget::~CalendarTitleBarWidget() {

}
