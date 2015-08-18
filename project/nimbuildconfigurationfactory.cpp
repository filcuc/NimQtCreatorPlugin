#include "nimbuildconfigurationfactory.h"
#include "nimbuildconfiguration.h"
#include "nimbuildinfo.h"
#include "nimproject.h"
#include "nimcompilerbuildstep.h"
#include "nimcompilercleanstep.h"

#include <utils/qtcassert.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/buildstep.h>


namespace NimPlugin {

NimBuildConfigurationFactory::NimBuildConfigurationFactory(QObject *parent)
    : IBuildConfigurationFactory(parent)
{}

NimBuildConfigurationFactory::~NimBuildConfigurationFactory()
{

}

QList<ProjectExplorer::BuildInfo *> NimBuildConfigurationFactory::availableBuilds(const ProjectExplorer::Target *parent) const
{
    using namespace ProjectExplorer;

    // Retrieve the project path
    auto nimProject = qobject_cast<NimProject*>(parent->project());
    if (!nimProject)
        return {};

    // Create the build info
    NimBuildInfo *info = createBuildInfo(parent->kit()->id(),
                                         nimProject->path().toString(),
                                         BuildConfiguration::Debug);
    return { info };
}

QList<ProjectExplorer::BuildInfo *> NimBuildConfigurationFactory::availableSetups(const ProjectExplorer::Kit *k,
                                                                                  const QString &projectPath) const
{
    using namespace ProjectExplorer;

    NimBuildInfo *debug = createBuildInfo(k->id(), projectPath, BuildConfiguration::Debug);
    NimBuildInfo *release = createBuildInfo(k->id(), projectPath, BuildConfiguration::Release);
    return { debug, release };
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::create(ProjectExplorer::Target *parent,
                                                                          const ProjectExplorer::BuildInfo *info) const
{
    using namespace ProjectExplorer;

    auto project = qobject_cast<NimProject*>(parent->project());
    Q_ASSERT(project);

    auto nimInfo = static_cast<const NimBuildInfo *>(info);

    // Create the build configuration and initialize it from build info
    auto result = new NimBuildConfiguration(parent);
    result->setDisplayName(nimInfo->displayName);
    result->setDefaultDisplayName(nimInfo->displayName);
    result->setBuildType(nimInfo->buildType);
    result->setBuildDirectory(nimInfo->buildDirectory);
    result->setTargetNimFile(project->nimFiles().first()); // TODO: this is a temporary solution

    // Add nim compiler build step
    BuildStepList* buildSteps = result->stepList(Core::Id(Constants::BUILDSTEPS_BUILD));
    buildSteps->appendStep(new NimCompilerBuildStep(buildSteps));

    // Add clean step
    BuildStepList* cleanSteps = result->stepList(Core::Id(Constants::BUILDSTEPS_CLEAN));
    cleanSteps->appendStep(new NimCompilerCleanStep(cleanSteps));

    return result;
}

bool NimBuildConfigurationFactory::canRestore(const ProjectExplorer::Target *parent,
                                              const QVariantMap &map) const
{
    Q_UNUSED(parent);
    return NimBuildConfiguration::canRestore(map);
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::restore(ProjectExplorer::Target *parent,
                                                                           const QVariantMap &map)
{
    using namespace ProjectExplorer;

    Q_ASSERT(canRestore(parent, map));

    // Create the build configuration
    auto result = new NimBuildConfiguration(parent);

    // Restore from map
    auto status = result->fromMap(map);
    Q_ASSERT(status);

    // Add nim compiler build step
    BuildStepList* buildSteps = result->stepList(Core::Id(Constants::BUILDSTEPS_BUILD));
    buildSteps->appendStep(new NimCompilerBuildStep(buildSteps));

    // Add clean step
    BuildStepList* cleanSteps = result->stepList(Core::Id(Constants::BUILDSTEPS_CLEAN));
    cleanSteps->appendStep(new NimCompilerCleanStep(cleanSteps));

    return result;
}

bool NimBuildConfigurationFactory::canClone(const ProjectExplorer::Target *parent,
                                            ProjectExplorer::BuildConfiguration *product) const
{
    Q_UNUSED(parent);
    Q_UNUSED(product);
    return false;
}

ProjectExplorer::BuildConfiguration *NimBuildConfigurationFactory::clone(ProjectExplorer::Target *parent,
                                                                         ProjectExplorer::BuildConfiguration *product)
{
    Q_UNUSED(parent);
    Q_UNUSED(product);
    return nullptr;
}

int NimBuildConfigurationFactory::priority(const ProjectExplorer::Kit *k, const QString &projectPath) const
{
    Q_UNUSED(k);
    Q_UNUSED(projectPath);
    return 0;
}

int NimBuildConfigurationFactory::priority(const ProjectExplorer::Target *parent) const
{
    Q_UNUSED(parent);
    return 0;
}

Utils::FileName NimBuildConfigurationFactory::defaultBuildDirectory(const QString& projectPath,
                                                                    ProjectExplorer::BuildConfiguration::BuildType type)
{
    using namespace ProjectExplorer;

    QFileInfo projectFileInfo(projectPath);
    QDir parentDir = projectFileInfo.absoluteDir(); parentDir.cdUp();

    QString buildPath = parentDir.absolutePath()
            + QDir::separator()
            + QStringLiteral("build-")
            + projectFileInfo.baseName()
            + QStringLiteral("-") + buildTypeToString(type);

    return Utils::FileName::fromString(buildPath);
}

QString NimBuildConfigurationFactory::buildTypeToString(ProjectExplorer::BuildConfiguration::BuildType type)
{
    using namespace ProjectExplorer;

    switch (type) {
    case BuildConfiguration::Release:
        return QStringLiteral("Release");
    case BuildConfiguration::Debug:
        return QStringLiteral("Debug");
    default:
        return QStringLiteral("Unknown");
    }
}

NimBuildInfo *NimBuildConfigurationFactory::createBuildInfo(Core::Id kitId,
                                                            const QString &projectPath,
                                                            ProjectExplorer::BuildConfiguration::BuildType buildType) const
{
    using namespace ProjectExplorer;
    auto result = new NimBuildInfo(this);
    result->displayName = QStringLiteral("Nim%1Build").arg(buildTypeToString(buildType));
    result->buildDirectory = defaultBuildDirectory(projectPath, buildType);
    result->buildType = buildType;
    result->kitId = kitId;
    return result;
}

}
