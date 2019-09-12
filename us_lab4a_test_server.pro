
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = us_lab4a_test_server
TEMPLATE = app

CONFIG += c++14 warn_on

SOURCES += \
    src/main.cpp \
    src/LogSyntaxHighlighter.cpp \
    src/ServerThread.cpp \
    src/ServerWindow.cpp \
    src/test/TestDevice.cpp \
    src/util/HDF5Util.cpp \
    src/util/KeyValueFileReader.cpp \
    src/util/Log.cpp \
    src/util/ParameterMap.cpp \
    src/external/lzf/lzf_c.c \
    src/external/lzf/lzf_d.c \
    src/external/lzf/lzf_filter.c

HEADERS += \
    src/ArrayAcqProtocol.h \
    src/ArrayAcqServer.h \
    src/ArrayAcqServerProtocol.h \
    src/LogSyntaxHighlighter.h \
    src/RawBuffer.h \
    src/ServerThread.h \
    src/ServerWindow.h \
    src/test/TestDevice.h \
    src/util/Exception.h \
    src/util/HDF5Util.h \
    src/util/KeyValueFileReader.h \
    src/util/Log.h \
    src/util/Matrix.h \
    src/util/ParameterMap.h \
    src/util/Util.h \
    src/external/lzf/lzf.h \
    src/external/lzf/lzfP.h \
    src/external/lzf/lzf_filter.h

FORMS += \
    ui/ServerWindow.ui

LIBS += -lboost_system \
    -lhdf5_cpp

exists(/usr/include/hdf5/serial) {
    # Debian 9.
    INCLUDEPATH += /usr/include/hdf5/serial
    LIBS += -lhdf5_serial
} else {
    LIBS += -lhdf5
}

INCLUDEPATH += \
    src \
    src/test \
    src/util \
    src/external \
    src/external/lzf

DEPENDPATH += \
    src \
    src/test \
    src/util \
    src/external \
    src/external/lzf

QMAKE_CXXFLAGS_DEBUG = -march=native -O0 -g
QMAKE_CXXFLAGS_RELEASE = -march=native -O3

MOC_DIR = tmp
OBJECTS_DIR = tmp
UI_DIR = tmp
