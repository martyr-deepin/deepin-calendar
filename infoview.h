#ifndef INFOVIEW_H
#define INFOVIEW_H

#include <QFrame>

#include <dlinkbutton.h>

DWIDGET_USE_NAMESPACE

class QLabel;
class Spinner;

class InfoView : public QFrame
{
    Q_OBJECT
public:
    explicit InfoView(QFrame *parent = 0);

signals:
    void yearChanged(int year);
    void monthChanged(int month);
    void todayButtonClicked();

public slots:
    void setTime(const QString &time) const;
    void setFestival(const QString &festival) const;

    int year() const;
    int month() const;
    void setYear(int year) const;
    void setMonth(int month) const;
    void setYearRange(int min, int max) const;

    void increaseMonth(bool increase);

    void setTodayButtonVisible(bool visible) const;

private:
    QLabel * m_timeLabel;
    QLabel * m_festivalLabel;
    DLinkButton * m_todayButton;
    Spinner * m_yearSpinner;
    Spinner * m_monthSpinner;
};

#endif // INFOVIEW_H
