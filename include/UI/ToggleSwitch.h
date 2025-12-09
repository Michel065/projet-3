#pragma once
#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>

class ToggleSwitch : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(int handleX READ handleX WRITE setHandleX)

public:
    explicit ToggleSwitch(QWidget* parent = nullptr);

    bool isOn() const { return m_isOn; }
    void setOn(bool on, bool animated = true);

    int handleX() const { return m_handleX; }
    void setHandleX(int x);

signals:
    void toggled(bool on);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    bool m_isOn = false;
	bool* valueLink = nullptr;
    int m_handleX = 0;
    QPropertyAnimation* m_anim;
};
