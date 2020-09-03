QT += network
TEMPLATE  = app
SOURCES += logserver.cpp \
           server.cpp \
           model.cpp

HEADERS += server.h \
            model.h
CONFIG += debug c++11
