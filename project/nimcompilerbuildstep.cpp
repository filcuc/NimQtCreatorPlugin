#include "nimcompilerbuildstep.h"
#include "nimcompilerbuildstepconfigwidget.h"

#include <projectexplorer/buildconfiguration.h>

#include <QDir>

namespace NimPlugin {

NimCompilerBuildStep::NimCompilerBuildStep(ProjectExplorer::BuildStepList *parentList)
    : ProjectExplorer::AbstractProcessStep(parentList, Core::Id("NimCompilerStep"))
{
    setDefaultDisplayName(QLatin1String("Nim Compiler"));
    setDisplayName(QLatin1String("Nim Compiler"));
    updateProcessParameters();
}

ProjectExplorer::BuildStepConfigWidget *NimCompilerBuildStep::createConfigWidget()
{
    return new NimCompilerBuildStepConfigWidget(this);
}

Utils::FileName NimCompilerBuildStep::target() const
{
    return m_target;
}

void NimCompilerBuildStep::setTarget(const Utils::FileName &target)
{
    if (target == m_target)
        return;
    m_target = target;
    updateProcessParameters();
    emit targetChanged(target);
}

QString NimCompilerBuildStep::additionalArguments() const
{
    return m_additionalArguments;
}

void NimCompilerBuildStep::setAdditionalArguments(const QString &additionalArguments)
{
    if (additionalArguments == m_additionalArguments)
        return;
    m_additionalArguments = additionalArguments;
    updateProcessParameters();
    emit additionalArgumentsChanged(additionalArguments);
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
    processParameters()->setCommand(QLatin1String("nim"));
}

void NimCompilerBuildStep::updateWorkingDirectory()
{
    processParameters()->setWorkingDirectory(buildConfiguration()->buildDirectory().toString());
}

void NimCompilerBuildStep::updateArguments()
{    
    using namespace ProjectExplorer;

    QStringList arguments;
    arguments << QLatin1String("c");

    switch (buildConfiguration()->buildType())
    {
    case BuildConfiguration::Release:
        arguments << QLatin1String("-d:release");
        break;
    case BuildConfiguration::Debug:
        arguments << QLatin1String("--debugInfo");
        arguments << QLatin1String("--lineDir:on");
        break;
    default:
        break;
    }

    QString buildDir = buildConfiguration()->buildDirectory().toString();

    QString cacheDir = QLatin1String("--nimCache:") + buildDir;
    arguments << cacheDir;

    QString outputFile = QLatin1String("--out:") + buildDir
                       + QDir::separator()
                       + QFileInfo(m_target.toString()).baseName();
    arguments << outputFile;

    arguments << m_additionalArguments;
    arguments << m_target.toString();

    // Remove empty args
    auto predicate = [](const QString& str)->bool{return str.isEmpty();};
    auto it = std::remove_if(arguments.begin(), arguments.end(), predicate);
    arguments.erase(it, arguments.end());

    processParameters()->setArguments(arguments.join(QChar::Space));
}

void NimCompilerBuildStep::updateEnvironment()
{
    processParameters()->setEnvironment(buildConfiguration()->environment());
}

}

