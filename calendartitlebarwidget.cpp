#include "calendartitlebarwidget.h"

#include <QLabel>
#include <QHBoxLayout>

CalendarTitleBarWidget::CalendarTitleBarWidget(QWidget *parent)
    : QWidget(parent),
      m_icon(new QLabel),
      m_title(new QLabel)
{
    setFixedWidth(parent->width() - 100);

    m_icon->setFixedSize(24, 24);
    m_icon->setPixmap(QPixmap(":/resources/icon/deepin-calendar_48.png").scaled(m_icon->size()));

    m_title->setFixedWidth(200);
    m_title->setAlignment(Qt::AlignCenter);

    QHBoxLayout * layout = new QHBoxLayout;

    layout->addWidget(m_icon);
    layout->addStretch();
    layout->addWidget(m_title);
    layout->addStretch();
    layout->addSpacing(m_icon->width());

    setLayout(layout);
}

void CalendarTitleBarWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}
