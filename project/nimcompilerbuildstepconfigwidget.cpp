#include "nimcompilerbuildstepconfigwidget.h"
#include "nimcompilerbuildstep.h"
#include "ui_nimcompilerbuildstepconfigwidget.h"
#include "nimproject.h"
#include "nimcompilerbuildstep.h"
#include "nimbuildconfiguration.h"

namespace NimPlugin {

NimCompilerBuildStepConfigWidget::NimCompilerBuildStepConfigWidget(NimCompilerBuildStep *buildStep)
    : ProjectExplorer::BuildStepConfigWidget()
    , m_buildStep(buildStep)
    , m_ui(new Ui::NimCompilerBuildStepConfigWidget())
{
    m_ui->setupUi(this);
    connectBuildConfigurationSignals();
    connectUISignals();
    updateUI();
}

NimCompilerBuildStepConfigWidget::~NimCompilerBuildStepConfigWidget()
{
    delete m_ui;
    m_ui = nullptr;
}

QString NimCompilerBuildStepConfigWidget::summaryText() const
{
    return QStringLiteral("Nim build step");
}

QString NimCompilerBuildStepConfigWidget::displayName() const
{
    return QStringLiteral("Nim build step");
}

void NimCompilerBuildStepConfigWidget::connectUISignals()
{
    connect(m_ui->targetComboBox, SIGNAL(activated(int)), this, SLOT(onTargetChanged(int)));
    connect(m_ui->additionalArgumentsLineEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onAdditionalArgumentsTextEdited(QString)));
}

void NimCompilerBuildStepConfigWidget::updateBuildDirectory()
{
    auto workingDir = m_buildStep->processParameters()->workingDirectory();
    m_ui->buildDirectoryLineEdit->setText(workingDir);
}

void NimCompilerBuildStepConfigWidget::onTargetChanged(int index)
{
    Q_UNUSED(index);

    auto bc = dynamic_cast<NimBuildConfiguration*>(m_buildStep->buildConfiguration());
    Q_ASSERT(bc);

    auto data = m_ui->targetComboBox->currentData();
    Utils::FileName path = Utils::FileName::fromString(data.toString());
    bc->setTargetNimFile(path);
}

void NimCompilerBuildStepConfigWidget::connectBuildConfigurationSignals()
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(m_buildStep->buildConfiguration());
    Q_ASSERT(bc);

    connect(bc, SIGNAL(targetNimFileChanged(Utils::FileName)), this, SLOT(updateUI()));
    connect(bc, SIGNAL(userCompilerOptionsChanged(QStringList)), this, SLOT(updateUI()));
    connect(bc, SIGNAL(buildDirectoryChanged()), this, SLOT(updateUI()));
}

void NimCompilerBuildStepConfigWidget::updateUI()
{
    updateBuildDirectory();
    updateCommandLineText();
    updateTargetComboBox();
    updateAdditionalArgumentsLineEdit();
    updateBuildTypeComboBox();
}

void NimCompilerBuildStepConfigWidget::onAdditionalArgumentsTextEdited(const QString &text)
{
    auto bc = dynamic_cast<NimBuildConfiguration*>(m_buildStep->buildConfiguration());
    Q_ASSERT(bc);
    bc->setUserCompilerOptions(text.split(QChar::Space));
}

void NimCompilerBuildStepConfigWidget::updateCommandLineText()
{
    auto parameters = m_buildStep->processParameters();

    QStringList command;
    command << parameters->command();
    command << parameters->arguments();

    // Remove empty args
    auto predicate = [](const QString& str)->bool{return str.isEmpty();};
    auto it = std::remove_if(command.begin(), command.end(), predicate);
    command.erase(it, command.end());

    m_ui->commandTextEdit->setText(command.join(QChar::LineFeed));
}

void NimCompilerBuildStepConfigWidget::updateTargetComboBox()
{
    using namespace ProjectExplorer;

    auto project = dynamic_cast<NimProject*>(m_buildStep->project());

    // Save current selected file
    QVariant currentFile;
    if (m_ui->targetComboBox->currentIndex() != -1)
        currentFile = m_ui->targetComboBox->currentData();

    // Re enter the files
    m_ui->targetComboBox->clear();
    for (Utils::FileName filename : project->nimFiles())
        m_ui->targetComboBox->addItem(filename.fileName(), filename.toString());

    // Reselect the previous file
    if (currentFile.isValid()) {
        int index = m_ui->targetComboBox->findData(currentFile);
        if (index > 0)
            m_ui->targetComboBox->setCurrentIndex(index);
    }
}

void NimCompilerBuildStepConfigWidget::updateAdditionalArgumentsLineEdit()
{
    using namespace ProjectExplorer;

    auto bc = dynamic_cast<NimBuildConfiguration*>(m_buildStep->buildConfiguration());
    Q_ASSERT(bc);

    m_ui->additionalArgumentsLineEdit->setText(bc->userCompilerOptions().join(QChar::Space));
}

void NimCompilerBuildStepConfigWidget::updateBuildTypeComboBox()
{
    using namespace ProjectExplorer;
    auto bc = dynamic_cast<NimBuildConfiguration*>(m_buildStep->buildConfiguration());
    Q_ASSERT(bc);
    m_ui->buildTypeComboBox->setCurrentIndex(bc->buildType() == BuildConfiguration::Debug ? 0 : 1);
}

}

