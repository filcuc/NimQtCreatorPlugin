#include "nimeditor.h"
#include "nimeditorconstants.h"
#include "nimeditorplugin.h"
#include "tools/nimindenter.h"
#include "tools/nimhighlighter.h"

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/textdocument.h>

#include <utils/qtcassert.h>

using namespace TextEditor;

namespace NimEditor {
namespace Internal {

NimEditorFactory::NimEditorFactory()
{
    setId(Constants::C_NIMEDITOR_ID);
    setDisplayName(tr(Constants::C_EDITOR_DISPLAY_NAME));
    addMimeType(QLatin1String(Constants::C_NIM_MIMETYPE));

    setEditorActionHandlers(TextEditorActionHandler::Format
                       | TextEditorActionHandler::UnCommentSelection
                       | TextEditorActionHandler::UnCollapseAll);

    setDocumentCreator([]() { return new TextDocument(Constants::C_NIMEDITOR_ID); });
    setIndenterCreator([]() { return new NimIndenter; });
    setSyntaxHighlighterCreator([]() { return new NimHighlighter; });
    setCommentStyle(Utils::CommentDefinition::HashStyle);
    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);
}

} // namespace Internal
} // namespace NimEditor
