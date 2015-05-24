QTC_SOURCE=$$(QTC_SOURCE)
QTC_BUILD=$$(QTC_BUILD)
isEmpty(QTC_SOURCE):error(QTC_SOURCE must be set)
isEmpty(QTC_BUILD):error(QTC_BUILD must be set)
IDE_BUILD_TREE=$$QTC_BUILD

message("QTC_SOURCE is $(QTC_SOURCE)")
message("QTC_BUILD is $(QTC_BUILD)")

QTC_PLUGIN_NAME = NimPlugin
QTC_PLUGIN_DEPENDS = coreplugin texteditor projectexplorer
include($$QTC_SOURCE/src/qtcreatorplugin.pri)

CONFIG += debug

DEFINES += \
    NIMPLUGIN_LIBRARY

RESOURCES += \
    nimplugin.qrc

INCLUDEPATH += $$PWD

HEADERS += \
    nimplugin.h \
    nimpluginconstants.h \
    editor/nimeditor.h \
    editor/nimhighlighter.h \
    editor/nimindenter.h \
    tools/nimlexer.h \
    tools/sourcecodestream.h \
    project/nimproject.h \
    project/nimprojectmanager.h \
    project/nimprojectnode.h \
    project/nimprojectwizard.h

SOURCES += \
    nimplugin.cpp \
    editor/nimeditor.cpp \
    editor/nimhighlighter.cpp \
    editor/nimindenter.cpp \
    tools/nimlexer.cpp \
    project/nimproject.cpp \
    project/nimprojectmanager.cpp \
    project/nimprojectwizard.cpp \
    project/nimprojectnode.cpp

OTHER_FILES += README.md
