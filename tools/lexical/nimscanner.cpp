#include "nimscanner.h"
#include "../../nimeditorconstants.h"
#include "../../nimeditorplugin.h"

#include <QString>
#include <QSet>

namespace NimEditor {
namespace Internal {

NimScanner::NimScanner(const QChar *text, const int length)
    : m_src(text, length)
    , m_state(0)
{
}

NimScanner::~NimScanner()
{
}

void NimScanner::setState(int state)
{
    m_state = state;
}

int NimScanner::state() const
{
    return m_state;
}

FormatToken NimScanner::read()
{
    m_src.setAnchor();
    if (m_src.isEnd())
        return FormatToken(Format_EndOfBlock, m_src.anchor(), 0);

    State state;
    QChar saved;
    parseState(state, saved);
    switch (state) {
    case State_String:
        return readStringLiteral(saved);
    case State_MultiLineString:
        return readMultiLineStringLiteral(saved);
    default:
        return onDefaultState();
    }
}

QString NimScanner::value(const FormatToken &tk) const
{
    return m_src.value(tk.begin(), tk.length());
}

const QSet<QString>&NimScanner::keywords()
{
    static QSet<QString> result {
        "addr", "and", "as", "asm", "atomic",
        "break", "block", "bind",
        "case", "cast", "concept", "const", "continue",
        "defer", "discard", "distinct", "div", "do",
        "elif", "else", "end", "enum", "except", "export",
        "finally", "for", "from", "func",
        "generic",
        "if", "import", "in", "include", "is", "isnot", "iterator",
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
        "yield"
    };
    return result;
}

const QSet<QString>&NimScanner::magics()
{
    static QSet<QString> result {};
    return result;
}

const QSet<QString>&NimScanner::builtins()
{
    static QSet<QString> result {
        "int", "cint",
        "float", "cfloat",
        "long", "clong",
        "bool", "cbool",
        "string", "cstring",
        "range",
        "true", "false", "len",
        "low", "high", "add",
        "pop", "ord", "echo"
    };
    return result;
}

FormatToken NimScanner::onDefaultState()
{
    QChar first = m_src.peek();
    m_src.move();

    if (first == QLatin1Char('\\') && m_src.peek() == QLatin1Char('\n')) {
        m_src.move();
        return FormatToken(Format_Whitespace, m_src.anchor(), 2);
    }

    if (first == QLatin1Char('.') && m_src.peek().isDigit())
        return readFloatNumber();

    if (first == QLatin1Char('\'') || first == QLatin1Char('\"'))
        return readStringLiteral(first);

    if (first.isLetter() || (first == QLatin1Char('_')))
        return readIdentifier();

    if (first.isDigit())
        return readNumber();

    if (first == QLatin1Char('#')) {
        if (m_src.peek() == QLatin1Char('#'))
            return readDoxygenComment();
        return readComment();
    }

    if (first.isSpace())
        return readWhiteSpace();

    return readOperator();
}

/**
 * @brief Lexer::passEscapeCharacter
 * @return returns true if escape sequence doesn't end with newline
 */
void NimScanner::checkEscapeSequence(QChar quoteChar)
{
    if (m_src.peek() == QLatin1Char('\\')) {
        m_src.move();
        QChar ch = m_src.peek();
        if (ch == QLatin1Char('\n') || ch.isNull())
            saveState(State_String, quoteChar);
    }
}

/**
  reads single-line string literal, surrounded by ' or " quotes
  */
FormatToken NimScanner::readStringLiteral(QChar quoteChar)
{
    QChar ch = m_src.peek();
    if (ch == quoteChar && m_src.peek(1) == quoteChar) {
        saveState(State_MultiLineString, quoteChar);
        return readMultiLineStringLiteral(quoteChar);
    }

    while (ch != quoteChar && !ch.isNull()) {
        checkEscapeSequence(quoteChar);
        m_src.move();
        ch = m_src.peek();
    }
    if (ch == quoteChar)
        clearState();
    m_src.move();
    return FormatToken(Format_String, m_src.anchor(), m_src.length());
}

/**
  reads multi-line string literal, surrounded by ''' or """ sequencies
  */
FormatToken NimScanner::readMultiLineStringLiteral(QChar quoteChar)
{
    for (;;) {
        QChar ch = m_src.peek();
        if (ch.isNull())
            break;
        if (ch == quoteChar
                && (m_src.peek(1) == quoteChar)
                && (m_src.peek(2) == quoteChar)) {
            clearState();
            m_src.move();
            m_src.move();
            m_src.move();
            break;
        }
        m_src.move();
    }

    return FormatToken(Format_String, m_src.anchor(), m_src.length());
}

/**
  reads identifier and classifies it
  */
FormatToken NimScanner::readIdentifier()
{
    QChar ch = m_src.peek();
    while (ch.isLetterOrNumber() || (ch == QLatin1Char('_'))) {
        m_src.move();
        ch = m_src.peek();
    }
    QString value = m_src.value();

    Format tkFormat = Format_Identifier;
    if (value == QLatin1String("self"))
        tkFormat = Format_ClassField;
    else if (NimScanner::builtins().contains(value))
        tkFormat = Format_Type;
    else if (NimScanner::magics().contains(value))
        tkFormat = Format_MagicAttr;
    else if (NimScanner::keywords().contains(value))
        tkFormat = Format_Keyword;

    return FormatToken(tkFormat, m_src.anchor(), m_src.length());
}

inline static bool isHexDigit(QChar ch)
{
    return (ch.isDigit()
            || (ch >= QLatin1Char('a') && ch <= QLatin1Char('f'))
            || (ch >= QLatin1Char('A') && ch <= QLatin1Char('F')));
}

inline static bool isOctalDigit(QChar ch)
{
    return (ch.isDigit() && ch != QLatin1Char('8') && ch != QLatin1Char('9'));
}

inline static bool isBinaryDigit(QChar ch)
{
    return (ch == QLatin1Char('0') || ch == QLatin1Char('1'));
}

inline static bool isValidIntegerSuffix(QChar ch)
{
    return (ch == QLatin1Char('l') || ch == QLatin1Char('L'));
}

FormatToken NimScanner::readNumber()
{
    if (!m_src.isEnd()) {
        QChar ch = m_src.peek();
        if (ch.toLower() == QLatin1Char('b')) {
            m_src.move();
            while (isBinaryDigit(m_src.peek()))
                m_src.move();
        } else if (ch.toLower() == QLatin1Char('o')) {
            m_src.move();
            while (isOctalDigit(m_src.peek()))
                m_src.move();
        } else if (ch.toLower() == QLatin1Char('x')) {
            m_src.move();
            while (isHexDigit(m_src.peek()))
                m_src.move();
        } else { // either integer or float number
            return readFloatNumber();
        }
        if (isValidIntegerSuffix(m_src.peek()))
            m_src.move();
    }
    return FormatToken(Format_Number, m_src.anchor(), m_src.length());
}

FormatToken NimScanner::readFloatNumber()
{
    enum
    {
        State_INTEGER,
        State_FRACTION,
        State_EXPONENT
    } state;
    state = (m_src.peek(-1) == QLatin1Char('.')) ? State_FRACTION : State_INTEGER;

    for (;;) {
        QChar ch = m_src.peek();
        if (ch.isNull())
            break;

        if (state == State_INTEGER) {
            if (ch == QLatin1Char('.'))
                state = State_FRACTION;
            else if (!ch.isDigit())
                break;
        } else if (state == State_FRACTION) {
            if (ch == QLatin1Char('e') || ch == QLatin1Char('E')) {
                QChar next = m_src.peek(1);
                QChar next2 = m_src.peek(2);
                bool isExp = next.isDigit()
                        || (((next == QLatin1Char('-')) || (next == QLatin1Char('+'))) && next2.isDigit());
                if (isExp) {
                    m_src.move();
                    state = State_EXPONENT;
                } else {
                    break;
                }
            } else if (!ch.isDigit()) {
                break;
            }
        } else if (!ch.isDigit()) {
            break;
        }
        m_src.move();
    }

    QChar ch = m_src.peek();
    if ((state == State_INTEGER && (ch == QLatin1Char('l') || ch == QLatin1Char('L')))
            || (ch == QLatin1Char('j') || ch == QLatin1Char('J')))
        m_src.move();

    return FormatToken(Format_Number, m_src.anchor(), m_src.length());
}

/**
  reads single-line python comment, started with "#"
  */
FormatToken NimScanner::readComment()
{
    QChar ch = m_src.peek();
    while (ch != QLatin1Char('\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return FormatToken(Format_Comment, m_src.anchor(), m_src.length());
}

/**
  reads single-line python doxygen comment, started with "##"
  */
FormatToken NimScanner::readDoxygenComment()
{
    QChar ch = m_src.peek();
    while (ch != QLatin1Char('\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return FormatToken(Format_Doxygen, m_src.anchor(), m_src.length());
}

/**
  reads whitespace
  */
FormatToken NimScanner::readWhiteSpace()
{
    while (m_src.peek().isSpace())
        m_src.move();
    return FormatToken(Format_Whitespace, m_src.anchor(), m_src.length());
}

/**
  reads punctuation symbols, excluding some special
  */
FormatToken NimScanner::readOperator()
{
    const QString EXCLUDED_CHARS = QLatin1String("\'\"_#");
    QChar ch = m_src.peek();
    while (ch.isPunct() && !EXCLUDED_CHARS.contains(ch)) {
        m_src.move();
        ch = m_src.peek();
    }
    return FormatToken(Format_Operator, m_src.anchor(), m_src.length());
}

void NimScanner::clearState()
{
    m_state = 0;
}

void NimScanner::saveState(State state, QChar savedData)
{
    m_state = (state << 16) | static_cast<int>(savedData.unicode());
}

void NimScanner::parseState(State &state, QChar &savedData) const
{
    state = static_cast<State>(m_state >> 16);
    savedData = static_cast<ushort>(m_state);
}

} // namespace Internal
} // namespace NimEditor
