#ifndef SPINNER_H
#define SPINNER_H

#include <QWidget>

#include <dimagebutton.h>

DWIDGET_USE_NAMESPACE

class QLabel;

class Spinner : public QWidget
{
    Q_OBJECT
public:
    explicit Spinner(QWidget *parent = 0);

    void setRange(int min, int max);
    int value() const;

signals:
    void valueChanged(int value);

public slots:
    void setValue(int value);

private:
    DImageButton *m_prevButton;
    DImageButton *m_nextButton;
    QLabel *m_label;

    int m_value = 0;
    int m_min = INT_MIN;
    int m_max = INT_MAX;

    void setPrevButtonDisabled(bool disabled) const;
    void setNextButtonDisabled(bool disabled) const;
};

#endif // SPINNER_H
