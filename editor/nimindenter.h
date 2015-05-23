#pragma once

#include <texteditor/indenter.h>

namespace NimPlugin {

class NimLexer;

/// This class indent handle the indentation in the editor
class NimIndenter Q_DECL_FINAL : public TextEditor::Indenter
{
public:
    /// Constructor
    NimIndenter();

    virtual ~NimIndenter();

    bool isElectricCharacter(const QChar &ch) const Q_DECL_OVERRIDE;

    void indentBlock(QTextDocument *document,
                     const QTextBlock &block,
                     const QChar &typedChar,
                     const TextEditor::TabSettings &settings) Q_DECL_OVERRIDE;

private:
    static const QSet<QString>& jumpKeywords();
    static const QSet<QChar>& electricCharacters();
    static constexpr int tabSize() { return 2; }

    bool startsBlock(const QString &line, int state) const;
    bool endsBlock(const QString& line, int state) const;

    int calculateIndentDiff(const QString& previousLine, int state) const;
};

} // namespace NimEditor
