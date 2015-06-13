#pragma once

#include <projectexplorer/localapplicationrunconfiguration.h>

namespace NimPlugin {

/// Nim run configuration
class NimRunConfiguration : public ProjectExplorer::LocalApplicationRunConfiguration
{
    Q_OBJECT

public:
    /// Constructor
    NimRunConfiguration(ProjectExplorer::Target *parent);

    // RunConfiguration interface
    QWidget *createConfigurationWidget() Q_DECL_OVERRIDE;

    // LocalApplicationRunConfiguration interface
    /// Return the executable name
    QString executable() const Q_DECL_OVERRIDE;

    /// Return true if run in terminal
    bool runInTerminal() const;

    /// Return the run mode
    ProjectExplorer::ApplicationLauncher::Mode runMode() const Q_DECL_OVERRIDE;

    /// Return the working directory
    QString workingDirectory() const Q_DECL_OVERRIDE;

    /// Return the commandline args
    QString commandLineArguments() const Q_DECL_OVERRIDE;

    /// To map
    QVariantMap toMap() const Q_DECL_OVERRIDE;

    /// From map
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;

public Q_SLOTS:
    /// Sets the executable path
    void setExecutable(const QString& path);

    /// Sets the run mode
    void setRunMode(ProjectExplorer::ApplicationLauncher::Mode);

    /// Sets the working directory path
    void setWorkingDirectory(const QString &path);

    /// Sets the command line aguments
    void setCommandLineArguments(const QString& args);

    /// Sets the run in terminal
    void setRunInTerminal(bool runInTerminal);

Q_SIGNALS:
    /// Emitted on executable changed
    void executableChanged(const QString& args);

    /// Emitted on runMode changed
    void runModeChanged(ProjectExplorer::ApplicationLauncher::Mode);

    /// Emitted on working directory changed
    void workingDirectoryChanged(const QString& workingDirectory);

    /// Emmited on command line args changed
    void commandLineArgumentsChanged(const QString& args);

    /// Emitted on run in terminal changed
    void runInTerminalChanged(bool);

private Q_SLOTS:
    /// Update the run configuration settings with those of build
    void updateConfiguration();

private:
    /// Connects the target signals
    void connectTargetSignals();

    /// Connects the build configuration signals
    void connectBuildConfigurationSignals();

    QString m_executable;
    QString m_workingDirectory;
    QString m_commandLineArguments;
    ProjectExplorer::ApplicationLauncher::Mode m_runMode;
};

}
