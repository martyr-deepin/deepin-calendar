/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "calendarview.h"
#include "calendardbus.h"

#include "constants.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QQueue>

QQueue<int> *CalendarView::queue = nullptr;
QMap<QDate, CaLunarDayInfo> *CalendarView::lunarCache = nullptr;
CaLunarDayInfo *CalendarView::emptyCaLunarDayInfo = nullptr;

CalendarView::CalendarView(QWidget *parent) : QWidget(parent)
{
    m_DBusInter = new CalendarDBus("com.deepin.api.LunarCalendar", "/com/deepin/api/LunarCalendar", QDBusConnection::sessionBus(), this);
    if (!queue)
        queue = new QQueue<int>;
    if (!lunarCache)
        lunarCache = new QMap<QDate, CaLunarDayInfo>;
    if (!emptyCaLunarDayInfo)
        emptyCaLunarDayInfo = new CaLunarDayInfo;
    m_dayNumFont.setPixelSize(20);
    m_dayLunarFont.setPixelSize(12);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    QLocale locale;
    for (int i = 0; i != 7; ++i)
    {
        QLabel *label = new QLabel(locale.dayName(i ? i : 7, QLocale::ShortFormat));
        if (i == 0 || i == 6) {
            label->setObjectName("CalendarHeaderWeekend");
        } else {
            label->setObjectName("CalendarHeaderWeekday");
        }
        label->setAlignment(Qt::AlignCenter);
        label->setFixedSize(DDECalendar::HeaderItemWidth, DDECalendar::HeaderItemHeight);
        headerLayout->addWidget(label);
    }
    headerLayout->setMargin(0);
    headerLayout->setSpacing(0);

    //add separator line
    QLabel* separatorLine = new QLabel(this);
    separatorLine->setFixedHeight(1);
    separatorLine->setFixedWidth(660);
    separatorLine->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.1);");

    QHBoxLayout* separatorLineLayout = new QHBoxLayout;
    separatorLineLayout->setMargin(0);
    separatorLineLayout->setSpacing(0);
    separatorLineLayout->addStretch(1);
    separatorLineLayout->addWidget(separatorLine);
    separatorLineLayout->addStretch(1);

    // cells grid
    QGridLayout *gridLayout = new QGridLayout;
    for (int r = 0; r != 6; ++r)
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget;
            cell->setFixedSize(DDECalendar::CellWidth, DDECalendar::CellHeight);
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);
        }
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(separatorLineLayout);
    mainLayout->addLayout(gridLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);

    connect(this, &CalendarView::dateSelected, this, &CalendarView::handleCurrentDateChanged);
}

void CalendarView::handleCurrentDateChanged(const QDate &date, const CaLunarDayInfo &detail) {
    Q_UNUSED(detail);
    setCurrentDate(date);
    emit currentDateChanged(date.year(), date.month());
    this->update();
}

int CalendarView::getDateType(const QDate &date) const
{
    const int currentIndex = getDateIndex(date);
    const CaLunarDayInfo info = getCaLunarDayInfo(currentIndex);

    const int dayOfWeek = date.dayOfWeek();
    bool weekends = dayOfWeek == 6 || dayOfWeek == 7;
    bool isCurrentMonth = m_currentDate.month() == date.month();
    bool isFestival = !info.mSolarFestival.isEmpty() || !info.mLunarFestival.isEmpty();

    int resultFlag = 0;
    if (!isCurrentMonth)
        resultFlag |= SO_NotCurrentMonth;
    if (isFestival)
        resultFlag |= SO_Festival;
    if (weekends)
        resultFlag |= SO_Weekends;

    return resultFlag;
}

void CalendarView::setCurrentDate(const QDate &date)
{
    m_currentDate = date;
    int tmpcurrentIndex = getDateIndex(m_currentDate);
    const CaLunarDayInfo info = getCaLunarDayInfo(tmpcurrentIndex);


    if (!info.mLunarFestival.isEmpty()) {
        emit currentFestivalChanged(info.mLunarFestival);
    } else if (!info.mTerm.isEmpty()) {
        emit currentFestivalChanged(info.mTerm);
    } else if (!info.mSolarFestival.isEmpty()) {
        QStringList tmpFestival = info.mSolarFestival.split(" ");

        if (tmpFestival.length()>=3) {
            emit currentFestivalChanged(QString("%1 %2").arg(tmpFestival[0]).arg(tmpFestival[1]));
        } else {
            emit currentFestivalChanged(info.mSolarFestival);
        }
    } else {
        emit currentFestivalChanged("");
    }

    const QDate firstDay(date.year(), date.month(), 1);

    const int daysOfCal = (firstDay.dayOfWeek() % 7) + date.day() - 1;

    const int y = date.dayOfWeek() % 7;
    const int x = daysOfCal / 7;
    const int currentIndex = x * 7 + y;

    for (int i(0); i != 42; ++i) {
        m_days[i] = date.addDays(i - currentIndex);
    }

    setSelectedCell(currentIndex);
}

void CalendarView::setLunarVisible(bool visible)
{
    int state = int(m_showState);

    if (visible)
        state |= ShowLunar;
    else
        state &= ~ShowLunar;

    m_showState = ShowState(state);
    update();
}

