#pragma once

#include <texteditor/indenter.h>
#include <QStringList>

namespace NimEditor {

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

    static const QSet<QString>& jumpKeywords();
    static const QSet<QChar>& electricCharacters();

protected:
    static constexpr int tabSize() { return 2; }

    bool isElectricLine(const QString &line) const;
    int getIndentDiff(const QString &previousLine) const;
};

} // namespace NimEditor
