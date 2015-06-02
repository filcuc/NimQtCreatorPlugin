#include "nimcompilerbuildstep.h"
#include "nimcompilerbuildstepconfigwidget.h"
#include "nimbuildconfiguration.h"

#include <projectexplorer/buildconfiguration.h>

#include <QDir>

namespace NimPlugin {

NimCompilerBuildStep::NimCompilerBuildStep(ProjectExplorer::BuildStepList *parentList)
    : ProjectExplorer::AbstractProcessStep(parentList, Core::Id("NimCompilerStep"))
{
    setDefaultDisplayName(QStringLiteral("Nim Compiler"));
    setDisplayName(QStringLiteral("Nim Compiler"));
    connectBuildConfigurationSignals();
    updateProcessParameters();
}

ProjectExplorer::BuildStepConfigWidget *NimCompilerBuildStep::createConfigWidget()
{
    return new NimCompilerBuildStepConfigWidget(this);
}

void NimCompilerBuildStep::connectBuildConfigurationSignals()
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(buildConfiguration());
    connect(bc, SIGNAL(buildDirectoryChanged()), this, SLOT(updateProcessParameters()));
    connect(bc, SIGNAL(targetNimFileChanged(Utils::FileName)), this, SLOT(updateProcessParameters()));
    connect(bc, SIGNAL(userCompilerOptionsChanged(QStringList)), this, SLOT(updateProcessParameters()));
}

void NimCompilerBuildStep::updateProcessParameters()
{
    updateCommand();
    updateArguments();
    updateWorkingDirectory();
    updateEnvironment();
}

void NimCompilerBuildStep::updateCommand()
{
    processParameters()->setCommand(QStringLiteral("nim"));
}

void NimCompilerBuildStep::updateWorkingDirectory()
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(buildConfiguration());
    processParameters()->setWorkingDirectory(bc->buildDirectory().toString());
}

void NimCompilerBuildStep::updateArguments()
{    
    auto bc = dynamic_cast<NimBuildConfiguration*>(buildConfiguration());

    QStringList arguments;
    arguments << QStringLiteral("c");
    arguments << bc->compilerOptions();
    arguments << bc->userCompilerOptions();
    arguments << bc->targetNimFile().toString();

    // Remove empty args
    auto predicate = [](const QString& str)->bool{return str.isEmpty();};
    auto it = std::remove_if(arguments.begin(), arguments.end(), predicate);
    arguments.erase(it, arguments.end());

    processParameters()->setArguments(arguments.join(QChar::Space));
}

void NimCompilerBuildStep::updateEnvironment()
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(buildConfiguration());
    processParameters()->setEnvironment(bc->environment());
}

}

