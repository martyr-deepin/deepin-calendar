#include "calendartitlebarwidget.h"

LeftArrowButton::LeftArrowButton(QWidget *parent)
    : DImageButton(parent) {
    this->setHoverPic(":/resources/icon/left_button.png");
    this->setNormalPic(":/resources/icon/left_button.png");
    this->setPressPic(":/resources/icon/left_button.png");
}
LeftArrowButton::~LeftArrowButton(){}

RightArrowButton::RightArrowButton(QWidget *parent)
    : DImageButton(parent) {
    this->setFixedSize(24, 24);
    this->setHoverPic(":/resources/icon/right_button.png");
    this->setNormalPic(":/resources/icon/right_button.png");
    this->setPressPic(":/resources/icon/right_button.png");
}
RightArrowButton::~RightArrowButton(){}

CalendarTitleBarWidget::CalendarTitleBarWidget(QWidget *parent)
    : QWidget(parent) {

    DImageButton* m_calendarIcon = new DImageButton(this);
    m_calendarIcon->setFixedSize(24, 24);
    m_calendarIcon->setHoverPic(":/resources/icon/calendar.png");
    m_calendarIcon->setNormalPic(":/resources/icon/calendar.png");
    m_calendarIcon->setPressPic(":/resources/icon/calendar.png");

    m_festivalLabel = new QLabel(this);
    m_festivalLabel->setText(tr("new year"));
    m_yearLabel = new QLabel(this);
    m_yearLabel->setText(QString(tr("%1 year")).arg(m_year));

    m_monthLabel = new QLabel(this);
    m_monthLabel->setText(QString(tr("%1 month").arg(m_month)));
    m_yearLeftBtn = new LeftArrowButton(this);
    m_yearRightBtn = new RightArrowButton(this);

    m_monthLeftBtn = new LeftArrowButton(this);
    m_monthRightBtn = new RightArrowButton(this);

    m_layout = new QHBoxLayout;
    m_layout->setMargin(0);
    m_layout->addSpacing(5);
    m_layout->addWidget(m_calendarIcon);
    m_layout->addSpacing(30);
    m_layout->addWidget(m_festivalLabel);
    m_layout->addSpacing(30);
    m_layout->addWidget(m_yearLeftBtn);
    m_layout->addWidget(m_yearLabel);
    m_layout->addWidget(m_yearRightBtn);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_monthLeftBtn);
    m_layout->addWidget(m_monthLabel);
    m_layout->addWidget(m_monthRightBtn);
    setLayout(m_layout);
}

void CalendarTitleBarWidget::setCurrentYearMonth(int yearNum, int monthNum) {
    m_year = yearNum;
    m_month = monthNum;
    m_yearLabel->setText(QString(tr("%1 year")).arg(m_year));
    m_monthLabel->setText(QString(tr("%1 month").arg(m_month)));
}

void CalendarTitleBarWidget::setYearIncrease(bool increase) {
    if (increase) {
        m_year+=1;
    } else {
        m_year-=1;
    }
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
}
CalendarTitleBarWidget::~CalendarTitleBarWidget() {

}
