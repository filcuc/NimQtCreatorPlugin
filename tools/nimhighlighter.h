#pragma once

#include <texteditor/syntaxhighlighter.h>
#include "nimlexer.h"

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
    void initTextFormats();

    int categoryForType(int type);
    int highlightLine(const QString &text, int initialState);
};

} // namespace Internal
} // namespace NimEditor
