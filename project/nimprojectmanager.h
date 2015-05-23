#pragma once

#include <projectexplorer/iprojectmanager.h>

namespace NimPlugin {

class NimProjectManager Q_DECL_FINAL : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    NimProjectManager();

    QString mimeType() const Q_DECL_OVERRIDE;

    ProjectExplorer::Project *openProject(const QString &fileName, QString *errorString) Q_DECL_OVERRIDE;
};

}
