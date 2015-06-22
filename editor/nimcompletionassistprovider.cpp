#include "nimcompletionassistprovider.h"
#include "nimpluginconstants.h"

#include <coreplugin/id.h>
#include <texteditor/codeassist/keywordscompletionassist.h>

namespace NimPlugin {

NimCompletionAssistProvider::NimCompletionAssistProvider()
{

}

bool NimCompletionAssistProvider::supportsEditor(Core::Id editorId) const
{
    return editorId == Core::Id(Constants::C_NIMEDITOR_ID);
}

TextEditor::IAssistProcessor *NimCompletionAssistProvider::createProcessor() const
{
    using namespace TextEditor;

    Keywords keywords ({QStringLiteral("nimVar")}, {QStringLiteral("nimProc")}, QMap<QString, QStringList>());

    return new KeywordsCompletionAssistProcessor(keywords);
}

}

