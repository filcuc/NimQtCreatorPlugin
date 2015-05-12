#pragma once

#include <texteditor/indenter.h>
#include <QStringList>

namespace NimEditor {

class NimIndenter : public TextEditor::Indenter
{
public:
    NimIndenter();
    virtual ~NimIndenter();

    bool isElectricCharacter(const QChar &ch) const;
    void indentBlock(QTextDocument *document,
                     const QTextBlock &block,
                     const QChar &typedChar,
                     const TextEditor::TabSettings &settings);

protected:
    bool isElectricLine(const QString &line) const;
    int getIndentDiff(const QString &previousLine) const;

private:
    QStringList m_jumpKeywords;
};

} // namespace NimEditor
