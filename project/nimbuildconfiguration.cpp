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

    auto displayName = map[QStringLiteral("displayName")].toString();
    auto buildDirectory = Utils::FileName::fromString(map[QStringLiteral("buildDirectory")].toString());
    auto buildType = static_cast<BuildConfiguration::BuildType>(map[QStringLiteral("buildType")].toInt());
    auto userCompilerOptions = map[QStringLiteral("userCompilerOptions")].toString().split(QLatin1Char('|'));
    auto targetNimFile = Utils::FileName::fromString(map[QStringLiteral("targetNimFile")].toString());

    setDisplayName(displayName);
    setBuildDirectory(buildDirectory);
    setBuildType(buildType);
    setUserCompilerOptions(userCompilerOptions);
    setTargetNimFile(targetNimFile);

    return true;
}

QVariantMap NimBuildConfiguration::toMap() const
{
    QVariantMap result;
    result[QStringLiteral("displayName")] = displayName();
    result[QStringLiteral("buildDirectory")] = buildDirectory().toString();
    result[QStringLiteral("buildType")] = buildType();
    result[QStringLiteral("userCompilerOptions")] = userCompilerOptions().join(QLatin1Char('|'));
    result[QStringLiteral("targetNimFile")] = targetNimFile().toString();
    return result;
}

Utils::FileName NimBuildConfiguration::cacheDirectory() const
{
    return buildDirectory();
}

Utils::FileName NimBuildConfiguration::outFilePath() const
{
    return buildDirectory().appendPath(QStringLiteral("outFile"));
}

QStringList NimBuildConfiguration::compilerOptions() const
{
    using namespace ProjectExplorer;

    QStringList result;

    switch(m_buildType) {
    case Release:
        result << QStringLiteral("-d:release");
        break;
    case Debug:
        result << QStringLiteral("--debugInfo")
               << QStringLiteral("--lineDir:on");
        break;
    default:
        break;
    }

    // Sets the output file and cache dir
    result << QStringLiteral("--out:%1").arg(outFilePath().toString());
    result << QStringLiteral("--nimCache:%1").arg(cacheDirectory().toString());

    return result;
}

QStringList NimBuildConfiguration::userCompilerOptions() const
{
    return m_userCompilerOptions;
}

void NimBuildConfiguration::setUserCompilerOptions(const QStringList &options)
{
    m_userCompilerOptions = options;
    emit userCompilerOptionsChanged(options);
}

bool NimBuildConfiguration::canRestore(const QVariantMap &map)
{
    return map.contains(QStringLiteral("displayName"))
        && map.contains(QStringLiteral("buildDirectory"))
        && map.contains(QStringLiteral("buildType"))
        && map.contains(QStringLiteral("userCompilerOptions"))
        && map.contains(QStringLiteral("targetNimFile"));
}

Utils::FileName NimBuildConfiguration::targetNimFile() const
{
    return m_targetNimFile;
}

void NimBuildConfiguration::setTargetNimFile(const Utils::FileName &nimFile)
{
    if (nimFile == m_targetNimFile)
        return;
    m_targetNimFile = nimFile;
    emit targetNimFileChanged(nimFile);
}

}

