QT += widgets
CONFIG += c++14
CONFIG += sdk_no_version_check
TEMPLATE = app
TARGET = msdscript_ui
QMAKE_LIBS_OPENGL = -framework OpenGL

INCLUDEPATH += msdscript_core

SOURCES += \
    main.cpp \
    msdWidget.cpp \
    msdscript_core/env.cpp \
    msdscript_core/expr.cpp \
    msdscript_core/parse.cpp \
    msdscript_core/val.cpp

HEADERS += \
    msdWidget.h \
    msdscript_core/env.h \
    msdscript_core/expr.h \
    msdscript_core/parse.h \
    msdscript_core/pointer.h \
    msdscript_core/val.h
