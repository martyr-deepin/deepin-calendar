#include "spinner.h"

#include <QLabel>
#include <QHBoxLayout>

Spinner::Spinner(QWidget *parent) :
    QWidget(parent),
    m_prevButton(new DImageButton),
    m_nextButton(new DImageButton),
    m_label(new QLabel)
{
    setFixedHeight(23);

    setPrevButtonDisabled(false);
    setNextButtonDisabled(false);

    m_label->setFixedWidth(40);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setText(QString::number(m_value));

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_prevButton);
    layout->addWidget(m_label);
    layout->addWidget(m_nextButton);
    setLayout(layout);

    connect(m_prevButton, &DImageButton::clicked, [this]{
        setValue(m_value - 1);
    });
    connect(m_nextButton, &DImageButton::clicked, [this]{
        setValue(m_value + 1);
    });
}

void Spinner::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

int Spinner::value() const
{
    return m_value;
}

void Spinner::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        m_label->setText(QString::number(m_value));

        setPrevButtonDisabled(value <= m_min);
        setNextButtonDisabled(value >= m_max);

        emit valueChanged(value);
    }
}

void Spinner::setPrevButtonDisabled(bool disabled) const
{
    m_prevButton->setDisabled(disabled);
    if (disabled) {
        m_prevButton->setHoverPic(":/resources/icon/previous_disabled.png");
        m_prevButton->setNormalPic(":/resources/icon/previous_disabled.png");
        m_prevButton->setPressPic(":/resources/icon/previous_disabled.png");
    } else {
        m_prevButton->setHoverPic(":/resources/icon/previous_hover.png");
        m_prevButton->setNormalPic(":/resources/icon/previous_normal.png");
        m_prevButton->setPressPic(":/resources/icon/previous_press.png");
    }
}

void Spinner::setNextButtonDisabled(bool disabled) const
{
    m_nextButton->setDisabled(disabled);
    if (disabled) {
        m_nextButton->setHoverPic(":/resources/icon/next_disabled.png");
        m_nextButton->setNormalPic(":/resources/icon/next_disabled.png");
        m_nextButton->setPressPic(":/resources/icon/next_disabled.png");
    } else {
        m_nextButton->setHoverPic(":/resources/icon/next_hover.png");
        m_nextButton->setNormalPic(":/resources/icon/next_normal.png");
        m_nextButton->setPressPic(":/resources/icon/next_press.png");
    }
}
