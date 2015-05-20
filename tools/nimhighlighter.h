#pragma once

#include <texteditor/syntaxhighlighter.h>
#include "nimlexer.h"

namespace NimEditor {
namespace Internal {

class NimScanner;

class NimHighlighter : public TextEditor::SyntaxHighlighter
{
    enum Category {
        TextCategory = 0,
        KeywordCategory,
        CommentCategory,
        DocumentationCategory,
        TypeCategory,
        StringCategory,
        NumberCategory,
        OperatorCategory,
        FunctionCategory
    };

public:
    NimHighlighter();

protected:
    void highlightBlock(const QString &text);

private:
    void initTextFormats();

    Category categoryForToken(const NimLexer::Token& token, const QString& tokenValue);
    Category categoryForIdentifier(const NimLexer::Token& token, const QString& tokenValue);

    int highlightLine(const QString &text, int initialState);
};

} // namespace Internal
} // namespace NimEditor
