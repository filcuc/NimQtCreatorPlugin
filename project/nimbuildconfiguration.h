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
    virtual ProjectExplorer::NamedWidget *createConfigWidget();
    virtual ProjectExplorer::BuildConfiguration::BuildType buildType() const;
    void setBuildType(ProjectExplorer::BuildConfiguration::BuildType buildType);
    void setBuildDirectory(const Utils::FileName &dir) Q_DECL_OVERRIDE;

private:
    ProjectExplorer::BuildConfiguration::BuildType m_buildType;
};

}
