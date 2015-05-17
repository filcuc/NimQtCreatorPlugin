include(../../qtcreatorplugin.pri)

DEFINES += \
    NIMEDITOR_LIBRARY

RESOURCES += \
    nimeditorplugin.qrc

HEADERS += \
    nimeditorplugin.h \
    nimeditor.h \
    nimeditorconstants.h \
    tools/lexical/sourcecodestream.h \
    tools/nimhighlighter.h \
    tools/nimindenter.h \
    tools/nimlexer.h

SOURCES += \
    nimeditorplugin.cpp \
    nimeditor.cpp \
    tools/nimhighlighter.cpp \
    tools/nimindenter.cpp \
    tools/nimlexer.cpp
