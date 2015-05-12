#pragma once

#include "nimformattoken.h"
#include "sourcecodestream.h"

#include <QString>
#include <QSet>

namespace NimEditor {
namespace Internal {

/**
 * @brief The Scanner class - scans source code for highlighting only
 */
class Scanner
{
    Scanner(const Scanner &other);
    void operator =(const Scanner &other);

public:
    enum State {
        State_Default,
        State_String,
        State_MultiLineString
    };

    Scanner(const QChar *text, const int length);
    ~Scanner();

    void setState(int state);
    int state() const;
    FormatToken read();
    QString value(const FormatToken& tk) const;

private:
    FormatToken onDefaultState();

    void checkEscapeSequence(QChar quoteChar);
    FormatToken readStringLiteral(QChar quoteChar);
    FormatToken readMultiLineStringLiteral(QChar quoteChar);
    FormatToken readIdentifier();
    FormatToken readNumber();
    FormatToken readFloatNumber();
    FormatToken readComment();
    FormatToken readDoxygenComment();
    FormatToken readWhiteSpace();
    FormatToken readOperator();

    void clearState();
    void saveState(State state, QChar savedData);
    void parseState(State &state, QChar &savedData) const;

    SourceCodeStream m_src;
    int m_state;
    const QSet<QString> m_keywords;
    const QSet<QString> m_magics;
    const QSet<QString> m_builtins;
};

} // namespace Internal
} // namespace NimEditor
