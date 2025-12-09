#include "UI\ToggleSwitch.h"
#include <QPainter>

ToggleSwitch::ToggleSwitch(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(50, 24);
    m_anim = new QPropertyAnimation(this, "handleX", this);
    m_anim->setDuration(150);
    m_handleX = 2;

}

void ToggleSwitch::mousePressEvent(QMouseEvent*)
{
    setOn(!m_isOn, true);
    emit toggled(m_isOn);
}

void ToggleSwitch::setOn(bool on, bool animated)
{
    m_isOn = on;

    int target = on ? width() - 22 : 2;

    if (animated)
    {
        m_anim->stop();
        m_anim->setStartValue(m_handleX);
        m_anim->setEndValue(target);
        m_anim->start();
    }
    else
    {
        setHandleX(target);
    }

    update();
}

void ToggleSwitch::setHandleX(int x)
{
    m_handleX = x;
    update();
}

void ToggleSwitch::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Fond
    QColor bg = m_isOn ? QColor("#4cd964") : QColor("#bfbfbf");
    p.setBrush(bg);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(rect(), height() / 2, height() / 2);

    // Handle
    p.setBrush(Qt::white);
    p.drawEllipse(QRect(m_handleX, 2, 20, 20));
}
