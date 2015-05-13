#include "nimeditorplugin.h"
#include "nimeditor.h"
#include "nimeditorconstants.h"
#include "tools/nimhighlighter.h"

#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/id.h>
#include <coreplugin/editormanager/editormanager.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/texteditorconstants.h>
#include <utils/mimetypes/mimedatabase.h>

#include <QtPlugin>
#include <QCoreApplication>

using namespace NimEditor::Constants;

static const char *const LIST_OF_NIM_KEYWORDS[] = {
    "addr", "and", "as", "asm", "atomic",
    "break", "block", "bind",
    "case", "cast", "concept", "const", "continue",
    "defer", "discard", "distinct", "div", "do",
    "elif", "else", "end", "enum", "except", "export",
    "finally", "for", "from", "func",
    "generic",
    "if", "import", "in", "include", "is", "isnot", "iterator",
    "let",
    "macro", "method", "mixin", "mod",
    "nil", "not", "notin",
    "object", "of", "or", "out",
    "proc", "ptr",
    "raise", "ref", "return",
    "shl", "shr", "static",
    "template", "try", "tuple", "type",
    "using",
    "var",
    "when", "while", "with", "without",
    "xor",
    "yield"
};

static const char *const LIST_OF_NIM_MAGICS[] = {};

static const char *const LIST_OF_NIM_BUILTINS[] = {
    "range",
    "int", "cint",
    "float", "cfloat",
    "long", "clong",
    "bool", "cbool",
    "string", "cstring",
    "true", "false",
    "len",
    "low",
    "high",
    "add",
    "pop",
    "ord",
    "echo"
};

namespace NimEditor {
namespace Internal {

static NimEditorPlugin *m_instance = 0;

/// Copies identifiers from array to QSet
static void copyIdentifiers(const char * const words[], size_t bytesCount, QSet<QString> &result)
{
    const size_t count = bytesCount / sizeof(const char * const);
    for (size_t i = 0; i < count; ++i)
        result.insert(QLatin1String(words[i]));
}

NimEditorPlugin::NimEditorPlugin()
{
    m_instance = this;
}

NimEditorPlugin::~NimEditorPlugin()
{
    m_instance = 0;
}

bool NimEditorPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    Utils::MimeDatabase::addMimeTypes(QLatin1String(":/nimeditor/NimEditor.mimetypes.xml"));

    addAutoReleasedObject(new NimEditorFactory);

    // Initialize editor actions handler
    // Add MIME overlay icons (these icons displayed at Project dock panel)
    const QIcon icon = QIcon::fromTheme(QLatin1String(C_NIM_MIME_ICON));
    if (!icon.isNull())
        Core::FileIconProvider::registerIconOverlayForMimeType(icon, C_NIM_MIMETYPE);

    return true;
}

} // namespace Internal
} // namespace NimEditor
