#pragma once

#include <projectexplorer/iprojectmanager.h>

namespace NimEditor {

class NimProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    NimProjectManager();

    virtual QString mimeType() const Q_DECL_OVERRIDE;

    virtual ProjectExplorer::Project *openProject(const QString &fileName, QString *errorString) Q_DECL_OVERRIDE;
};

}
