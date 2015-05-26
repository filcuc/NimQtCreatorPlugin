#pragma once

#include <projectexplorer/buildconfiguration.h>

namespace NimPlugin {

class NimBuildConfigurationFactory Q_DECL_FINAL : public ProjectExplorer::IBuildConfigurationFactory
{
    Q_OBJECT

public:
    /// Constructor
    explicit NimBuildConfigurationFactory(QObject *parent = 0);

    /// Destructor
    virtual ~NimBuildConfigurationFactory();

    QList<ProjectExplorer::BuildInfo *> availableBuilds(const ProjectExplorer::Target *parent) const Q_DECL_OVERRIDE;

    QList<ProjectExplorer::BuildInfo *> availableSetups(const ProjectExplorer::Kit *k,
                                                        const QString &projectPath) const Q_DECL_OVERRIDE;

    ProjectExplorer::BuildConfiguration *create(ProjectExplorer::Target *parent,
                                                const ProjectExplorer::BuildInfo *info) const Q_DECL_OVERRIDE;

    bool canRestore(const ProjectExplorer::Target *parent,
                    const QVariantMap &map) const Q_DECL_OVERRIDE;

    ProjectExplorer::BuildConfiguration *restore(ProjectExplorer::Target *parent,
                                                 const QVariantMap &map) Q_DECL_OVERRIDE;

    bool canClone(const ProjectExplorer::Target *parent,
                  ProjectExplorer::BuildConfiguration *product) const Q_DECL_OVERRIDE;

    ProjectExplorer::BuildConfiguration *clone(ProjectExplorer::Target *parent,
                                               ProjectExplorer::BuildConfiguration *product) Q_DECL_OVERRIDE;

    int priority(const ProjectExplorer::Kit *k, const QString &projectPath) const Q_DECL_OVERRIDE;

    int priority(const ProjectExplorer::Target *parent) const Q_DECL_OVERRIDE;
};

}

