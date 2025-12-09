#pragma once
#include <QWidget>
#include <QVector>

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget* parent = nullptr);

    void setData(std::vector<float> values);
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> m_values;
};
