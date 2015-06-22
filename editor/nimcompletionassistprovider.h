#pragma once

#include <texteditor/codeassist/completionassistprovider.h>

namespace NimPlugin {

class NimCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT

public:
    NimCompletionAssistProvider();

    bool supportsEditor(Core::Id editorId) const Q_DECL_OVERRIDE;
    TextEditor::IAssistProcessor *createProcessor() const Q_DECL_OVERRIDE;
};

}
