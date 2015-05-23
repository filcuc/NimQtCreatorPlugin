#pragma once

#include <texteditor/texteditor.h>

namespace NimEditor {

class NimEditorFactory : public TextEditor::TextEditorFactory
{
    Q_OBJECT

public:
    NimEditorFactory();
};

} // namespace NimEditor
