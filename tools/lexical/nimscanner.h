#pragma once

#include "nimformattoken.h"
#include "sourcecodestream.h"

#include <QString>
#include <QSet>

namespace NimEditor {
namespace Internal {

class NimScanner
{
    NimScanner(const NimScanner &other);
    void operator =(const NimScanner &other);

public:
    enum State {
        State_Default,
        State_String,
        State_MultiLineString
    };

    NimScanner(const QChar *text, const int length);
    ~NimScanner();

    void setState(int state);
    int state() const;
    FormatToken read();
    QString value(const FormatToken& tk) const;

    static const QSet<QString>& keywords();
    static const QSet<QString>& magics();
    static const QSet<QString>& builtins();


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
};

} // namespace Internal
} // namespace NimEditor
