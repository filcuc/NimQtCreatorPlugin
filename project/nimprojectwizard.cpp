#include "project/nimprojectwizard.h"
#include "nimpluginconstants.h"

#include <coreplugin/basefilewizard.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/filewizardpage.h>
#include <QDebug>
#include <QDir>

namespace NimPlugin {

NimProjectWizard::NimProjectWizard()
{
    setWizardKind(Core::IWizardFactory::ProjectWizard);
    setDisplayName(tr("Import Existing Nim Project"));
    setId(QStringLiteral("Z.Nim"));
    setDescription(tr("Imports existing Nim projects."));
    setCategory(QLatin1String(ProjectExplorer::Constants::IMPORT_WIZARD_CATEGORY));
    setDisplayCategory(QLatin1String(ProjectExplorer::Constants::IMPORT_WIZARD_CATEGORY_DISPLAY));
    setIcon(QIcon(QLatin1String(Constants::C_NIM_ICON_PATH)));
}

Core::BaseFileWizard *NimProjectWizard::create(QWidget *parent, const Core::WizardDialogParameters &parameters) const
{
    Core::BaseFileWizard *wizard = new Core::BaseFileWizard(parent);
    wizard->setWindowTitle(displayName());

    Utils::FileWizardPage *page = new Utils::FileWizardPage;
    page->setPath(parameters.defaultPath());
    wizard->addPage(page);

    foreach (QWizardPage *p, parameters.extensionPages())
        wizard->addPage(p);

    return wizard;
}

Core::GeneratedFiles NimProjectWizard::generateFiles(const QWizard *widget, QString *) const
{
    const Core::BaseFileWizard *wizard = qobject_cast<const Core::BaseFileWizard *>(widget);
    Utils::FileWizardPage *page = wizard->find<Utils::FileWizardPage>();
    const QString projectPath = page->path();
    const QDir dir(projectPath);
    const QString projectName = page->fileName();

    Core::GeneratedFile projectFile(QFileInfo(dir, projectName + QLatin1String(".nimproject")).absoluteFilePath());
    projectFile.setContents(QLatin1String("# Nim project\n"));
    projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    return Core::GeneratedFiles() << projectFile;
}

bool NimProjectWizard::postGenerateFiles(const QWizard*, const Core::GeneratedFiles &files, QString *errorMessage)
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}

}
