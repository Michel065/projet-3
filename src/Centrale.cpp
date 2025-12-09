#include "Centrale.hpp"



Centrale::Centrale(int id,
                   Status statusInitial,
                   std::shared_ptr<Reservoir> reservoirAmont,
                    std::shared_ptr<Capteur> capteurQturb,
                    std::unique_ptr<ModuleRepartitionDebit>& moduleRepartition)
    : m_id(id),
      m_status(statusInitial),
      m_reservoirAmont(std::move(reservoirAmont)),
      m_capteurQturb(std::move(capteurQturb)),
      m_moduleRepartition(std::move(moduleRepartition))
{
    m_series = new QPieSeries();
    m_chart = new QChart();
    // Cr�er le chart
    m_chart->addSeries(m_series);
    m_chart->setTitle("Repartition de la puissance produite par turbine");
    QFont titleFont;
    titleFont.setFamily("Arial");     // ou ce que tu veux
    titleFont.setPointSize(14);       // taille du titre
    titleFont.setBold(true);          // en gras

    m_chart->setTitleFont(titleFont);
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);

    date = QDateTime::currentDateTime();

}

int Centrale::getId() const{return m_id;}

Status Centrale::getStatus() const{return m_status;}

void Centrale::setStatus(Status s){m_status = s;}

void Centrale::ajouterTurbine(std::unique_ptr<Turbine> turbine)
{
    if (turbine) {
        m_turbines.push_back(std::move(turbine));
    }
}

void Centrale::ajouterTurbineWidget(Turbine* turbine, QWidget* parent)
{
	std::unique_ptr<TurbineWidget> widget = std::make_unique<TurbineWidget>(turbine, parent);
	m_turbineWidgets.push_back(std::move(widget));
}

int Centrale::getNbTurbines() const {return m_turbines.size();}

float Centrale::calculerHauteurChute() const
{
    if (!m_reservoirAmont) {
        return 0.f;
    }

    float nivAmont = m_reservoirAmont->getNiveau();

    float Qtot = 0.f;
    for (const auto& t : m_turbines) {
        if (t) {
            Qtot += t->getDebit();
        }
    }

    const float p1 = -1.453e-6f;
    const float p2 =  0.007022f;
    const float p3 = 99.98f;

    float elav_calc = p1 * Qtot * Qtot + p2 * Qtot + p3;

    return nivAmont - elav_calc;
}


float Centrale::getProductionInstantanee() const
{
    if (m_status != Status::Marche) {
        return 0.f;
    }
    auto details = getProductionInstantaneeDetail();
    float somme = 0.f;
    for (const auto& [id, prod] : details) {
        somme += prod;
    }

    return somme;
}


std::vector<std::pair<int, float>> Centrale::getProductionInstantaneeDetail() const
{
    std::vector<std::pair<int, float>> details;

    if (m_status != Status::Marche) {
        for (const auto& t : m_turbines) {
            if (!t) continue;
            details.emplace_back(t->getId(), 0.f);
        }
        return details;
    }

    float hauteurChute = calculerHauteurChute();

    for (const auto& t : m_turbines) {
        if (!t) continue;

        t->setHauteurChute(hauteurChute);
        float p = t->getProductionInstantanee();
        details.emplace_back(t->getId(), p);
    }

    return details;
}

void Centrale::print_Production_centrale() const
{
    float H = calculerHauteurChute();
    float productionTotale = getProductionInstantanee();

    std::cout << "===== PRODUCTION CENTRALE " << m_id << " =====\n";
    std::cout << "Hauteur de chute : " << H << "\n";
    std::cout << "Production totale : " << productionTotale << " MW\n";
    std::cout << "===============================\n";
}

void Centrale::print_Production_centrale_detail() const
{
    float H = calculerHauteurChute();
    float productionTotale = getProductionInstantanee();

    std::cout << "===== DETAIL PRODUCTION CENTRALE " << m_id << " =====\n";
    std::cout << "Hauteur de chute calculee : " << H << "\n";
    std::cout << "Production totale : " << productionTotale << " MW\n";
    std::cout << "---------------------------------------------\n";

    for (const auto& t : m_turbines)
    {
        if (!t) continue;
        t->setHauteurChute(H);
        float prod = t->getProductionInstantanee();

        std::cout << " - Turbine " << t->getId()
                  << " | debit = " << t->getDebit()
                  << " | prod = " << prod << " MW\n";
    }

    std::cout << "===============================\n";
}

