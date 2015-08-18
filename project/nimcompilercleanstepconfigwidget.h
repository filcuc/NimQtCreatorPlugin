#pragma once

#include <projectexplorer/buildstep.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class NimCompilerCleanStepConfigWidget;
}
QT_END_NAMESPACE

namespace NimPlugin {

class NimCompilerCleanStep;

class NimCompilerCleanStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    /// Constructor
    NimCompilerCleanStepConfigWidget(NimCompilerCleanStep* cleanStep);

    /// Destructor
    virtual ~NimCompilerCleanStepConfigWidget();

    /// Return the summary text
    QString summaryText() const Q_DECL_OVERRIDE;

    /// Return the display name
    QString displayName() const Q_DECL_OVERRIDE;

private Q_SLOT:
    /// Updates the UI
    void updateUI();

private:
    NimCompilerCleanStep* m_cleanStep;
    Ui::NimCompilerCleanStepConfigWidget* m_ui;
};

}
