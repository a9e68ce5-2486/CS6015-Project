QT += widgets
CONFIG += c++14
CONFIG += sdk_no_version_check
TEMPLATE = app
TARGET = part1_form
QMAKE_LIBS_OPENGL = -framework OpenGL

SOURCES += \
    main.cpp \
    mainWidget.cpp

HEADERS += \
    mainWidget.h
