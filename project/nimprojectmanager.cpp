#include "nimprojectmanager.h"
#include "../nimeditorconstants.h"
#include "nimproject.h"

namespace NimEditor {

NimProjectManager::NimProjectManager()
{
}

QString NimProjectManager::mimeType() const
{
    return QLatin1String(Constants::C_NIM_PROJECT_MIMETYPE);
}

ProjectExplorer::Project *NimProjectManager::openProject(const QString &fileName, QString *errorString)
{
    if (!QFileInfo(fileName).isFile()) {
        *errorString = tr("Failed opening project '%1': Project is not a file").arg(fileName);
        return nullptr;
    }

    return new NimProject(this, fileName);
}

}
