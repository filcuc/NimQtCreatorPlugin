#pragma once

#include <texteditor/texteditor.h>

namespace NimPlugin {

class NimEditorFactory Q_DECL_FINAL : public TextEditor::TextEditorFactory
{
    Q_OBJECT

public:
    NimEditorFactory();
};

} // namespace NimEditor
