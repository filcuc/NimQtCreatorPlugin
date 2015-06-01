#pragma once

// Qt
#include <QPointer>
// QtCreator
#include <projectexplorer/target.h>
#include <projectexplorer/buildconfiguration.h>

namespace NimPlugin {

class NimProject;

class NimBuildConfiguration : public ProjectExplorer::BuildConfiguration
{
public:
    NimBuildConfiguration(ProjectExplorer::Target *target);

public:
    ProjectExplorer::NamedWidget *createConfigWidget() Q_DECL_OVERRIDE;
    ProjectExplorer::BuildConfiguration::BuildType buildType() const Q_DECL_OVERRIDE;
    void setBuildType(ProjectExplorer::BuildConfiguration::BuildType buildType);
    void setBuildDirectory(const Utils::FileName &dir) Q_DECL_OVERRIDE;
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;
    static bool canRestore(const QVariantMap &map);

private:
    ProjectExplorer::BuildConfiguration::BuildType m_buildType;
};

}
