# -------------------------------------------------
# Project created by QtCreator 2010-02-18T10:08:03
# -------------------------------------------------
TEMPLATE = app
TARGET = MegaServer
QT += core \
    xml \
    network
HEADERS += src/table.h \
    src/saxhandler.h \
    src/record.h \
    src/data.h \
    src/megaserver.h \
    src/megathread.h \
    src/megasocket.h \
    src/megaprotocol.h \
    src/util.h
SOURCES += src/table.cpp \
    src/saxhandler.cpp \
    src/record.cpp \
    src/main.cpp \
    src/data.cpp \
    src/megaserver.cpp \
    src/megathread.cpp \
    src/megasocket.cpp \
    src/util.cpp
