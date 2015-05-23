#pragma once

#include <texteditor/syntaxhighlighter.h>
#include "tools/nimlexer.h"

namespace NimPlugin {

class NimHighlighter Q_DECL_FINAL : public TextEditor::SyntaxHighlighter
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
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    void initTextFormats();

    Category categoryForToken(const NimLexer::Token& token, const QString& tokenValue);
    Category categoryForIdentifier(const NimLexer::Token& token, const QString& tokenValue);

    int highlightLine(const QString &text, int initialState);
};

} // namespace NimEditor
