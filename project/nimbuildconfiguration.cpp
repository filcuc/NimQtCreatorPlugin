#include "project/nimbuildconfiguration.h"
#include "project/nimproject.h"

#include <projectexplorer/namedwidget.h>


namespace NimPlugin {

NimBuildConfiguration::NimBuildConfiguration(ProjectExplorer::Target *target)
    : ProjectExplorer::BuildConfiguration(target, Core::Id("NimBuildConfiguration"))
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

bool NimBuildConfiguration::fromMap(const QVariantMap &map)
{
    using namespace ProjectExplorer;

    if (!canRestore(map))
        return false;

    auto displayName = map[QLatin1String("displayName")].toString();
    auto buildDirectory = Utils::FileName::fromString(map[QLatin1String("buildDirectory")].toString());
    auto buildType = static_cast<BuildConfiguration::BuildType>(map[QLatin1String("buildType")].toInt());

    setDisplayName(displayName);
    setBuildDirectory(buildDirectory);
    setBuildType(buildType);

    return true;
}

QVariantMap NimBuildConfiguration::toMap() const
{
    QVariantMap result;
    result[QLatin1String("displayName")] = displayName();
    result[QLatin1String("buildDirectory")] = buildDirectory().toString();
    result[QLatin1String("buildType")] = buildType();
    return result;
}

bool NimBuildConfiguration::canRestore(const QVariantMap &map)
{
    return map.contains(QLatin1String("displayName"))
        && map.contains(QLatin1String("buildDirectory"))
        && map.contains(QLatin1String("buildType"));
}

}

