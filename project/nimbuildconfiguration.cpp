#include "project/nimbuildconfiguration.h"
#include "project/nimproject.h"

namespace NimPlugin {

NimBuildConfiguration::NimBuildConfiguration(ProjectExplorer::Target *target)
    : ProjectExplorer::BuildConfiguration(target, Core::Id("prova"))
{}

ProjectExplorer::NamedWidget *NimBuildConfiguration::createConfigWidget()
{
    return nullptr;
}

ProjectExplorer::BuildConfiguration::BuildType NimBuildConfiguration::buildType() const
{
    return m_buildType;
}

void NimBuildConfiguration::setBuildType(ProjectExplorer::BuildConfiguration::BuildType buildType)
{
    m_buildType = buildType;
}

void NimBuildConfiguration::setBuildDirectory(const Utils::FileName &dir)
{
    ProjectExplorer::BuildConfiguration::setBuildDirectory(dir);
}

}

