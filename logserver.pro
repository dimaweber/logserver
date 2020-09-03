QT += network widgets gui
TEMPLATE  = app
SOURCES += logserver.cpp \
           fakelogclient.cpp \
           filtermodel.cpp \
           gui.cpp \
           logline.cpp \
           server.cpp \
           model.cpp

HEADERS += server.h \
            fakelogclient.h \
            filtermodel.h \
            gui.h \
            logline.h \
            model.h
CONFIG += debug c++11

DISTFILES += \
    fake.log
