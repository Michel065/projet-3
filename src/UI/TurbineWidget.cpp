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
        if (on) {
            m_turbine->setStatus(Status::Marche);
        }
        });

    // --- Champ numérique ---
    m_editValeur = new QLineEdit(this);
    m_editValeur->setPlaceholderText("Valeur...");
    m_editValeur->setFixedWidth(120);
    m_editValeur->setAlignment(Qt::AlignCenter);

    EditQMin = new QLineEdit(this);
    EditQMin->setPlaceholderText(QString::number(m_turbine->getDebitMin()));
    EditQMin->setFixedWidth(120);
    EditQMin->setAlignment(Qt::AlignCenter);

    EditQMin->setValidator(new QDoubleValidator(
        0,
        999,
        2,
        this
    ));

    EditQMax= new QLineEdit(this);
    EditQMax->setPlaceholderText(QString::number(m_turbine->getDebitMax()));
    EditQMax->setFixedWidth(120);
    EditQMax->setAlignment(Qt::AlignCenter);

    EditQMax->setValidator(new QDoubleValidator(
        0,
        999,
        2,
        this
    ));

    // --- ComboBox Status ---
    m_comboStatus = new QComboBox(this);
	m_comboStatus->addItem("Marche", QVariant::fromValue(static_cast<int>(Status::Marche)));
    m_comboStatus->addItem("Arret", QVariant::fromValue(static_cast<int>(Status::Arret)));
    m_comboStatus->addItem("Maintenance", QVariant::fromValue(static_cast<int>(Status::Maintenance)));
    m_comboStatus->setFixedWidth(120);

    connect(EditQMin, &QLineEdit::editingFinished, this, [this]() {
        bool ok;
        double value = EditQMin->text().toDouble(&ok);
        if (ok) {
            m_turbine->setDebitMin(value);
            // si tu as un mécanisme pour recalculer ou repaint
            update();
        }
        });

    connect(EditQMax, &QLineEdit::editingFinished, this, [this]() {
        bool ok;
        double value = EditQMax->text().toDouble(&ok);
        if (ok) {
            m_turbine->setDebitMax(value);
            update();
        }
        });

    connect(m_editValeur, &QLineEdit::editingFinished, this, [this]() {
        bool ok;
        double value = m_editValeur->text().toDouble(&ok);
        if (ok) {
            m_turbine->setDebit(value); // ou autre fonction selon ce champ
            update();
        }
        });

    connect(m_comboStatus, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &TurbineWidget::onStatusChanged);
}


void TurbineWidget::paintEvent(QPaintEvent* event)
{
    //On active ou désactiver les controles manuels

    m_editValeur->setValidator(new QDoubleValidator(
        m_turbine->getDebitMin(),
        m_turbine->getDebitMax(),
        2,
        this
    ));

    m_comboStatus->setVisible(!m_switch->isOn());
    m_editValeur->setVisible(!m_switch->isOn());
    EditQMax->setVisible(!m_switch->isOn());
    EditQMin->setVisible(!m_switch->isOn());

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int x = 0;
    int y = 0;
    QRect r(x, y, 250, m_turbine->getMode() ? 150 : 320);

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
    int inputY = r.bottom() - 150;

    if (!m_switch->isOn()) {
        p.drawText(r.adjusted(20, inputY, 0, 0), "Debit : ");
        p.drawText(r.adjusted(20, inputY + 35, 0, 0), "Mode : ");
        m_editValeur->move(inputX, inputY);
        m_comboStatus->move(inputX, inputY + 35);


        p.drawText(r.adjusted(20, inputY + 70, 0, 0), "Debit Min: ");
        p.drawText(r.adjusted(20, inputY + 105, 0, 0), "Debit Max: ");
        EditQMin->move(inputX + 20, inputY + 70);
        EditQMax->move(inputX + 20, inputY + 105);
    }
    QWidget::paintEvent(event);
}

void TurbineWidget::onStatusChanged(int index)
{
    // Récupérer la valeur associée à l'élément sélectionné
    Status s = static_cast<Status>(m_comboStatus->itemData(index).toInt());

    // Appliquer le nouveau statut à la turbine
    m_turbine->setStatus(s);
    if (s == Status::Maintenance || s == Status::Arret ) {
        m_turbine->setDebit(0);
    }
    update();
}