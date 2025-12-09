#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include "UI/ToggleSwitch.h"
#include "Turbine.hpp"
#include <QLineEdit>
#include <QComboBox>


class TurbineWidget : public QWidget
{
    Q_OBJECT

public:
    TurbineWidget(Turbine* turbine, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Turbine* m_turbine;
    ToggleSwitch* m_switch;
    QLineEdit* m_editValeur, *EditQMin, *EditQMax;
    QComboBox* m_comboStatus;
};
