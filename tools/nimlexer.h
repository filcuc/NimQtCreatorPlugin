#pragma once

#include <QString>

class SourceCodeStream;

namespace NimEditor {

class NimLexer final
{
public:
    enum State {
        Default = -1,
        MultiLineString = 0
    };

    struct TokenType {
        enum Type {
            Keyword = 0,
            Identifier,
            Comment,
            Documentation,
            StringLiteral,
            MultiLineStringLiteral,
            Operator,
            Number,
            EndOfText
        };
    };

    struct Token {
        Token()
            : begin(0)
            , length(0)
            , type(TokenType::EndOfText)
        {}

        Token(int b, int l, TokenType::Type t)
            : begin(b), length(l), type(t)
        {}

        int begin;
        int length;
        TokenType::Type type;
    };

    /// Constructor
    NimLexer(const QChar* text,
             int length,
             State state = State::Default );

    ~NimLexer();

    /// Return the next token or EndOfText
    Token next();

    /// Return the lexer current state
    State state() const { return m_state; }

private:
    Token onDefaultState();
    Token onMultiLineStringState();

    bool isSkipChar();

    bool isOperator();
    Token readOperator();

    bool matchCommentStart();
    Token readComment();

    bool matchDocumentationStart();
    Token readDocumentation();

    bool matchNumber();
    Token readNumber();

    bool matchIdentifierOrKeywordStart();
    Token readIdentifierOrKeyword();

    bool matchStringLiteralStart();
    Token readStringLiteral();

    bool matchMultiLineStringLiteralStart();
    Token readMultiLineStringLiteral(bool moveForward = true);

    const QChar* m_text;
    int m_length;
    State m_state;
    SourceCodeStream* m_stream;
};

} // NimEditor
