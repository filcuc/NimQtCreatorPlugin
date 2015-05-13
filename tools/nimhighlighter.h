#pragma once

#include <texteditor/syntaxhighlighter.h>

namespace NimEditor {
namespace Internal {

class NimScanner;

class NimHighlighter : public TextEditor::SyntaxHighlighter
{
public:
    NimHighlighter();

protected:
    void highlightBlock(const QString &text);

private:
    int highlightLine(const QString &text, int initialState);
    void highlightImport(Internal::NimScanner &scanner);
};

} // namespace Internal
} // namespace NimEditor
