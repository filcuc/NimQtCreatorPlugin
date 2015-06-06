#include "nimrunconfigurationwidget.h"
#include "ui_nimrunconfigurationwidget.h"
#include "nimrunconfiguration.h"

namespace NimPlugin {

NimRunConfigurationWidget::NimRunConfigurationWidget(NimRunConfiguration *rc,
                                                     QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::NimRunConfigurationWidget)
{
    Q_ASSERT(rc);
    m_ui->setupUi(this);
    connectRunConfigurationSignals(rc);
    updateUI(rc);
}

NimRunConfigurationWidget::~NimRunConfigurationWidget()
{}

void NimRunConfigurationWidget::updateUI(NimRunConfiguration *rc)
{
    using namespace ProjectExplorer;
    m_ui->executableLineEdit->setText(rc->executable());
    m_ui->workingDirectoryLineEdit->setText(rc->workingDirectory());
    m_ui->argumentsLineEdit->setText(rc->commandLineArguments());
    m_ui->runInTerminalCheckBox->setChecked(rc->runInTerminal());
}

void NimRunConfigurationWidget::connectRunConfigurationSignals(NimRunConfiguration *rc)
{
    connect(rc, SIGNAL(executableChanged(QString)), m_ui->executableLineEdit, SLOT(setText(QString)));
    connect(rc, SIGNAL(workingDirectoryChanged(QString)), m_ui->workingDirectoryLineEdit, SLOT(setText(QString)));
    connect(rc, SIGNAL(commandLineArgumentsChanged(QString)), m_ui->argumentsLineEdit, SLOT(setText(QString)));
    connect(rc, SIGNAL(runInTerminalChanged(bool)), m_ui->runInTerminalCheckBox, SLOT(setChecked(bool)));

    connect(m_ui->argumentsLineEdit, SIGNAL(textEdited(QString)), rc, SLOT(setCommandLineArguments(QString)));
    connect(m_ui->runInTerminalCheckBox, SIGNAL(toggled(bool)), rc, SLOT(setRunInTerminal(bool)));
}

}
