#ifndef CALENDARTITLEBARWIDGET_H
#define CALENDARTITLEBARWIDGET_H

#include <QWidget>

class QLabel;

class CalendarTitleBarWidget: public QWidget {
    Q_OBJECT
public:
    CalendarTitleBarWidget(QWidget* parent=0);

    void setTitle(const QString &title);

private:
    QLabel * m_icon;
    QLabel * m_title;
};

#endif // CALENDARTITLEBARWIDGET_H
