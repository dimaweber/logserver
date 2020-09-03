QT += network widgets gui
TEMPLATE  = app
SOURCES += logserver.cpp \
           fakelogclient.cpp \
           gui.cpp \
           server.cpp \
           model.cpp

HEADERS += server.h \
            fakelogclient.h \
            gui.h \
            model.h
CONFIG += debug c++11

DISTFILES += \
    fake.log
