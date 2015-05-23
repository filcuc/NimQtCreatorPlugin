#include "tools/nimlexer.h"
#include "tools/sourcecodestream.h"

#include <QSet>
#include <QRegularExpression>

namespace
{
    QSet<QChar> asCharSet(const std::initializer_list<char>& chars)
    {
        QSet<QChar> result;
        for (auto c : chars)
            result.insert(QLatin1Char(c));
        return result;
    }

    QSet<QString> asStringSet(const std::initializer_list<const char*> strings)
    {
        QSet<QString> result;
        for (auto s : strings)
            result.insert(QLatin1String(s));
        return result;
    }
}

namespace NimPlugin {

NimLexer::NimLexer(const QChar* text, int length, State state)
    : m_text(text)
    , m_length(length)
    , m_state(state)
    , m_stream(new SourceCodeStream(text, length))
{}

NimLexer::~NimLexer()
{
    delete m_stream;
}

NimLexer::Token NimLexer::next()
{
    switch (m_state)
    {
    case State::MultiLineString:
        return onMultiLineStringState();
    default:
        return onDefaultState();
    }
}

NimLexer::Token NimLexer::onDefaultState()
{
    while (!m_stream->isEnd())
    {
        if (isSkipChar()) {
            m_stream->move();
            continue;
        }
        if (isOperator())
            return readOperator();
        if (matchDocumentationStart())
            return readDocumentation();
        if (matchCommentStart())
            return readComment();
        if (matchNumber())
            return readNumber();
        if (matchMultiLineStringLiteralStart())
            return readMultiLineStringLiteral(true);
        if (matchStringLiteralStart())
            return readStringLiteral();
        if (matchIdentifierOrKeywordStart())
            return readIdentifierOrKeyword();
        m_stream->move();
    }

    return Token {0,0, TokenType::EndOfText };
}

NimLexer::Token NimLexer::onMultiLineStringState()
{
    if (m_stream->isEnd())
        return Token {0,0, TokenType::EndOfText };
    return readMultiLineStringLiteral(false);
}

bool NimLexer::isSkipChar()
{
    static QSet<QChar> skipChars = ::asCharSet({' ', '\t'});
    return skipChars.contains(m_stream->peek());
}

bool NimLexer::isOperator()
{
    static QSet<QChar> operators = ::asCharSet({'+', '-', '*', '/',
                                                '\\', '<', '>', '!',
                                                '?', '^', '.', '|',
                                                '=', '%', '&', '$',
                                                '@', '~', ':' });
    return operators.contains(m_stream->peek());
}

NimLexer::Token NimLexer::readOperator()
{
    m_stream->setAnchor();
    m_stream->move();
    return Token(m_stream->anchor(), m_stream->length(), TokenType::Operator);
}

bool NimLexer::matchCommentStart()
{
    return m_stream->peek() == QLatin1Char('#')
            && m_stream->peek(1) != QLatin1Char('#');
}

NimLexer::Token NimLexer::readComment()
{
    m_stream->setAnchor();
    m_stream->moveToEnd();
    return Token(m_stream->anchor(), m_stream->length(), TokenType::Comment);
}

bool NimLexer::matchDocumentationStart()
{
    return m_stream->peek() == QLatin1Char('#')
            && m_stream->peek(1) == QLatin1Char('#');
}

NimLexer::Token NimLexer::readDocumentation()
{
    m_stream->setAnchor();
    m_stream->moveToEnd();
    return Token(m_stream->anchor(), m_stream->length(), TokenType::Documentation);
}

bool NimLexer::matchNumber()
{
    return m_stream->peek().isNumber();
}

NimLexer::Token NimLexer::readNumber()
{
    m_stream->setAnchor();
    m_stream->move();

    while (!m_stream->isEnd()) {
        if (!m_stream->peek().isNumber())
            break;
        m_stream->move();
    }

    return Token(m_stream->anchor(), m_stream->length(), TokenType::Number);
}

bool NimLexer::matchIdentifierOrKeywordStart()
{
    static QRegularExpression isLetter {QLatin1String("[a-zA-Z\x80-\xFF]")};
    return isLetter.match(m_stream->peek()).hasMatch();
}

NimLexer::Token NimLexer::readIdentifierOrKeyword()
{
    static QRegularExpression isLetter {QLatin1String("[a-zA-Z\x80-\xFF]")};
    static QSet<QString> keywords =
            ::asStringSet({"addr", "and", "as", "asm", "atomic",
                           "bind", "block", "break",
                           "case", "cast", "concept", "const", "continue", "converter",
                           "defer", "discard", "distinct", "div", "do",
                           "elif", "else", "end", "enum", "except", "export",
                           "finally", "for", "from", "func",
                           "generic",
                           "if", "import", "in", "include", "interface", "is", "isnot", "iterator",
                           "let",
                           "macro", "method", "mixin", "mod",
                           "nil", "not", "notin",
                           "object", "of", "or", "out",
                           "proc", "ptr",
                           "raise", "ref", "return",
                           "shl", "shr", "static",
                           "template", "try", "tuple", "type",
                           "using",
                           "var",
                           "when", "while", "with", "without",
                           "xor",
                           "yield"});
    m_stream->setAnchor();
    m_stream->move();

    while (!m_stream->isEnd()) {
        const QChar& c = m_stream->peek();
        if (!(c == QLatin1Char('_')
              || c.isDigit()
              || isLetter.match(c).hasMatch()))
            break;
        m_stream->move();
    }

    QString value = m_stream->value();
    bool isKeyword = keywords.contains(value);

    return Token (m_stream->anchor(),
                  m_stream->length(),
                  isKeyword ? TokenType::Keyword : TokenType::Identifier );
}

bool NimLexer::matchStringLiteralStart()
{
    return m_stream->peek() == QLatin1Char('"');
}

NimLexer::Token NimLexer::readStringLiteral()
{
    m_stream->setAnchor();
    m_stream->move();

    while (!m_stream->isEnd()) {
        if (m_stream->peek() != QLatin1Char('\\')
                && m_stream->peek(1) == QLatin1Char('"')) {
            m_stream->move();
            m_stream->move();
            break;
        }
        m_stream->move();
    }

    return Token (m_stream->anchor(),
                  m_stream->length(),
                  TokenType::StringLiteral);
}

bool NimLexer::matchMultiLineStringLiteralStart()
{
    return m_stream->peek() == QLatin1Char('"')
            && m_stream->peek(1) == QLatin1Char('"')
            && m_stream->peek(2) == QLatin1Char('"');
}

NimLexer::Token NimLexer::readMultiLineStringLiteral(bool moveForward)
{
    m_state = State::MultiLineString;
    m_stream->setAnchor();

    // Move ahead of 3 chars
    if (moveForward) {
        m_stream->move();
        m_stream->move();
        m_stream->move();
    }

    while (!m_stream->isEnd()) {
        if (m_stream->peek() == QLatin1Char('"')
                && m_stream->peek(1) == QLatin1Char('"')
                && m_stream->peek(2) == QLatin1Char('"')) {
            m_stream->move();
            m_stream->move();
            m_stream->move();
            m_state = State::Default;
            break;
        }
        m_stream->move();
    }

    return Token (m_stream->anchor(),
                  m_stream->length(),
                  TokenType::MultiLineStringLiteral);
}

} // NimEditor
