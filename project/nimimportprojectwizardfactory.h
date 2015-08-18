#pragma once

#include <coreplugin/basefilewizardfactory.h>

namespace NimPlugin {

class NimImportProjectWizardFactory Q_DECL_FINAL : public Core::BaseFileWizardFactory
{
    Q_OBJECT

public:
    NimImportProjectWizardFactory();

protected:
    Core::BaseFileWizard *create(QWidget *parent,
                                 const Core::WizardDialogParameters &params) const Q_DECL_OVERRIDE;

    Core::GeneratedFiles generateFiles(const QWizard *wizard,
                                       QString *errorMessage) const Q_DECL_OVERRIDE;

    bool postGenerateFiles(const QWizard *wizard,
                           const Core::GeneratedFiles &files, QString *errorMessage) const Q_DECL_OVERRIDE;
};

}
