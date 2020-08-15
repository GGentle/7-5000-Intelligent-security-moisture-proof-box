# -------------------------------------------------
# Project created by QtCreator 2015-09-15T03:54:43
# -------------------------------------------------
TARGET = vedio
TEMPLATE = app
SOURCES += main.cpp \
    vedio.cpp \
    qextserialbase.cpp \
    posix_qextserialport.cpp \
    finger.cpp \
    CXvidEnc.cpp
HEADERS += vedio.h \
    vedio.h \
    linux.h \
    qextserialbase.h \
    posix_qextserialport.h \
    finger.h \
    CXvidEnc.h
FORMS += vedio.ui
QT += network

LIBS+=/usr/lib/libxvidcore.a
#cross compile
#LIBS+=/usr/local/lib/libxvidcore.a
