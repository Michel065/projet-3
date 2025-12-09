#include "UI\GraphUI.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QFont>
#include <algorithm>
#include <cmath>

GraphWidget::GraphWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(200, 150);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(false);
}

void GraphWidget::setData(std::vector<float> values)
{
    m_values.resize(values.size());
    for (size_t i = 0; i < values.size(); ++i)
        m_values[i] = static_cast<double>(values[i]);

    update();
}

void GraphWidget::clear()
{
    m_values.clear();
    update();
}

void GraphWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // --- Fond transparent ---
    setAttribute(Qt::WA_TranslucentBackground, true);
    p.fillRect(rect(), Qt::transparent);

    // --- Container arrondi avec ombre ---
    const int containerRadius = 8;
    QRect container = rect().adjusted(0, 0, 0, 0);

    // Ombre
    QColor shadowColor(0, 0, 0, 30);
    p.setBrush(shadowColor);
    p.setPen(Qt::NoPen);
    QRect shadowRect = container.translated(2, 2);
    p.drawRoundedRect(shadowRect, containerRadius, containerRadius);

    // Fond container
    QLinearGradient contGrad(container.topLeft(), container.bottomLeft());
    contGrad.setColorAt(0.0, QColor(255, 255, 255, 240));
    contGrad.setColorAt(1.0, QColor(250, 253, 255, 240));
    p.setBrush(contGrad);
    p.drawRoundedRect(container, containerRadius, containerRadius);

    if (m_values.isEmpty())
        return;

    // --- Bornes fixes ---
    const double minVal = 0.0;
    const double maxVal = 100.0;

    // --- Marges internes pour zone de graphique ---
    const int marginLeft = 48;
    const int marginBottom = 36;
    const int marginTop = 18;
    const int marginRight = 16;
    QRect area = rect().adjusted(marginLeft, marginTop, -marginRight, -marginBottom);

    // --- Grille horizontale subtile ---
    const int gridLines = 4;
    p.setPen(QPen(QColor(220, 225, 235), 1, Qt::DashLine));
    for (int i = 0; i <= gridLines; ++i) {
        int y = area.top() + i * area.height() / gridLines;
        p.drawLine(area.left(), y, area.right(), y);
    }

    // --- Axes ---
    p.setPen(QPen(QColor(60, 60, 60), 1.5));
    p.drawLine(area.bottomLeft(), area.bottomRight()); // X
    p.drawLine(area.bottomLeft(), area.topLeft());     // Y

    // --- Graduations Y (ticks + labels) ---
    p.setFont(QFont("Arial", 8));
    QFontMetrics fm(p.font());
    for (int i = 0; i <= gridLines; ++i) {
        int y = area.top() + i * area.height() / gridLines;
        double val = maxVal - i * (maxVal - minVal) / gridLines;
        QString text = QString::number(int(val));

        // Label à gauche
        int textWidth = fm.horizontalAdvance(text);
        p.setPen(QColor(80, 85, 90));
        p.drawText(area.left() - 8 - textWidth, y + fm.height() / 4, text);

        // Tick
        p.setPen(QPen(QColor(60, 60, 60), 1.5));
        p.drawLine(area.left() - 4, y, area.left(), y);
    }

    // --- Préparer points normalisés ---
    int n = m_values.size();
    QVector<QPointF> pts;
    pts.reserve(n);

    if (n == 1) {
        double v = std::clamp(m_values[0], minVal, maxVal);
        double norm = (v - minVal) / (maxVal - minVal);
        double x = area.left() + area.width() / 2.0;
        double y = area.bottom() - norm * area.height();
        pts.append(QPointF(x, y));
    }
    else {
        double stepX = double(area.width()) / (n - 1);
        for (int i = 0; i < n; ++i) {
            double v = std::clamp(m_values[i], minVal, maxVal);
            double norm = (v - minVal) / (maxVal - minVal);
            double x = area.left() + i * stepX;
            double y = area.bottom() - norm * area.height();
            pts.append(QPointF(x, y));
        }
    }

    // --- Graduations X (ticks + labels) dynamiques ---
    int maxLabels = 10;
    int stepLabel = std::max(1, n / maxLabels);
    double stepX = (n > 1) ? double(area.width()) / (n - 1) : 0;

    for (int i = 0; i < n; i += stepLabel) {
        int x = area.left() + i * stepX;

        // Tick
        p.setPen(QPen(QColor(60, 60, 60), 1.5));
        p.drawLine(x, area.bottom(), x, area.bottom() + 4);

        // Label
        QString text = QString::number(i);
        int textWidth = fm.horizontalAdvance(text);
        p.setPen(QColor(80, 85, 90));
        p.drawText(x - textWidth / 2, area.bottom() + fm.height() + 2, text);
    }

    // --- Tracer la courbe ---
    if (!pts.isEmpty()) {
        QPainterPath path;
        path.moveTo(pts[0]);
        for (int i = 1; i < pts.size(); ++i)
            path.lineTo(pts[i]);

        // Remplissage sous la courbe
        if (pts.size() >= 2) {
            QPainterPath fillPath = path;
            fillPath.lineTo(area.bottomRight());
            fillPath.lineTo(area.bottomLeft());
            fillPath.closeSubpath();

            QLinearGradient fillGrad(area.topLeft(), area.bottomLeft());
            fillGrad.setColorAt(0.0, QColor(70, 130, 220, 100));
            fillGrad.setColorAt(1.0, QColor(70, 130, 220, 30));

            p.setBrush(fillGrad);
            p.setPen(Qt::NoPen);
            p.drawPath(fillPath);
        }

        // Ombre de la courbe
        QPen shadowPen(QColor(20, 30, 50, 60), 6, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        p.setPen(shadowPen);
        p.setBrush(Qt::NoBrush);
        p.drawPath(path);

        // Courbe principale
        QPen curvePen(QColor(30, 110, 220), 2.5, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        p.setPen(curvePen);
        p.drawPath(path);

        double limitVal = 60.0;
        double normLimit = (limitVal - minVal) / (maxVal - minVal);
        int yLimit = area.bottom() - normLimit * area.height();

        p.setPen(QPen(QColor(220, 50, 50), 2, Qt::DashLine));
        p.drawLine(area.left(), yLimit, area.right(), yLimit);

        // Label "Limite maximale"
        QString limitText = "Limite maximale";
        int textWidth = fm.horizontalAdvance(limitText);
        p.setPen(QColor(220, 50, 50));
        p.drawText((area.left() + area.right())/2, yLimit + fm.height() + 2, limitText);
    }
}

