#include "nimrunconfiguration.h"
#include "nimrunconfigurationwidget.h"
#include "nimbuildconfiguration.h"
// Qt
#include <QFileInfo>
#include <QDir>
#include <QVariantMap>
// QtCreator
#include <projectexplorer/localenvironmentaspect.h>

namespace NimPlugin {

NimRunConfiguration::NimRunConfiguration(ProjectExplorer::Target* parent)
    : ProjectExplorer::LocalApplicationRunConfiguration(parent, Core::Id("Nim Run Configuration"))
    , m_runMode(ProjectExplorer::ApplicationLauncher::Gui)
{
    addExtraAspect(new ProjectExplorer::LocalEnvironmentAspect(this));
    setDisplayName(QStringLiteral("Nim Run Configuration"));
    setDefaultDisplayName(QStringLiteral("Nim Run Configuration"));
    connectTargetSignals();
    connectBuildConfigurationSignals();
    updateConfiguration();
}

QWidget *NimRunConfiguration::createConfigurationWidget()
{
    return new NimRunConfigurationWidget(this);
}

QString NimRunConfiguration::executable() const
{
    return m_executable;
}

bool NimRunConfiguration::runInTerminal() const
{
    using namespace ProjectExplorer;
    return m_runMode == ApplicationLauncher::Console;
}

ProjectExplorer::ApplicationLauncher::Mode NimRunConfiguration::runMode() const
{
    using namespace ProjectExplorer;
    return m_runMode;
}

QString NimRunConfiguration::workingDirectory() const
{
    return m_workingDirectory;
}

QString NimRunConfiguration::commandLineArguments() const
{
    return m_commandLineArguments;
}

QVariantMap NimRunConfiguration::toMap() const
{
    auto result = LocalApplicationRunConfiguration::toMap();
    result[QStringLiteral("NimPlugin_Executable")] = m_executable;
    result[QStringLiteral("NimPlugin_WorkingDirectory")] = m_workingDirectory;
    result[QStringLiteral("NimPlugin_CommandLineArguments")] = m_commandLineArguments;
    result[QStringLiteral("NimPlugin_RunMode")] = m_runMode;
    return result;
}

bool NimRunConfiguration::fromMap(const QVariantMap &map)
{
    using namespace ProjectExplorer;
    bool result = LocalApplicationRunConfiguration::fromMap(map);
    if (!result)
        return result;
    m_executable = map[QStringLiteral("NimPlugin_Executable")].toString();
    m_workingDirectory = map[QStringLiteral("NimPlugin_WorkingDirectory")].toString();
    m_commandLineArguments = map[QStringLiteral("NimPlugin_CommandLineArguments")].toString();
    m_runMode = static_cast<ApplicationLauncher::Mode>(map[QStringLiteral("NimPlugin_RunMode")].toInt());
    return true;
}

void NimRunConfiguration::connectBuildConfigurationSignals()
{
    auto buildConfiguration = qobject_cast<NimBuildConfiguration*>(activeBuildConfiguration());
    Q_ASSERT(buildConfiguration);
    connect(buildConfiguration, SIGNAL(buildDirectoryChanged()), this, SLOT(updateConfiguration()));
    connect(buildConfiguration, SIGNAL(targetNimFileChanged(Utils::FileName)), this, SLOT(updateConfiguration()));
}

void NimRunConfiguration::setExecutable(const QString &executable)
{
    if (m_executable == executable)
        return;
    m_executable = executable;
    emit executableChanged(executable);
}

void NimRunConfiguration::setRunMode(ProjectExplorer::ApplicationLauncher::Mode mode)
{
    using namespace ProjectExplorer;
    if (m_runMode == mode)
        return;
    m_runMode = mode;
    emit runModeChanged(mode);
    emit runInTerminalChanged(mode == ApplicationLauncher::Console);
}

void NimRunConfiguration::setWorkingDirectory(const QString &workingDirectory)
{
    if (m_workingDirectory == workingDirectory)
        return;
    m_workingDirectory = workingDirectory;
    emit workingDirectoryChanged(workingDirectory);
}

void NimRunConfiguration::setCommandLineArguments(const QString &commandLineArguments)
{
    if (m_commandLineArguments == commandLineArguments)
        return;
    m_commandLineArguments = commandLineArguments;
    emit commandLineArgumentsChanged(commandLineArguments);
}

void NimRunConfiguration::setRunInTerminal(bool runInTerminal)
{
    using namespace ProjectExplorer;
    setRunMode(runInTerminal ? ApplicationLauncher::Console : ApplicationLauncher::Gui);
}

void NimRunConfiguration::updateConfiguration()
{
    auto buildConfiguration = qobject_cast<NimBuildConfiguration*>(activeBuildConfiguration());
    Q_ASSERT(buildConfiguration);
    Utils::FileName outFile = buildConfiguration->outFilePath();
    QFileInfo outFileInfo = outFile.toFileInfo();
    setExecutable(outFileInfo.absoluteFilePath());
    setWorkingDirectory(outFileInfo.absoluteDir().absolutePath());
}

void NimRunConfiguration::connectTargetSignals()
{
    connect(this->target(),
            SIGNAL(activeBuildConfigurationChanged(ProjectExplorer::BuildConfiguration*)),
            this,
            SLOT(updateConfiguration()));
}

}
