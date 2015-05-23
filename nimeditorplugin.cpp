#include "nimeditorplugin.h"
#include "nimeditor.h"
#include "nimeditorconstants.h"
#include "tools/nimhighlighter.h"

#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/id.h>
#include <coreplugin/editormanager/editormanager.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/texteditorconstants.h>
#include <utils/mimetypes/mimedatabase.h>

#include <QtPlugin>
#include <QCoreApplication>

using namespace NimEditor::Constants;

namespace NimEditor {

static NimEditorPlugin *m_instance = 0;

NimEditorPlugin::NimEditorPlugin()
{
    m_instance = this;
}

NimEditorPlugin::~NimEditorPlugin()
{
    m_instance = 0;
}

bool NimEditorPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    Utils::MimeDatabase::addMimeTypes(QLatin1String(":/nimeditor/NimEditor.mimetypes.xml"));

    addAutoReleasedObject(new NimEditorFactory);

    // Initialize editor actions handler
    // Add MIME overlay icons (these icons displayed at Project dock panel)
    const QIcon icon = QIcon::fromTheme(QLatin1String(C_NIM_MIME_ICON));
    if (!icon.isNull())
        Core::FileIconProvider::registerIconOverlayForMimeType(icon, C_NIM_MIMETYPE);

    return true;
}

} // namespace NimEditor
