#pragma once

#include <projectexplorer/runconfiguration.h>

namespace NimPlugin {

/// The factory for run configurations
class NimRunConfigurationFactory Q_DECL_FINAL : public ProjectExplorer::IRunConfigurationFactory
{
public:
    /// Constructor
    NimRunConfigurationFactory();

    QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode) const Q_DECL_OVERRIDE;

    QString displayNameForId(Core::Id id) const Q_DECL_OVERRIDE;

    bool canCreate(ProjectExplorer::Target *parent, Core::Id id) const Q_DECL_OVERRIDE;

    bool canRestore(ProjectExplorer::Target *parent,
                    const QVariantMap &map) const Q_DECL_OVERRIDE;

    bool canClone(ProjectExplorer::Target *parent,
                  ProjectExplorer::RunConfiguration *product) const Q_DECL_OVERRIDE;

    ProjectExplorer::RunConfiguration *clone(ProjectExplorer::Target *parent,
                                             ProjectExplorer::RunConfiguration *product) Q_DECL_OVERRIDE;

private:
    bool canHandle(ProjectExplorer::Target *parent) const;

    ProjectExplorer::RunConfiguration *doCreate(ProjectExplorer::Target *parent, Core::Id id);
    ProjectExplorer::RunConfiguration *doRestore(ProjectExplorer::Target *parent, const QVariantMap &map);
};

}
