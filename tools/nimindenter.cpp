#include "nimindenter.h"
#include "nimlexer.h"

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

void NimIndenter::indentBlock(QTextDocument *document,
                                 const QTextBlock &block,
                                 const QChar &typedChar,
                                 const TextEditor::TabSettings &settings)
{
    Q_UNUSED(document);
    Q_UNUSED(typedChar);

    QTextBlock previousBlock = block.previous();

    if (!previousBlock.isValid()) {
        settings.indentLine(block, 0);
        return;
    }

    // Calculate indentation
    QString previousLine = previousBlock.text();
    NimLexer lexer(previousLine.constData(),
                   previousLine.length(),
                   static_cast<NimLexer::State>(previousBlock.userState()));
    int indentation = settings.indentationColumn(previousLine);
    indentation += calculateIndentDiff(previousLine, lexer);

    // Sets indentation
    settings.indentLine(block, std::max(0, indentation));
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
    static QSet<QChar> result { QLatin1Char(':'), QLatin1Char('=') };
    return result;
}

bool NimIndenter::startsBlock(const QString& line,
                              NimLexer& lexer) const
{
    NimLexer::Token previous;
    NimLexer::Token current = lexer.next();
    while (current.type != NimLexer::TokenType::EndOfText) {
        previous = current;
        current = lexer.next();
    }

    if (previous.type == NimLexer::TokenType::Operator) {
        auto ref = line.midRef(previous.begin, previous.length);
        return ref == QStringLiteral(":") || ref == QStringLiteral("=");
    }

    if (previous.type == NimLexer::TokenType::Keyword) {
        auto ref = line.midRef(previous.begin, previous.length);
        return ref == QLatin1String("type")
                || ref == QLatin1String("var")
                || ref == QLatin1String("let")
                || ref == QLatin1String("enum")
                || ref == QLatin1String("object");
    }

    return false;
}

bool NimIndenter::endsBlock(const QString& line,
                            NimLexer& lexer) const
{
    NimLexer::Token previous;
    NimLexer::Token current = lexer.next();
    while (current.type != NimLexer::TokenType::EndOfText) {
        previous = current;
        current = lexer.next();
    }

    if (previous.type == NimLexer::TokenType::Keyword) {
        auto ref = line.midRef(previous.begin, previous.length);
        return ref == QLatin1String("return")
               || ref == QLatin1String("break")
               || ref == QLatin1String("continue");
    }

    return false;
}

int NimIndenter::calculateIndentDiff(const QString &previousLine,
                                     NimLexer& lexer) const
{
    if (previousLine.isEmpty())
        return 0;

    if (startsBlock(previousLine, lexer))
        return NimIndenter::tabSize();

    if (endsBlock(previousLine, lexer))
        return -NimIndenter::tabSize();

    return 0;
}

} // namespace NimEditor
