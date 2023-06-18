#TARGET = SchottenTotten_OOP-Cpp

QT = core gui widgets

HEADERS = \
   $$PWD/h/partie.h \
   $$PWD/h/schotten_totten.h \
   $$PWD/h/Tactique.h

SOURCES = \
   # $$PWD/cpp/main.cpp \
   $$PWD/cpp/partie.cpp \
   $$PWD/cpp/schotten_totten.cpp \
   $$PWD/cpp/Tactique.cpp \
   $$PWD/cpp/tuile.cpp \
   $$PWD/cpp/user_interface.cpp
   $$PWD/GUI_test.cpp

INCLUDEPATH = \
    $$PWD/h

#DEFINES = 

