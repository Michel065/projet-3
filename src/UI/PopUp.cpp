#include "UI/PopUp.h"

void showNumericPopup(QWidget* parent, int valMin, int valMax)
{
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle("Saisir un nombre");
    dialog->setModal(true); // bloque l'accès à la fenêtre principale

    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Label explicatif
    QLabel* label = new QLabel("Entrez une valeur :", dialog);
    layout->addWidget(label);

    // Champ numérique
    QSpinBox* spinBox = new QSpinBox(dialog);
    spinBox->setRange(0, 1000); // bornes min et max
    spinBox->setValue(10);      // valeur par défaut
    layout->addWidget(spinBox);

    // Bouton OK
    QPushButton* okButton = new QPushButton("OK", dialog);
    layout->addWidget(okButton);

    QObject::connect(okButton, &QPushButton::clicked, [dialog, spinBox]() {
        int value = spinBox->value();
        qDebug() << "Valeur entrée :" << value; // tu peux utiliser la valeur comme tu veux
        dialog->accept(); // ferme la pop-up
        });

    dialog->setLayout(layout);
    dialog->exec(); // affiche la pop-up
}
