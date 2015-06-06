#include "project/nimnewfilewizard.h"
#include "nimpluginconstants.h"

#include <coreplugin/basefilewizard.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <utils/filewizardpage.h>
#include <QDebug>
#include <QDir>

namespace NimPlugin {

NimNewFileWizard::NimNewFileWizard()
{
    setWizardKind(Core::IWizardFactory::FileWizard);
    setDisplayName(tr("Nim source file"));
    setId(QStringLiteral("Z.Nim"));
    setDescription(tr("Nim source file"));
    setCategory(QLatin1String("Nim"));
    setDisplayCategory(QLatin1String("Nim"));
    setIcon(QIcon(QLatin1String(Constants::C_NIM_ICON_PATH)));
}

Core::BaseFileWizard *NimNewFileWizard::create(QWidget *parent,
                                               const Core::WizardDialogParameters &parameters) const
{
    auto result = new Core::BaseFileWizard(parent);
    result->setWindowTitle(displayName());

    auto page = new Utils::FileWizardPage;
    page->setPath(parameters.defaultPath());
    result->addPage(page);

    foreach (QWizardPage *p, parameters.extensionPages())
        result->addPage(p);

    return result;
}

Core::GeneratedFiles NimNewFileWizard::generateFiles(const QWizard *widget, QString *) const
{
    const auto wizard = qobject_cast<const Core::BaseFileWizard *>(widget);
    const auto page = wizard->find<Utils::FileWizardPage>();
    const QString filePath = page->path();
    const QDir dir(filePath);
    QString fileName = page->fileName();
    if (!fileName.endsWith(QLatin1String(".nim")))
        fileName += QLatin1String(".nim");

    Core::GeneratedFile projectFile(QFileInfo(dir, fileName).absoluteFilePath());
    projectFile.setContents(QLatin1String("if isMainModule:\n  discard()"));
    projectFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return {projectFile};
}

bool NimNewFileWizard::postGenerateFiles(const QWizard*, const Core::GeneratedFiles &files, QString *errorMessage)
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}

}
