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
    tools/lexical/nimformattoken.h \
    tools/lexical/nimscanner.h

SOURCES += \
    nimeditorplugin.cpp \
    nimeditor.cpp \
    tools/nimhighlighter.cpp \
    tools/nimindenter.cpp \
    tools/lexical/nimscanner.cpp
