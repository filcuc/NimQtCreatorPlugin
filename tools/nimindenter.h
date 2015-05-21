#pragma once

#include <texteditor/indenter.h>
#include <QStringList>

namespace NimEditor {

class NimLexer;

class NimIndenter : public TextEditor::Indenter
{
public:
    NimIndenter();

    virtual ~NimIndenter();

    bool isElectricCharacter(const QChar &ch) const override;

    void indentBlock(QTextDocument *document,
                     const QTextBlock &block,
                     const QChar &typedChar,
                     const TextEditor::TabSettings &settings) override;

private:
    static const QSet<QString>& jumpKeywords();
    static const QSet<QChar>& electricCharacters();
    static constexpr int tabSize() { return 2; }

    bool startsBlock(const QString &line, NimLexer& lexer) const;
    bool endsBlock(const QString& line, NimLexer& lexer) const;

    int calculateIndentDiff(const QString& previousLine,
                            NimLexer& lexer) const;
};

} // namespace NimEditor
