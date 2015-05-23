#pragma once

#include <extensionsystem/iplugin.h>

namespace NimPlugin {

class NimPlugin Q_DECL_FINAL : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "NimPlugin.json")

public:
    /// Constructor
    NimPlugin();

    /// Destructor
    virtual ~NimPlugin();

    /// Initialize the plugin
    bool initialize(const QStringList &arguments, QString *errorMessage) Q_DECL_OVERRIDE;

    /// Extensions initialized
    void extensionsInitialized() Q_DECL_OVERRIDE {}
};

} // namespace NimEditor
