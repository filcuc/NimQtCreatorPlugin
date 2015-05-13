#include "nimindenter.h"
#include "lexical/nimscanner.h"

#include <texteditor/tabsettings.h>

#include <QSet>

namespace NimEditor {

NimIndenter::NimIndenter()
{}

NimIndenter::~NimIndenter()
{}

bool NimIndenter::isElectricCharacter(const QChar& ch) const
{
    return NimIndenter::electricCharacters().contains(ch);
}

/**
 * @brief Indents one block (i.e. one line) of code
 * @param doc Unused
 * @param block Block that represents line
 * @param typedChar Unused
 * @param tabSettings An IDE tabulation settings
 *
 * Usually this function called once when you begin new line of code by pressing
 * Enter. If Indenter reimplements indent() function, than indentBlock() may be
 * called in other cases.
 */
void NimIndenter::indentBlock(QTextDocument *document,
                                 const QTextBlock &block,
                                 const QChar &typedChar,
                                 const TextEditor::TabSettings &settings)
{
    Q_UNUSED(document);
    Q_UNUSED(typedChar);
    QTextBlock previousBlock = block.previous();
    if (previousBlock.isValid()) {
        QString previousLine = previousBlock.text();
        int indentation = settings.indentationColumn(previousLine);

        if (isElectricLine(previousLine))
            indentation += NimIndenter::tabSize();
        else
            indentation = qMax<int>(0, indentation + getIndentDiff(previousLine));

        settings.indentLine(block, indentation);
    } else {
        // First line in whole document
        settings.indentLine(block, 0);
    }
}

const QSet<QString>& NimIndenter::jumpKeywords()
{
    static QSet<QString> result {
        QLatin1String("return"),
        QLatin1String("yield"),
        QLatin1String("break"),
        QLatin1String("continue"),
        QLatin1String("raise"),
        QLatin1String("pass")
    };
    return result;
}

const QSet<QChar>&NimIndenter::electricCharacters()
{
    static QSet<QChar> result { ':', '=' };
    return result;
}

/// @return True if electric character is last non-space character at given string
bool NimIndenter::isElectricLine(const QString &line) const
{
    if (line.isEmpty())
        return false;

    // trim spaces in 'if True:  '
    int index = line.length() - 1;
    while ((index > 0) && line[index].isSpace())
        --index;

    return isElectricCharacter(line[index]);
}

/// @return negative indent diff if previous line breaks control flow branch
int NimIndenter::getIndentDiff(const QString &previousLine) const
{
    Internal::NimScanner sc(previousLine.constData(), previousLine.length());
    forever {
        Internal::FormatToken tk = sc.read();
        if ((tk.format() == Internal::Format_Keyword) && NimIndenter::jumpKeywords().contains(sc.value(tk)))
            return -NimIndenter::tabSize();
        if (tk.format() != Internal::Format_Whitespace)
            break;
    }
    return 0;
}

} // namespace NimEditor
