#include "UI/TurbineWidget.h"
#include <QLabel>

TurbineWidget::TurbineWidget(Turbine* turbine, QWidget* parent)
    : QWidget(parent)
    , m_turbine(turbine)
{
    setFixedSize(260, 400);

    // --- Toggle switch ---
    m_switch = new ToggleSwitch(this);
    m_switch->move(width() - 80, 120);
    m_switch->setOn(true, false);

    connect(m_switch, &ToggleSwitch::toggled, this, [this](bool on) {
        m_turbine->setMode(on ? ModeTurbine::Automatique
            : ModeTurbine::Manuel);
        });

    // --- Champ numérique ---
    m_editValeur = new QLineEdit(this);
    m_editValeur->setPlaceholderText("Valeur...");
    m_editValeur->setFixedWidth(120);
    m_editValeur->setAlignment(Qt::AlignCenter);

    m_editValeur->setValidator(new QDoubleValidator(
        m_turbine->getDebitMin(),
        m_turbine->getDebitMax(),
        2,
        this
    ));

    // --- ComboBox Status ---
    m_comboStatus = new QComboBox(this);
	m_comboStatus->addItem("Marche", QVariant::fromValue(static_cast<int>(Status::Marche)));
    m_comboStatus->addItem("Arret", QVariant::fromValue(static_cast<int>(Status::Arret)));
    m_comboStatus->addItem("Maintenance", QVariant::fromValue(static_cast<int>(Status::Maintenance)));
    m_comboStatus->setFixedWidth(120);
}


void TurbineWidget::paintEvent(QPaintEvent* event)
{
    //On active ou désactiver les controles manuels
    m_comboStatus->setEnabled(!m_switch->isOn());
    m_editValeur->setEnabled(!m_switch->isOn());
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int x = 0;
    int y = 0;
    QRect r(x, y, 250, 250);

    // --- Shadow ---
    p.setBrush(QColor(0, 0, 0, 60));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r.adjusted(4, 4, 4, 4), 15, 15);

    // --- Card ---
    p.setBrush(QColor(255, 255, 255));
    p.setPen(QPen(QColor(200, 200, 220), 2));
    p.drawRoundedRect(r, 15, 15);

    // --- Text: Title ---
    p.setPen(QColor(10, 20, 50));
    p.setFont(QFont("Arial", 16, QFont::Bold));
    p.drawText(r.adjusted(20, 15, 0, 0),
        QString("Turbine : %1").arg(m_turbine->getId()));

    // --- Text: Puissance ---
    p.setFont(QFont("Arial", 12));
    p.drawText(r.adjusted(20, 50, 0, 0),
        QString("Puissance : %1 MW")
        .arg(m_turbine->getProductionInstantanee(), 0, 'f', 2));

    // --- Text: Débit ---
    if (m_turbine->getDebit() >= 0)
        p.drawText(r.adjusted(20, 80, 0, 0),
            QString("Debit : %1 m3/s")
            .arg(m_turbine->getDebit(), 0, 'f', 2));

    // --- Text: Mode ---
    if (m_turbine->getMode() == ModeTurbine::Manuel)
        p.drawText(r.adjusted(20, 120, 0, 0), "Mode : Manuel");
    else
        p.drawText(r.adjusted(20, 120, 0, 0), "Mode : Auto");

    // ================================
    //   Positionnement des widgets
    // ================================
    int inputX = r.left() + 80;
    int inputY = r.bottom() - 80;

    p.drawText(r.adjusted(20, inputY, 0, 0), "Debit : ");
    p.drawText(r.adjusted(20, inputY + 35, 0, 0), "Mode : ");
    m_editValeur->move(inputX, inputY);
    m_comboStatus->move(inputX, inputY + 35);

    QWidget::paintEvent(event);
}
