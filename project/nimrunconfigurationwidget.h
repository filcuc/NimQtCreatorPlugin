#pragma once

#include <QWidget>

namespace Ui {
class NimRunConfigurationWidget;
}

namespace NimPlugin {

class NimRunConfiguration;

/// The widget for editing the Nim run configuration
class NimRunConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit NimRunConfigurationWidget(NimRunConfiguration *rc,
                                       QWidget *parent = 0);

    /// Destructor
    virtual ~NimRunConfigurationWidget();

private Q_SLOTS:
    /// Update the UI
    void updateUI(NimRunConfiguration *rc);

private:
    /// Connect the run configuration signals
    void connectRunConfigurationSignals(NimRunConfiguration *rc);

    QScopedPointer<Ui::NimRunConfigurationWidget> m_ui;
};

}
