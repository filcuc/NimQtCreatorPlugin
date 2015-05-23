#pragma once

#include <coreplugin/basefilewizardfactory.h>

namespace NimEditor {

class NimProjectWizard : public Core::BaseFileWizardFactory
{
    Q_OBJECT

public:
    NimProjectWizard();

protected:
    Core::BaseFileWizard *create(QWidget *parent, const Core::WizardDialogParameters &wizardDialogParameters) const Q_DECL_OVERRIDE;
    Core::GeneratedFiles generateFiles(const QWizard *widget, QString *) const Q_DECL_OVERRIDE;
    bool postGenerateFiles(const QWizard *, const Core::GeneratedFiles &files, QString *errorMessage);
};

}
