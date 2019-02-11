
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = us_lab4a_test_server
TEMPLATE = app

CONFIG += c++14 warn_on

SOURCES += \
    src/ServerThread.cpp \
    src/main.cpp \
    src/LogSyntaxHighlighter.cpp \
    src/ServerWindow.cpp \
    src/test/TestDevice.cpp \
    src/util/Log.cpp \
    src/util/KeyValueFileReader.cpp \
    src/util/ParameterMap.cpp \
    src/util/HDF5Util.cpp

HEADERS += \
    src/ServerThread.h \
    src/RawBuffer.h \
    src/LogSyntaxHighlighter.h \
    src/ArrayAcqServerProtocol.h \
    src/ArrayAcqServer.h \
    src/ArrayAcqProtocol.h \
    src/ServerWindow.h \
    src/test/TestDevice.h \
    src/util/Log.h \
    src/util/Exception.h \
    src/util/Util.h \
    src/util/ParameterMap.h \
    src/util/KeyValueFileReader.h \
    src/util/HDF5Util.h \
    src/util/Matrix.h

FORMS += \
    ui/ServerWindow.ui

INCLUDEPATH += src src/test src/util

LIBS += -lboost_system \
    -lhdf5_cpp

exists(/usr/include/hdf5/serial) {
    # Debian 9.
    INCLUDEPATH += /usr/include/hdf5/serial
    LIBS += -lhdf5_serial
} else {
    LIBS += -lhdf5
}

DEPENDPATH += src src/test src/util

QMAKE_CXXFLAGS_DEBUG = -march=native -O0 -g
QMAKE_CXXFLAGS_RELEASE = -march=native -O3

MOC_DIR = tmp
OBJECTS_DIR = tmp
UI_DIR = tmp
