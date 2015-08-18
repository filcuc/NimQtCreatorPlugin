#include "nimrunconfigurationfactory.h"
#include "nimproject.h"
#include "nimrunconfiguration.h"

#include <projectexplorer/target.h>
#include <debugger/debuggerrunconfigurationaspect.h>

namespace NimPlugin {

NimRunConfigurationFactory::NimRunConfigurationFactory()
{
}

QList<Core::Id> NimRunConfigurationFactory::availableCreationIds(ProjectExplorer::Target *parent,
                                                                 ProjectExplorer::IRunConfigurationFactory::CreationMode mode) const
{
    Q_UNUSED(mode);
    if (canHandle(parent))
        return { Core::Id("NimRunId") };
    return {};
}

QString NimRunConfigurationFactory::displayNameForId(Core::Id id) const
{
    QString result = id.toString() + QStringLiteral("-TempRunConf");
    return result;
}

bool NimRunConfigurationFactory::canCreate(ProjectExplorer::Target *parent, Core::Id id) const
{
    Q_UNUSED(id);
    return canHandle(parent);
}

bool NimRunConfigurationFactory::canRestore(ProjectExplorer::Target *parent,
                                            const QVariantMap &map) const
{
    Q_UNUSED(parent);
    Q_UNUSED(map);
    return canHandle(parent);
}

bool NimRunConfigurationFactory::canClone(ProjectExplorer::Target *parent,
                                          ProjectExplorer::RunConfiguration *product) const
{
    Q_UNUSED(parent);
    Q_UNUSED(product);
    return false;
}

ProjectExplorer::RunConfiguration *NimRunConfigurationFactory::clone(ProjectExplorer::Target *parent,
                                                                     ProjectExplorer::RunConfiguration *product)
{
    Q_UNUSED(parent);
    Q_UNUSED(product);
    return nullptr;
}

bool NimRunConfigurationFactory::canHandle(ProjectExplorer::Target *parent) const
{
    Q_UNUSED(parent);
    return qobject_cast<NimProject*>(parent->project());
}

ProjectExplorer::RunConfiguration *NimRunConfigurationFactory::doCreate(ProjectExplorer::Target *parent,
                                                                        Core::Id id)
{
    Q_UNUSED(id);
    auto result = new NimRunConfiguration(parent);
    return result;
}

ProjectExplorer::RunConfiguration *NimRunConfigurationFactory::doRestore(ProjectExplorer::Target *parent,
                                                                         const QVariantMap &map)
{
    Q_UNUSED(map);
    auto result = new NimRunConfiguration(parent);
    result->fromMap(map);
    return result;
}

}
