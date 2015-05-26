#pragma once

#include <projectexplorer/buildinfo.h>
#include <projectexplorer/buildconfiguration.h>

namespace NimPlugin {

class NimBuildConfigurationFactory;

class NimBuildInfo : public ProjectExplorer::BuildInfo
{
public:
    NimBuildInfo(const NimBuildConfigurationFactory* factory);

    ProjectExplorer::BuildConfiguration::BuildType buildType;
};

}
