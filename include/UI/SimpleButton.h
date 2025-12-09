#ifndef SIMPLEBOUTON_H
#define SIMPLEBOUTON_H

#include <QPushButton>
#include <functional>

class SimpleBouton : public QPushButton
{
    Q_OBJECT

public:
    explicit SimpleBouton(const QString& text = "",
        QWidget* parent = nullptr,
        std::function<void()> callback = nullptr)
        : QPushButton(text, parent),
        m_callback(std::move(callback))
    {
        // Connexion automatique du callback
        if (m_callback)
        {
            connect(this, &QPushButton::clicked,
                this, [this]() { m_callback(); });
        }

        // Style moderne (Material-like)
        setMinimumSize(90, 30);
        setCursor(Qt::PointingHandCursor);

        setStyleSheet(R"(
            QPushButton {
                background-color: #2980b9;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 6px 10px;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton:hover {
                background-color: #3498db;
            }
            QPushButton:pressed {
                background-color: #2471a3;
            }
            QPushButton:disabled {
                background-color: #bdc3c7;
                color: #6e6e6e;
            }
        )");
    }

    // Active/désactive proprement
    void setActif(bool actif)
    {
        setEnabled(actif);
    }

private:
    std::function<void()> m_callback;
};

#endif // SIMPLEBOUTON_H
