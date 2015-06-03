#include "nimcompilercleanstep.h"
#include "nimbuildconfiguration.h"
#include "nimcompilercleanstepconfigwidget.h"

#include <QDir>

namespace NimPlugin {

NimCompilerCleanStep::NimCompilerCleanStep(ProjectExplorer::BuildStepList *parentList)
        : ProjectExplorer::BuildStep(parentList, Core::Id("Nim Clean Step")) //ProjectExplorer::AbstractProcessStep(parentList, Core::Id("Nim Clean Step"))
{
    setDefaultDisplayName(QStringLiteral("Nim Clean Step"));
    setDisplayName(QStringLiteral("Nim Clean Step"));
}

ProjectExplorer::BuildStepConfigWidget *NimCompilerCleanStep::createConfigWidget()
{
    using namespace ProjectExplorer;
    return new NimCompilerCleanStepConfigWidget(this);
}

bool NimCompilerCleanStep::init()
{
    Utils::FileName buildDir = buildConfiguration()->buildDirectory();
    bool result = buildDir.exists();
    if (result)
        m_buildDir = buildDir;
    return result;
}

void NimCompilerCleanStep::run(QFutureInterface<bool> &fi)
{
    // If build directory doesn't exists exit
    if (!m_buildDir.exists()) {
        auto toNativePath = QDir::toNativeSeparators(m_buildDir.toString());
        emit addOutput(tr("Build directory \"%1\" doesn't exists").arg(toNativePath), BuildStep::ErrorMessageOutput);
        fi.reportResult(false);
        emit finished();
        return;
    }

    // Remove the build directory
    QString error;
    bool result = Utils::FileUtils::removeRecursively(m_buildDir, &error);

    // Report exit code
    if (result)
        emit addOutput(tr("Clean step completed successfully"), BuildStep::NormalOutput);
    else
        emit addOutput(tr("An error occured during the clean step \"%1\"").arg(error), BuildStep::ErrorMessageOutput);
    fi.reportResult(result);
    emit finished();
}

}

