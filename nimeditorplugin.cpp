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

/*******************************************************************************
 * List of Python keywords (includes "print" that isn't keyword in python 3
 ******************************************************************************/
static const char *const LIST_OF_PYTHON_KEYWORDS[] = {
    "and",
    "as",
    "assert",
    "break",
    "class",
    "continue",
    "def",
    "del",
    "elif",
    "else",
    "except",
    "exec",
    "finally",
    "for",
    "from",
    "global",
    "if",
    "import",
    "in",
    "is",
    "lambda",
    "not",
    "or",
    "pass",
    "print",
    "raise",
    "return",
    "try",
    "while",
    "with",
    "yield"
};

/*******************************************************************************
 * List of Python magic methods and attributes
 ******************************************************************************/
static const char *const LIST_OF_PYTHON_MAGICS[] = {
    // ctor & dtor
    "__init__",
    "__del__",
    // string conversion functions
    "__str__",
    "__repr__",
    "__unicode__",
    // attribute access functions
    "__setattr__",
    "__getattr__",
    "__delattr__",
    // binary operators
    "__add__",
    "__sub__",
    "__mul__",
    "__truediv__",
    "__floordiv__",
    "__mod__",
    "__pow__",
    "__and__",
    "__or__",
    "__xor__",
    "__eq__",
    "__ne__",
    "__gt__",
    "__lt__",
    "__ge__",
    "__le__",
    "__lshift__",
    "__rshift__",
    "__contains__",
    // unary operators
    "__pos__",
    "__neg__",
    "__inv__",
    "__abs__",
    "__len__",
    // item operators like []
    "__getitem__",
    "__setitem__",
    "__delitem__",
    "__getslice__",
    "__setslice__",
    "__delslice__",
    // other functions
    "__cmp__",
    "__hash__",
    "__nonzero__",
    "__call__",
    "__iter__",
    "__reversed__",
    "__divmod__",
    "__int__",
    "__long__",
    "__float__",
    "__complex__",
    "__hex__",
    "__oct__",
    "__index__",
    "__copy__",
    "__deepcopy__",
    "__sizeof__",
    "__trunc__",
    "__format__",
    // magic attributes
    "__name__",
    "__module__",
    "__dict__",
    "__bases__",
    "__doc__"
};

/*******************************************************************************
 * List of python built-in functions and objects
 ******************************************************************************/
static const char *const LIST_OF_PYTHON_BUILTINS[] = {
    "range",
    "xrange",
    "int",
    "float",
    "long",
    "hex",
    "oct"
    "chr",
    "ord",
    "len",
    "abs",
    "None",
    "True",
    "False"
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
    copyIdentifiers(LIST_OF_PYTHON_KEYWORDS, sizeof(LIST_OF_PYTHON_KEYWORDS), m_keywords);
    copyIdentifiers(LIST_OF_PYTHON_MAGICS, sizeof(LIST_OF_PYTHON_MAGICS), m_magics);
    copyIdentifiers(LIST_OF_PYTHON_BUILTINS, sizeof(LIST_OF_PYTHON_BUILTINS), m_builtins);
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

QSet<QString> NimEditorPlugin::keywords()
{
    return m_instance->m_keywords;
}

QSet<QString> NimEditorPlugin::magics()
{
    return m_instance->m_magics;
}

QSet<QString> NimEditorPlugin::builtins()
{
    return m_instance->m_builtins;
}

} // namespace Internal
} // namespace NimEditor