const Turbine* Centrale::getTurbine(int index) const
{
    if (index < 0 || index >= getNbTurbines())
        return nullptr;

    return m_turbines[index].get();
}

Turbine* Centrale::getTurbine(int index)// uniquement utilise dans le cas ou on veux simuler depuis l'exterieur
{
    if (index < 0 || index >= getNbTurbines())
        return nullptr;

    return m_turbines[index].get();
}

void Centrale::UpdateScreen()
{
    if (cpt == timeStep) {
		m_src.get()->avancer();
        mettreAJour();

        std::vector<float> productionHistory, indices;
        for (int i = 0; i < getTurbine(currentTurbineGraph)->getHistoriqueData().size(); ++i)
        {
            productionHistory.push_back(getTurbine(currentTurbineGraph)->getHistoriqueData()[i].puissance);
            indices.push_back(i);
        }
        graphWidget->setData(productionHistory);
        graphWidget->update();

		needUpdate = true;
        cpt = 0;
    }
    else {
        cpt++;
    }
    if (updateGraph)
    {
        std::vector<float> productionHistory, indices;
        for (int i = 0; i < getTurbine(currentTurbineGraph)->getHistoriqueData().size(); ++i)
        {
            productionHistory.push_back(getTurbine(currentTurbineGraph)->getHistoriqueData()[i].puissance);
            indices.push_back(i);
        }
        graphWidget->setData(productionHistory);
        graphWidget->update();
        updateGraph = false;
    }
}

void Centrale::draw(QPainter& p, int height, int width)
{   
    if(needUpdate)
    {
        drawRepartitionPuissance(p);
        needUpdate = false;
	}

    QString dateStr = QDate::currentDate().toString("dd MMMM yyyy");

    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.setPen(Qt::black);

    QRect dateRect(0, 0, width - 10, 50); // marge de 10 px
    p.drawText(dateRect, Qt::AlignRight | Qt::AlignVCenter, date.toString("dd MMMM yyyy \n hh:mm"));
}

void Centrale::drawRepartitionPuissance(QPainter& p)
{

    m_series->clear();
    // Ajouter les turbines ou cat�gories
    for (const auto& t : m_turbines) {
        m_series->append(QString("Turbine %1").arg(t->getId()), t->getProductionInstantanee());
    }

    // Mettre en style donut
    m_series->setHoleSize(0.35);  // 0 = plein, 1 = invisible

    // Mettre en valeur les slices et labels
    int i = 0;
    QList<QColor> colors = { QColor(155, 89, 182), QColor(241, 148, 138),
                             QColor(247, 202, 24), QColor(52, 152, 219), QColor(189, 195, 199) };

    for (QPieSlice* slice : m_series->slices()) {
        slice->setLabelVisible(true);
        slice->setBrush(colors[i % colors.size()]);

        // label style "Valeur (Pourcentage)"
        slice->setLabel(QString("%1 (%2%)")
            .arg(slice->label())
            .arg(int(slice->percentage() * 100)));

        if (i == 1) {  // par exemple, mettre en avant le deuxi�me slice
            slice->setExploded(true);
            slice->setLabelArmLengthFactor(0.3);
            slice->setLabelVisible(true);
        }
        ++i;
    } 
}

void Centrale::SetDataSource(std::shared_ptr<SourceDonnees> data)
{
    m_src = data;
}

void Centrale::SetParentWidget(QWidget* parent)
{
    m_chartView = new QChartView(m_chart, parent);
    // QChartView
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setGeometry(parent->width() * 0.05, parent->height() * 0.6, 500, 300);
    m_chartView->setAutoFillBackground(false);
    m_chartView->show();


    graphWidget = new GraphWidget(parent);
    graphWidget->setGeometry(parent->width() * 0.6, parent->height() * 0.6, 500, 300);
    graphWidget->show();

    for (int i = 0; i < getNbTurbines(); i++) {
        m_buttons.push_back(new SimpleBouton(
            QString("Turbine %1").arg(getTurbine(i)->getId()), 
            parent,
            [this, i]() {
                currentTurbineGraph = i;
                updateGraph = true;
            }
        ));
		m_buttons.back()->setGeometry(parent->width() * 0.5, parent->height() * 0.65 + i * 50, 100, 30);
		m_buttons.back()->show();
    }
}

