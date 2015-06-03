#pragma once

#include <projectexplorer/buildstep.h>

namespace Ui {
    class NimCompilerCleanStepConfigWidget;
}

namespace NimPlugin {

class NimCompilerCleanStep;

class NimCompilerCleanStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    NimCompilerCleanStepConfigWidget(NimCompilerCleanStep* cleanStep);
    QString summaryText() const Q_DECL_OVERRIDE;
    QString displayName() const Q_DECL_OVERRIDE;

private Q_SLOT:
    void updateUI();

private:
    NimCompilerCleanStep* m_cleanStep;
    Ui::NimCompilerCleanStepConfigWidget* m_ui;
};

}
