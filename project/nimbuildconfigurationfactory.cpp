#include "nimbuildconfigurationfactory.h"
#include "nimbuildconfiguration.h"
#include "nimbuildinfo.h"
#include "nimproject.h"

#include <utils/qtcassert.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/target.h>

namespace NimPlugin {

NimBuildConfigurationFactory::NimBuildConfigurationFactory(QObject *parent)
{

}

NimBuildConfigurationFactory::~NimBuildConfigurationFactory()
{

}

QList<ProjectExplorer::BuildInfo *> NimBuildConfigurationFactory::availableBuilds(const ProjectExplorer::Target *parent) const
{
    using namespace ProjectExplorer;

    // Retrieve the project path
    auto nimProject = dynamic_cast<NimProject*>(parent->project());
    if (!nimProject)
        return {};

    // Create the build info
    auto info = new NimBuildInfo(this);
    info->displayName = QLatin1String("NimDebugBuild");
    info->buildDirectory = nimProject->path();
    info->kitId = parent->kit()->id();
    info->buildType = ProjectExplorer::BuildConfiguration::Debug;

    return { info };
}

QList<ProjectExplorer::BuildInfo *> NimBuildConfigurationFactory::availableSetups(const ProjectExplorer::Kit *k,
                                                                                  const QString &projectPath) const
{
    using namespace ProjectExplorer;

    // Create the debug build info
    auto debugBuild = new NimBuildInfo(this);
    debugBuild->displayName = QLatin1String("NimDebugBuild");
    debugBuild->buildDirectory = Utils::FileName::fromString(projectPath);
    debugBuild->kitId = k->id();
    debugBuild->buildType = ProjectExplorer::BuildConfiguration::Debug;

    // Create the release build info
    auto releaseBuild = new NimBuildInfo(this);
    releaseBuild->displayName = QLatin1String("NimReleaseBuild");
    releaseBuild->buildDirectory = Utils::FileName::fromString(projectPath);
    releaseBuild->kitId = k->id();
    releaseBuild->buildType = ProjectExplorer::BuildConfiguration::Release;

    return { debugBuild, releaseBuild };
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::create(ProjectExplorer::Target *parent,
                                                                          const ProjectExplorer::BuildInfo *info) const
{
    auto nimInfo = static_cast<const NimBuildInfo *>(info);
    auto result = new NimBuildConfiguration(parent);
    result->setDisplayName(nimInfo->displayName);
    result->setDefaultDisplayName(nimInfo->displayName);
    result->setBuildType(nimInfo->buildType);
    result->setBuildDirectory(nimInfo->buildDirectory);
    return result;
}

bool NimBuildConfigurationFactory::canRestore(const ProjectExplorer::Target *parent,
                                              const QVariantMap &map) const
{
    return false;
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::restore(ProjectExplorer::Target *parent,
                                                                           const QVariantMap &map)
{
    return nullptr;
}

bool NimBuildConfigurationFactory::canClone(const ProjectExplorer::Target *parent,
                                            ProjectExplorer::BuildConfiguration *product) const
{
    return false;
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::clone(ProjectExplorer::Target *parent,
                                                                         ProjectExplorer::BuildConfiguration *product)
{
    return nullptr;
}

int NimBuildConfigurationFactory::priority(const ProjectExplorer::Kit *k, const QString &projectPath) const
{
    return 0;
}

int NimBuildConfigurationFactory::priority(const ProjectExplorer::Target *parent) const
{
    return 0;
}

}
