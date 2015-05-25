#include "project/nimimportprojectwizard.h"
#include "nimpluginconstants.h"

#include <coreplugin/basefilewizard.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/filewizardpage.h>
#include <QDebug>
#include <QDir>

namespace NimPlugin {

NimImportProjectWizard::NimImportProjectWizard()
{
    setWizardKind(Core::IWizardFactory::ProjectWizard);
    setDisplayName(tr("Import Existing Nim Project"));
    setId(QStringLiteral("Z.Nim"));
    setDescription(tr("Imports existing Nim projects."));
    setCategory(QLatin1String(ProjectExplorer::Constants::IMPORT_WIZARD_CATEGORY));
    setDisplayCategory(QLatin1String(ProjectExplorer::Constants::IMPORT_WIZARD_CATEGORY_DISPLAY));
    setIcon(QIcon(QLatin1String(Constants::C_NIM_ICON_PATH)));
}

Core::BaseFileWizard *NimImportProjectWizard::create(QWidget *parent,
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

Core::GeneratedFiles NimImportProjectWizard::generateFiles(const QWizard *widget, QString *) const
{
    const auto wizard = qobject_cast<const Core::BaseFileWizard *>(widget);
    const auto page = wizard->find<Utils::FileWizardPage>();
    const QString projectPath = page->path();
    const QDir dir(projectPath);
    const QString projectName = page->fileName();

    Core::GeneratedFile projectFile(QFileInfo(dir, projectName + QLatin1String(".nimproject")).absoluteFilePath());
    projectFile.setContents(QLatin1String("# Nim project\n"));
    projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    return {projectFile};
}

bool NimImportProjectWizard::postGenerateFiles(const QWizard*, const Core::GeneratedFiles &files, QString *errorMessage)
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}

}
