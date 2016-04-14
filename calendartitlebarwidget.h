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
};

class RightArrowButton:public DImageButton {
    Q_OBJECT
public:
    RightArrowButton(QWidget* parent=0);
    ~RightArrowButton();
};

class CalendarTitleBarWidget:public QWidget {
    Q_OBJECT
public:
    CalendarTitleBarWidget(QWidget* parent=0);
    ~CalendarTitleBarWidget();
public slots:
    void setCurrentYearMonth(int yearNum, int monthNum);
    //if increase=true, m_year+1; else m_year-1;
    void setYearIncrease(bool increase);
    void setMonthIncrease(bool increase);
private:
    QLabel* m_festivalLabel;
    QLabel* m_yearLabel;
    QLabel* m_monthLabel;
    int m_year=2000;
    int m_month=10;
    DImageButton* m_yearLeftBtn;
    DImageButton* m_yearRightBtn;
    DImageButton* m_monthLeftBtn;
    DImageButton* m_monthRightBtn;
    QHBoxLayout* m_layout;
};

#endif // CALENDARTITLEBARWIDGET_H