void Centrale::SetupPositionTurbineWidgets(int startX, int startY, int widthScreen)
{
	int spacingX = int(widthScreen * 0.9 - (m_turbineWidgets.size() * getNbTurbines()))/ (getNbTurbines());
    for(auto & widget : m_turbineWidgets)
    {
        widget->move(startX, startY);
		widget->show();
        startX += spacingX;

	}
}


//test repartition
std::vector<EtatTurbine> Centrale::construireEtatsTurbines() const
{
    std::vector<EtatTurbine> etats;
    etats.reserve(m_turbines.size());

    for (const auto& t : m_turbines) {
        if (!t) continue;
        EtatTurbine e;
        e.id           = t->getId();
        e.status       = t->getStatus();
        e.debitMin     = t->getDebitMin();
        e.debitMax     = t->getDebitMax();
        e.debitActuel  = t->getDebit();
        etats.push_back(e);
    }

    return etats;
}

void Centrale::setCommandeTurbine(int idTurbine, const CommandeTurbine& cmd)
{
    // Si aucune contrainte n'est active, on supp
    if (!cmd.forceDebit && !cmd.forceStatus) {
        m_commandesTurbines.erase(idTurbine);
        return;
    }

    m_commandesTurbines[idTurbine] = cmd;
}

const CommandeTurbine& Centrale::getCommandeTurbine(int idTurbine) const
{
    static const CommandeTurbine cmdNeutre{};
    auto it = m_commandesTurbines.find(idTurbine);
    if (it != m_commandesTurbines.end())
        return it->second;
    return cmdNeutre;
}

void Centrale::clearCommandeTurbine(int idTurbine)
{
    m_commandesTurbines.erase(idTurbine);
}

void Centrale::clearToutesCommandes()
{
    m_commandesTurbines.clear();
}

ResultatRepartition Centrale::repartirDebit(float debitTotal)
{
    if (!m_moduleRepartition) {
        ResultatRepartition res;
        res.repartitionPossible = false;
        res.debitVanne = debitTotal;
        res.message = "Aucun module de repartition de debit n'est defini.";
        return res;
    }

    auto etats = construireEtatsTurbines();
    auto res = m_moduleRepartition->calculer(etats, debitTotal, m_commandesTurbines);

    std::size_t idx = 0;
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (idx < res.debitsTurbines.size()) {
            t->setDebit(res.debitsTurbines[idx]);
        }
        ++idx;
    }

    return res;
}

void Centrale::setDebitMinTurbine(int idTurbine, float newMin)
{
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (t->getId() == idTurbine) {
            t->setDebitMin(newMin);
            return;
        }
    }
    std::cout << "Centrale " << m_id << " : turbine " << idTurbine << " introuvable (setDebitMin)\n";
}

void Centrale::setDebitMaxTurbine(int idTurbine, float newMax)
{
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (t->getId() == idTurbine) {
            t->setDebitMax(newMax);
            return;
        }
    }
    std::cout << "Centrale " << m_id << " : turbine " << idTurbine << " introuvable (setDebitMax)\n";
}

ResultatRepartition Centrale::mettreAJour() {
    ResultatRepartition res;
    
    date.addSecs(7200);

    float hauteurChute = calculerHauteurChute();

    for (const auto& t : m_turbines) {
        if (!t && t->getMode() == ModeTurbine::Automatique)
            t->mettreAJourDepuisCapteur();
    }

    float debitTotal = 0.f;
    if (m_capteurQturb) {
        debitTotal = m_capteurQturb->lire();
    }
    else{std::cout << "Centrale " << m_id<< " : capteur Qturb non defini\n";}

    //res = repartirDebit(debitTotal);

    std::size_t idx = 0;
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (idx < res.debitsTurbines.size())
            t->setDebit(res.debitsTurbines[idx]);
        if(t->getMode() == ModeTurbine::Automatique) 
            t->setHauteurChute(hauteurChute);

        t->addDataToHistorique({
            t->getId(),
            t->getDebit(),
            t->getDebitMin(),
            t->getDebitMax(),
            t->getHauteurChute(),
            t->getProductionInstantanee()
        });

        ++idx;
    }

    if (!res.repartitionPossible && !res.message.empty()) {
        std::cout << "Centrale " << m_id << " : " << res.message << '\n';
    }

    if (m_reservoirAmont) m_reservoirAmont->mettreAJour();
    return res;
}

