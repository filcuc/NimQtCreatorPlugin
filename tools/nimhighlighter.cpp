#include "nimhighlighter.h"
#include "nimlexer.h"

#include <texteditor/textdocument.h>
#include <texteditor/texteditorconstants.h>

namespace NimEditor {
    namespace Internal {

        NimHighlighter::NimHighlighter() {
            initTextFormats();
        }

        void NimHighlighter::highlightBlock(const QString &text)
        {
            setCurrentBlockState(highlightLine(text, previousBlockState()));
        }

        void NimHighlighter::initTextFormats()
        {
            // We use a map because it's ordered
            static QMap<NimLexer::TokenType::Type, TextEditor::TextStyle> categories {
                {NimLexer::TokenType::Keyword, TextEditor::C_KEYWORD},
                {NimLexer::TokenType::Identifier, TextEditor::C_TEXT},
                {NimLexer::TokenType::Comment, TextEditor::C_COMMENT},
                {NimLexer::TokenType::Documentation, TextEditor::C_DOXYGEN_COMMENT},
                {NimLexer::TokenType::StringLiteral, TextEditor::C_STRING},
                {NimLexer::TokenType::MultiLineStringLiteral, TextEditor::C_STRING},
                {NimLexer::TokenType::Operator, TextEditor::C_OPERATOR}
            };

            QVector<TextEditor::TextStyle> formats;
            for (auto category : categories.keys())
                formats << categories[category];
            setTextFormatCategories(formats);
        }

        int NimHighlighter::highlightLine(const QString &text, int initialState)
        {
            NimLexer lexer(text.constData(),
                           text.size(),
                           static_cast<NimLexer::State>(initialState));

            NimLexer::Token tk;
            while ((tk = lexer.next()).type != NimLexer::TokenType::EndOfText) {
                setFormat(tk.begin, tk.length, formatForCategory(tk.type));
            }

            return lexer.state();
        }

    } // namespace Internal
} // namespace NimEditor
