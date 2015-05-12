#pragma once

#include <texteditor/texteditor.h>

namespace NimEditor {
namespace Internal {

class NimEditorFactory : public TextEditor::TextEditorFactory
{
    Q_OBJECT

public:
    NimEditorFactory();
};

} // namespace Internal
} // namespace NimEditor
