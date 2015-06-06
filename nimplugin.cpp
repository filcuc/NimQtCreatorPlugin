#include "nimplugin.h"
#include "nimpluginconstants.h"
#include "editor/nimeditor.h"
#include "editor/nimhighlighter.h"
#include "project/nimprojectmanager.h"
#include "project/nimimportprojectwizard.h"
#include "project/nimnewprojectwizard.h"
#include "project/nimnewfilewizard.h"
#include "project/nimbuildconfigurationfactory.h"
#include "project/nimrunconfigurationfactory.h"

#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/id.h>
#include <coreplugin/editormanager/editormanager.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/texteditorconstants.h>
#include <utils/mimetypes/mimedatabase.h>

#include <QtPlugin>

using namespace NimPlugin::Constants;

namespace NimPlugin {

static NimPlugin *m_instance = 0;

NimPlugin::NimPlugin()
{
    m_instance = this;
}

NimPlugin::~NimPlugin()
{
    m_instance = 0;
}

bool NimPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    Utils::MimeDatabase::addMimeTypes(QLatin1String(":/NimPlugin.mimetypes.xml"));

    addAutoReleasedObject(new NimEditorFactory);
    addAutoReleasedObject(new NimProjectManager);
    addAutoReleasedObject(new NimImportProjectWizard);
    addAutoReleasedObject(new NimNewProjectWizard);
    addAutoReleasedObject(new NimNewFileWizard);
    addAutoReleasedObject(new NimBuildConfigurationFactory);
    addAutoReleasedObject(new NimRunConfigurationFactory);

    // Initialize editor actions handler
    // Add MIME overlay icons (these icons displayed at Project dock panel)
    const QIcon icon (QLatin1String(Constants::C_NIM_ICON_PATH));
    if (!icon.isNull())
        Core::FileIconProvider::registerIconOverlayForMimeType(icon, C_NIM_MIMETYPE);

    return true;
}

} // namespace NimEditor