void CalendarView::setLunarFestivalHighlight(bool highlight)
{
    highlight = true;
    int state = int(m_showState);
    //

    if (highlight)
        state |= ShowLunarFestivalHighlight;
    else
        state &= ~ShowLunarFestivalHighlight;

    m_showState = ShowState(state);
    update();
}

void CalendarView::setCellSelectable(bool selectable)
{
    if (selectable == m_cellSelectable)
        return;
    m_cellSelectable = selectable;

    emit cellSelectableChanged(m_cellSelectable);
}

int CalendarView::getDateIndex(const QDate &date) const
{
    for (int i = 0; i != 42; ++i)
        if (m_days[i] == date)
            return i;

    return 0;
}

bool CalendarView::eventFilter(QObject *o, QEvent *e)
{
    QWidget *cell = qobject_cast<QWidget *>(o);

    if (cell && m_cellList.contains(cell)) {
        if (e->type() == QEvent::Paint) {
            paintCell(cell);
        } else if (e->type() == QEvent::MouseButtonRelease) {
            cellClicked(cell);
        }
    }

    return false;
}

const QString CalendarView::getCellDayNum(int pos)
{
    const QString result = QString::number(m_days[pos].day());

    if (m_showState & ShowLunar)
        return result + '\n';
    else
        return result;
}

const QDate CalendarView::getCellDate(int pos)
{
    return m_days[pos];
}

const QString CalendarView::getLunar(int pos)
{
    CaLunarDayInfo info = getCaLunarDayInfo(pos);

    if (info.mLunarDayName == "初一") {
        info.mLunarDayName = info.mLunarMonthName;
    }

    if (info.mTerm.isEmpty())
        return info.mLunarDayName;
    return info.mTerm;
}

const CaLunarDayInfo CalendarView::getCaLunarDayInfo(int pos) const
{
    const QDate date = m_days[pos];

    if (lunarCache->contains(date))
        return lunarCache->value(date);
    if (lunarCache->size() > 300)
        lunarCache->clear();

//    QTimer::singleShot(500, [this, pos] {getDbusData(pos);});
    queue->push_back(pos);
    QTimer::singleShot(300, this, SLOT(getDbusData()));

    return *emptyCaLunarDayInfo;
}

void CalendarView::getDbusData() const
{
    if (queue->isEmpty())
        return;

    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    const int pos = queue->head();
    queue->pop_front();
    const QDate date = m_days[pos];

    bool o1;
    QDBusReply<CaLunarDayInfo> reply = m_DBusInter->GetLunarInfoBySolar(date.year(), date.month(), date.day(), o1);

    lunarCache->insert(date, reply.value());

    m_cellList.at(pos)->update();

    // refersh lunar info
    if (date == m_currentDate)
        emit dateSelected(date, reply.value());
}

void CalendarView::paintCell(QWidget *cell)
{
    const int pos = m_cellList.indexOf(cell);
    const int type = getDateType(m_days[pos]);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate();

    QPainter painter(cell);

    // draw selected cell background circle
    if (isSelectedCell)
    {
        QRect fillRect = cell->rect();

        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(QBrush(m_backgroundCircleColor));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 4, 4);
    }

    painter.setPen(Qt::SolidLine);

    // draw text of day
    if (isSelectedCell) {
        painter.setPen(m_selectedTextColor);
    } else if (isCurrentDay) {
        painter.setPen(m_currentDayTextColor);
    } else {
        const int tType = type & 0xff;
        if (tType & SO_NotCurrentMonth)
            painter.setPen(m_notCurrentTextColor);
        else if (type == SO_Weekends)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultTextColor);
    }

    painter.setFont(m_dayNumFont);
    painter.drawText(cell->rect().adjusted(0, 0, 0, -8), Qt::AlignCenter, getCellDayNum(pos));

    // draw text of day type
    if (m_showState & ShowLunar)
    {
        if (isSelectedCell) {
            painter.setPen(m_selectedLunarColor);
        } else if (isCurrentDay) {
            painter.setPen(m_currentDayLunarColor);
        } else if (m_showState & ShowLunarFestivalHighlight) {
            const int tType = type & 0xff;
            if (tType & SO_NotCurrentMonth)
                painter.setPen(tType & SO_Festival ? m_festivalLunarColor : m_notCurrentLunarColor);
            else if (tType & SO_Festival)
                painter.setPen(m_festivalLunarColor);
            else if (tType & SO_Weekends)
                painter.setPen(m_weekendsLunarColor);
            else
                painter.setPen(m_defaultLunarColor);
        }

        painter.setFont(m_dayLunarFont);
        painter.drawText(cell->rect().adjusted(0, 26, 0, 0), Qt::AlignCenter, getLunar(pos));
    }

    painter.end();
}

void CalendarView::cellClicked(QWidget *cell)
{
    if (!m_cellSelectable)
        return;

    const int pos = m_cellList.indexOf(cell);
    if (pos == -1)
        return;

    setSelectedCell(pos);

    // my gift eggs
    static int gift = 0;
    if (m_days[pos] == QDate(1993, 7, 28))
        if (++gift == 10)
            QMessageBox::about(this, "LinuxDeepin", "by shibowen <sbw@sbw.so> :P");
}

void CalendarView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();

    emit dateSelected(m_days[index], getCaLunarDayInfo(index));
}
