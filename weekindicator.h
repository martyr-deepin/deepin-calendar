#ifndef WEEKINDICATOR_H
#define WEEKINDICATOR_H

#include "constants.h"
#include <QWidget>
#include <QHBoxLayout>

class WeekIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit WeekIndicator(QWidget *parent = 0);
    void setList(int weekday);

private:
    int checkDay(int weekday);

private:
    QHBoxLayout *m_mainLayout;
};

#endif // WEEKINDICATOR_H
