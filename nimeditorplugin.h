#pragma once

#include <extensionsystem/iplugin.h>
#include <QSet>

namespace NimEditor {
namespace Internal {

class NimEditorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "NimEditor.json")

public:
    NimEditorPlugin();
    virtual ~NimEditorPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorMessage);
    virtual void extensionsInitialized() {}
};

} // namespace Internal
} // namespace NimEditor
