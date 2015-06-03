#include "nimcompilercleanstepconfigwidget.h"
#include "ui_nimcompilercleanstepconfigwidget.h"
#include "nimcompilercleanstep.h"

#include "projectexplorer/buildconfiguration.h"

namespace NimPlugin {

NimCompilerCleanStepConfigWidget::NimCompilerCleanStepConfigWidget(NimCompilerCleanStep *cleanStep)
    : ProjectExplorer::BuildStepConfigWidget()
    , m_cleanStep(cleanStep)
    , m_ui(new Ui::NimCompilerCleanStepConfigWidget())
{
    m_ui->setupUi(this);
    connect(cleanStep->buildConfiguration(), SIGNAL(buildDirectoryChanged()), this, SLOT(updateUI()));
    updateUI();
}

QString NimCompilerCleanStepConfigWidget::summaryText() const
{
    return QStringLiteral("Nim clean step");
}

QString NimCompilerCleanStepConfigWidget::displayName() const
{
    return QStringLiteral("Nim clean step");
}

void NimCompilerCleanStepConfigWidget::updateUI()
{
    auto buildDiretory = m_cleanStep->buildConfiguration()->buildDirectory();
    m_ui->workingDirectoryLineEdit->setText(buildDiretory.toString());
}

}

