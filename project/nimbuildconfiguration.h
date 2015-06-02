#pragma once

// Qt
#include <QPointer>
// QtCreator
#include <projectexplorer/target.h>
#include <projectexplorer/buildconfiguration.h>

namespace NimPlugin {

class NimProject;

/// A Nim build configuration
class NimBuildConfiguration : public ProjectExplorer::BuildConfiguration
{
    Q_OBJECT

public:
    /// Constructor
    NimBuildConfiguration(ProjectExplorer::Target *target);

    /// Return the configuration widget
    ProjectExplorer::NamedWidget *createConfigWidget() Q_DECL_OVERRIDE;

    /// Return the build type
    ProjectExplorer::BuildConfiguration::BuildType buildType() const Q_DECL_OVERRIDE;

    /// Sets the build type
    void setBuildType(ProjectExplorer::BuildConfiguration::BuildType buildType);

    /// Sets the build directory
    void setBuildDirectory(const Utils::FileName &dir) Q_DECL_OVERRIDE;

    /// Return truf if the build configuration has been successfully restore
    /// from the given map, false otherwise
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;

    /// Dump the build configuration as a variant map
    QVariantMap toMap() const Q_DECL_OVERRIDE;

    /// Return the nim compiler cache dir
    Utils::FileName cacheDirectory() const;

    /// Return the outfile path
    Utils::FileName outFilePath() const;

    /// Return the compiler options for this build type
    QStringList compilerOptions() const;

    /// Return the user compiler options
    QStringList userCompilerOptions() const;

    /// Sets the user compiler options
    void setUserCompilerOptions(const QStringList &options);

    /// Return true if the build configuration can be restore from
    /// the given map, false otherwise
    static bool canRestore(const QVariantMap &map);

    /// Return the nim file associated to the target of this configuration
    Utils::FileName targetNimFile() const;

    /// Sets the target nim file of this configuration
    void setTargetNimFile(const Utils::FileName &nimFile);

signals:
    /// Emitted when the user compiler options have been changed
    void userCompilerOptionsChanged(const QStringList &options);

    /// Emitted when the target nim file has been changed
    void targetNimFileChanged(const Utils::FileName &nimFile);

private:
    /// The build type
    ProjectExplorer::BuildConfiguration::BuildType m_buildType;

    /// The user compiler options
    QStringList m_userCompilerOptions;

    /// The user target Nim file
    Utils::FileName m_targetNimFile;
};

}
