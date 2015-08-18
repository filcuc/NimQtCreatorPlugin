#include "project/nimnewprojectwizardfactory.h"
#include "nimpluginconstants.h"

#include <coreplugin/basefilewizard.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/filewizardpage.h>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

namespace NimPlugin {

NimNewProjectWizardFactory::NimNewProjectWizardFactory()
{
    setId("Z.NimNewProjectWizard");
    setCategory(QLatin1String(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY));
    setDisplayCategory(QCoreApplication::translate("ProjectExplorer",
                                                   ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY_DISPLAY));
    setDisplayName(tr("Create a new Nim Project"));
    setDescription(tr("Creates a new Nim project."));
    setWizardKind(Core::IWizardFactory::ProjectWizard);
    setIcon(QIcon(QLatin1String(Constants::C_NIM_ICON_PATH)));
}

Core::BaseFileWizard *NimNewProjectWizardFactory::create(QWidget *parent,
                                                     const Core::WizardDialogParameters &parameters) const
{
    auto result = new Core::BaseFileWizard(this, parameters.extraValues(), parent);
    result->setWindowTitle(displayName());

    auto page = new Utils::FileWizardPage;
    page->setPath(parameters.defaultPath());
    result->addPage(page);

    foreach (QWizardPage *p, result->extensionPages())
        result->addPage(p);

    return result;
}

Core::GeneratedFiles NimNewProjectWizardFactory::generateFiles(const QWizard *widget, QString *) const
{
    const auto wizard = qobject_cast<const Core::BaseFileWizard *>(widget);
    const auto page = wizard->find<Utils::FileWizardPage>();
    const QString projectPath = page->path();
    const QDir dir(projectPath);
    const QString projectName = page->fileName();

    Core::GeneratedFiles result;
    {
        Core::GeneratedFile projectFile(QFileInfo(dir, projectName + QLatin1String(".nimproject")).absoluteFilePath());
        projectFile.setContents(QLatin1String("# Nim project\n"));
        projectFile.setAttributes(Core::GeneratedFile::OpenProjectAttribute);
        result << projectFile;
    }

    {
        Core::GeneratedFile mainFile(QFileInfo(dir, projectName + QLatin1String(".nim")).absoluteFilePath());
        mainFile.setContents(QLatin1String("if isMainModule:\n  echo \"Hello World\"\n"));
        mainFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);
        result << mainFile;
    }

    return result;
}

bool NimNewProjectWizardFactory::postGenerateFiles(const QWizard*, const Core::GeneratedFiles &files, QString *errorMessage) const
{
    return ProjectExplorer::CustomProjectWizard::postGenerateOpen(files, errorMessage);
}

}
