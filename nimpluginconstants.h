#pragma once

#include <QtGlobal>

namespace NimPlugin {
namespace Constants {

const char C_NIMEDITOR_ID[] = "NimPlugin.NimEditor";
const char C_EDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "Nim Editor");

const char C_NIM_ICON_PATH[] = ":/nim.png";

/*******************************************************************************
 * MIME type
 ******************************************************************************/
const char C_NIM_MIMETYPE[] = "text/x-nim";
const char C_NIM_MIME_ICON[] = "text-x-nim";
const char C_NIM_PROJECT_MIMETYPE[] = "text/x-nim-project";

} // namespace Constants
} // namespace NimEditor
