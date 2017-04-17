#include "weekindicator.h"

#include <QLabel>
#include <QDebug>
#include <QDate>

WeekIndicator::WeekIndicator(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(30);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setMargin(0);
    setLayout(m_mainLayout);
}

void WeekIndicator::setList(int weekday)
{
    QLayoutItem* child;
    while((child = m_mainLayout->takeAt(0)) != 0) {
        if(child->widget() != 0) {
            delete child->widget();
        }
        delete child;
    }

    QLocale locale;
    for (int i = 0; i != 7; ++i) {

        int d = checkDay(i - weekday);

        QLabel *label = new QLabel(locale.dayName(d ? d : 7, QLocale::ShortFormat));

        if ((i == weekday - 1 && weekday != 0) || i == weekday || (weekday == 0 && i == 6)) {
            label->setObjectName("CalendarHeaderWeekend");
        } else {
            label->setObjectName("CalendarHeaderWeekday");
        }

        label->setAlignment(Qt::AlignCenter);
        label->setFixedSize(DDECalendar::HeaderItemWidth, DDECalendar::HeaderItemHeight);
        m_mainLayout->addWidget(label, 0, Qt::AlignCenter);
    }
}

int WeekIndicator::checkDay(int weekday) {

    // check the week, calculate the correct order in the custom.

    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}
