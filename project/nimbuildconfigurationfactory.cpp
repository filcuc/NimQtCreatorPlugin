#include "nimbuildconfigurationfactory.h"
#include "nimbuildconfiguration.h"
#include "nimbuildinfo.h"
#include "nimproject.h"
#include "nimcompilerbuildstep.h"

#include <utils/qtcassert.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/buildstep.h>


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

    auto tmp = QFileInfo(projectPath);
    QDir dir = tmp.absoluteDir();
    dir.cdUp();
    QString buildDirectory = dir.absolutePath()
            + QDir::separator()
            + QLatin1String("build-") + tmp.baseName();

    // Create the debug build info
    auto debugBuild = new NimBuildInfo(this);
    debugBuild->displayName = QLatin1String("NimDebugBuild");
    debugBuild->buildDirectory = Utils::FileName::fromString(buildDirectory + QLatin1String("-Debug"));
    debugBuild->kitId = k->id();
    debugBuild->buildType = ProjectExplorer::BuildConfiguration::Debug;

    // Create the release build info
    auto releaseBuild = new NimBuildInfo(this);
    releaseBuild->displayName = QLatin1String("NimReleaseBuild");
    releaseBuild->buildDirectory = Utils::FileName::fromString(buildDirectory + QLatin1String("-Release"));
    releaseBuild->kitId = k->id();
    releaseBuild->buildType = ProjectExplorer::BuildConfiguration::Release;

    return { debugBuild, releaseBuild };
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::create(ProjectExplorer::Target *parent,
                                                                          const ProjectExplorer::BuildInfo *info) const
{
    using namespace ProjectExplorer;

    auto project = dynamic_cast<NimProject*>(parent->project());
    Q_ASSERT(project);

    auto nimInfo = static_cast<const NimBuildInfo *>(info);
    auto result = new NimBuildConfiguration(parent);
    result->setDisplayName(nimInfo->displayName);
    result->setDefaultDisplayName(nimInfo->displayName);
    result->setBuildType(nimInfo->buildType);
    result->setBuildDirectory(nimInfo->buildDirectory);

    // Add nim compiler build step
    BuildStepList* buildSteps = result->stepList(Core::Id(Constants::BUILDSTEPS_BUILD));
    auto nimCompilerStep = new NimCompilerBuildStep(buildSteps);
    nimCompilerStep->setTarget(project->nimFiles().first());
    buildSteps->appendStep(nimCompilerStep);

    return result;
}

bool NimBuildConfigurationFactory::canRestore(const ProjectExplorer::Target *parent,
                                              const QVariantMap &map) const
{
    return NimBuildConfiguration::canRestore(map);
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::restore(ProjectExplorer::Target *parent,
                                                                           const QVariantMap &map)
{
    Q_ASSERT(canRestore(parent, map));
    auto result = new NimBuildConfiguration(parent);
    auto status = result->fromMap(map);
    Q_ASSERT(status);
    return result;
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
