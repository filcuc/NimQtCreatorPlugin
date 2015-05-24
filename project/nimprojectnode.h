#pragma once

#include <projectexplorer/projectnodes.h>

namespace Utils { class FileName; }

namespace NimPlugin {

class NimProject;

class NimProjectNode Q_DECL_FINAL : public ProjectExplorer::ProjectNode
{
public:
    NimProjectNode(const Utils::FileName &projectFilePath);

    QList<ProjectExplorer::ProjectAction> supportedActions(Node *node) const Q_DECL_OVERRIDE;
    bool canAddSubProject(const QString &) const Q_DECL_OVERRIDE;
    bool addSubProjects(const QStringList &) Q_DECL_OVERRIDE;
    bool removeSubProjects(const QStringList &) Q_DECL_OVERRIDE;
    bool addFiles(const QStringList &, QStringList*) Q_DECL_OVERRIDE;
    bool removeFiles(const QStringList &, QStringList*) Q_DECL_OVERRIDE;
    bool deleteFiles(const QStringList &) Q_DECL_OVERRIDE;
    bool renameFile(const QString &, const QString &) Q_DECL_OVERRIDE;
};

}
