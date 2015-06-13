#include "nimpluginconstants.h"
#include "nimplugin.h"
#include "editor/nimeditor.h"
#include "editor/nimindenter.h"
#include "editor/nimhighlighter.h"

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/textdocument.h>

#include <utils/qtcassert.h>

using namespace TextEditor;

namespace NimPlugin {

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
    setMarksVisible(false);
    setCodeFoldingSupported(true);
    setMarksVisible(true);
}

} // namespace NimEditor
