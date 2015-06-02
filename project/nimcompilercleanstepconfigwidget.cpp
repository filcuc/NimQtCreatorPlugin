#include "nimcompilercleanstepconfigwidget.h"
#include "ui_nimcompilercleanstepconfigwidget.h"

namespace NimPlugin {

NimCompilerCleanStepConfigWidget::NimCompilerCleanStepConfigWidget(NimCompilerCleanStep *cleanStep)
    : ProjectExplorer::BuildStepConfigWidget()
    , m_cleanStep(cleanStep)
    , m_ui(new Ui::NimCompilerCleanStepConfigWidget())
{
    m_ui->setupUi(this);
}

QString NimCompilerCleanStepConfigWidget::summaryText() const
{
    return QStringLiteral("Nim clean step");
}

QString NimCompilerCleanStepConfigWidget::displayName() const
{
    return QStringLiteral("Nim clean step");
}

}

