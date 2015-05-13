/**
 * @brief The Highlighter class pre-highlights Python source using simple scanner.
 *
 * Highlighter doesn't highlight user types (classes and enumerations), syntax
 * and semantic errors, unnecessary code, etc. It's implements only
 * basic highlight mechanism.
 *
 * Main highlight procedure is highlightBlock().
 */

#include "nimhighlighter.h"
#include "lexical/nimscanner.h"

#include <texteditor/textdocument.h>
#include <texteditor/texteditorconstants.h>

namespace NimEditor {
namespace Internal {

/**
 * @class PythonEditor::Internal::PythonHighlighter
 * @brief Handles incremental lexical highlighting, but not semantic
 *
 * Incremental lexical highlighting works every time when any character typed
 * or some text inserted (i.e. copied & pasted).
 * Each line keeps associated scanner state - integer number. This state is the
 * scanner context for next line. For example, 3 quotes begin a multiline
 * string, and each line up to next 3 quotes has state 'MultiLineString'.
 *
 * @code
 *  def __init__:               # Normal
 *      self.__doc__ = """      # MultiLineString (next line is inside)
 *                     banana   # MultiLineString
 *                     """      # Normal
 * @endcode
 */

NimHighlighter::NimHighlighter()
{
    static QVector<TextEditor::TextStyle> categories;
    if (categories.isEmpty()) {
        categories << TextEditor::C_NUMBER
                   << TextEditor::C_STRING
                   << TextEditor::C_KEYWORD
                   << TextEditor::C_TYPE
                   << TextEditor::C_FIELD
                   << TextEditor::C_JS_SCOPE_VAR
                   << TextEditor::C_OPERATOR
                   << TextEditor::C_COMMENT
                   << TextEditor::C_DOXYGEN_COMMENT
                   << TextEditor::C_TEXT
                   << TextEditor::C_VISUAL_WHITESPACE
                   << TextEditor::C_STRING;
    }
    setTextFormatCategories(categories);
}

/**
 * @brief NimHighlighter::highlightBlock highlights single line of Python code
 * @param text is single line without EOLN symbol. Access to all block data
 * can be obtained through inherited currentBlock() function.
 *
 * This function receives state (int number) from previously highlighted block,
 * scans block using received state and sets initial highlighting for current
 * block. At the end, it saves internal state in current block.
 */
void NimHighlighter::highlightBlock(const QString &text)
{
    int initialState = previousBlockState();
    if (initialState == -1)
        initialState = 0;
    setCurrentBlockState(highlightLine(text, initialState));
}

/**
 * @return True if this keyword is acceptable at start of import line
 */
static bool isImportKeyword(const QString &keyword)
{
    return keyword == QLatin1String("import") || keyword == QLatin1String("from");
}

/**
 * @brief Highlight line of code, returns new block state
 * @param text Source code to highlight
 * @param initialState Initial state of scanner, retrieved from previous block
 * @return Final state of scanner, should be saved with current block
 */
int NimHighlighter::highlightLine(const QString &text, int initialState)
{
    NimScanner scanner(text.constData(), text.size());
    scanner.setState(initialState);

    FormatToken tk;
    bool hasOnlyWhitespace = true;
    while ((tk = scanner.read()).format() != Format_EndOfBlock) {
        Format format = tk.format();
        if (format == Format_Keyword) {
            QString value = scanner.value(tk);
            if (isImportKeyword(value) && hasOnlyWhitespace) {
                setFormat(tk.begin(), tk.length(), formatForCategory(format));
                highlightImport(scanner);
                break;
            }
        }

        setFormat(tk.begin(), tk.length(), formatForCategory(format));
        if (format != Format_Whitespace)
            hasOnlyWhitespace = false;
    }
    return scanner.state();
}

/**
 * @brief Highlights rest of line as import directive
 */
void NimHighlighter::highlightImport(NimScanner &scanner)
{
    FormatToken tk;
    while ((tk = scanner.read()).format() != Format_EndOfBlock) {
        Format format = tk.format();
        if (tk.format() == Format_Identifier)
            format = Format_ImportedModule;
        setFormat(tk.begin(), tk.length(), formatForCategory(format));
    }
}

} // namespace Internal
} // namespace NimEditor
