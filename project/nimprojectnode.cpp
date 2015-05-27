#include "project/nimprojectnode.h"
#include "project/nimproject.h"

namespace NimPlugin {

NimProjectNode::NimProjectNode(const Utils::FileName &projectFilePath)
    : ProjectExplorer::ProjectNode(projectFilePath)
{}

QList<ProjectExplorer::ProjectAction> NimProjectNode::supportedActions(Node *node) const
{
    using namespace ProjectExplorer;
    static const QList<ProjectAction> fileActions = { ProjectAction::Rename
                                                    , ProjectAction::RemoveFile };
    static const QList<ProjectAction> folderActions = { ProjectAction::AddNewFile
                                                      , ProjectAction::RemoveFile };
    switch (node->nodeType())
    {
    case FileNodeType:
        return fileActions;
    case FolderNodeType:
    case ProjectNodeType:
        return folderActions;
    default:
        return ProjectExplorer::ProjectNode::supportedActions(node);
    }
}

bool NimProjectNode::addSubProjects(const QStringList &)
{
    return false;
}

bool NimProjectNode::canAddSubProject(const QString &) const
{
    return false;
}

bool NimProjectNode::removeSubProjects(const QStringList &)
{
    return false;
}

bool NimProjectNode::addFiles(const QStringList &, QStringList *)
{
    return true;
}

bool NimProjectNode::removeFiles(const QStringList &, QStringList *)
{
    return true;
}

bool NimProjectNode::deleteFiles(const QStringList &)
{
    return true;
}

bool NimProjectNode::renameFile(const QString &, const QString &)
{
    return true;
}

}
