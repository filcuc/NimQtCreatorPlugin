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
            static QMap<Category, TextEditor::TextStyle> categoryStyle = {
                {TextCategory, TextEditor::C_TEXT},
                {KeywordCategory, TextEditor::C_KEYWORD},
                {CommentCategory, TextEditor::C_COMMENT},
                {DocumentationCategory, TextEditor::C_DOXYGEN_COMMENT},
                {TypeCategory, TextEditor::C_TYPE},
                {StringCategory, TextEditor::C_STRING},
                {NumberCategory, TextEditor::C_NUMBER},
                {OperatorCategory, TextEditor::C_OPERATOR},
                {FunctionCategory, TextEditor::C_FUNCTION},
            };

            QVector<TextEditor::TextStyle> formats;
            for (const auto& category : categoryStyle.keys())
                formats << categoryStyle[category];
            setTextFormatCategories(formats);
        }

        NimHighlighter::Category NimHighlighter::categoryForToken(const NimLexer::Token& token,
                                                                  const QString& tokenValue)
        {
            switch (token.type)
            {
            case NimLexer::TokenType::Keyword: return KeywordCategory;
            case NimLexer::TokenType::Identifier: return categoryForIdentifier(token, tokenValue);
            case NimLexer::TokenType::Comment: return CommentCategory;
            case NimLexer::TokenType::Documentation: return DocumentationCategory;
            case NimLexer::TokenType::StringLiteral: return StringCategory;
            case NimLexer::TokenType::MultiLineStringLiteral: return StringCategory;
            case NimLexer::TokenType::Operator: return OperatorCategory;
            default: return TextCategory;
            }
        }

        NimHighlighter::Category NimHighlighter::categoryForIdentifier(const NimLexer::Token& token,
                                                                       const QString& tokenValue)
        {
            Q_ASSERT(token.type == NimLexer::TokenType::Identifier);

            static QSet<QString> nimBuiltInValues {
                QStringLiteral("true"),
                QStringLiteral("false")
            };

            static QSet<QString> nimBuiltInFunctions {
                QStringLiteral("echo"),
                QStringLiteral("isMainModule"),
            };

            static QSet<QString> nimBuiltInTypes {
                QStringLiteral("bool"),
                QStringLiteral("cbool"),
                QStringLiteral("string"),
                QStringLiteral("cstring"),
                QStringLiteral("int"),
                QStringLiteral("cint"),
                QStringLiteral("long"),
                QStringLiteral("clong"),
                QStringLiteral("double"),
                QStringLiteral("cdouble"),
                QStringLiteral("table"),
                QStringLiteral("RootObj")
            };

            if (nimBuiltInFunctions.contains(tokenValue))
                return TypeCategory;
            else if (nimBuiltInValues.contains(tokenValue))
                return KeywordCategory;
            else if (nimBuiltInTypes.contains(tokenValue))
                return TypeCategory;
            else
                return TextCategory;
        }

        int NimHighlighter::highlightLine(const QString &text, int initialState)
        {
            NimLexer lexer(text.constData(),
                           text.size(),
                           static_cast<NimLexer::State>(initialState));

            NimLexer::Token tk;
            while ((tk = lexer.next()).type != NimLexer::TokenType::EndOfText) {
                int category = categoryForToken(tk, text.mid(tk.begin, tk.length));
                setFormat(tk.begin, tk.length, formatForCategory(category));
            }

            return lexer.state();
        }
    } // namespace Internal
} // namespace NimEditor
