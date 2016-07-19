#ifndef CALENDARTITLEBARWIDGET_H
#define CALENDARTITLEBARWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QtGui>
#include <QHBoxLayout>

#include <dimagebutton.h>

DWIDGET_USE_NAMESPACE

class LeftArrowButton:public DImageButton {
Q_OBJECT
public:
    LeftArrowButton(QWidget* parent=0);
    ~LeftArrowButton();
signals:
    void leftArrowClicked(bool clicked=false);
public slots:
    void setIncrease();
};

class RightArrowButton:public DImageButton {
    Q_OBJECT
public:
    RightArrowButton(QWidget* parent=0);
    ~RightArrowButton();
signals:
    void rightArrowclicked(bool clicked=true);
public slots:
    void setIncrease();
};

class CalendarTitleBarWidget:public QWidget {
    Q_OBJECT
public:
    CalendarTitleBarWidget(QWidget* parent=0);
    ~CalendarTitleBarWidget();
signals:
    void todayButtonClicked();
    void currentYearMonthChanged(int year, int month);

public slots:
    void setCurrentYearMonth(int yearNum, int monthNum);
    //if increase=true, m_year+1; else m_year-1;
    void setYearIncrease(bool increase);
    void setMonthIncrease(bool increase);
    void setFestival(const QString &festival);
    void showCalendarIcon();
    void hideCalendarIcon();

private:
    QLabel* m_festivalLabel;
    QLabel* m_yearLabel;
    QLabel* m_monthLabel;
    DImageButton* m_calendarIcon;

    int m_year=2000;
    int m_month=10;
    QString m_festival="";
    LeftArrowButton* m_yearLeftBtn;
    RightArrowButton* m_yearRightBtn;
    LeftArrowButton* m_monthLeftBtn;
    RightArrowButton* m_monthRightBtn;
    QHBoxLayout* m_layout;
};

#endif // CALENDARTITLEBARWIDGET_H
