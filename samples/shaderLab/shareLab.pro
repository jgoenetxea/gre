#-------------------------------------------------
#
# Project created by QtCreator 2014-07-23T23:43:25
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shaderLab
TEMPLATE = app

INCLUDEPATH += /usr/include/

SOURCES += main.cpp\
        mainwindow.cpp \
        glView.cpp \
        codeeditor.cpp \
        highlighter.cpp \
        ../gre/camera.cpp \
        ../gre/projectiveCamera.cpp \
        ../gre/orthoCamera.cpp \
        ../gre/node.cpp \
        ../gre/scene.cpp \
        ../gre/obj.cpp \
        ../gre/renderer.cpp \
        ../gre/translation.cpp \
        ../gre/objloader.cpp \
        ../gre/shaderProgram.cpp \
        ../gre/shapes.cpp \
        ../gre/texture.cpp \
        scenes/simpleQuad.cpp


HEADERS  += mainwindow.h \
            glView.h \
            testGlView.h \
            codeeditor.h \
            highlighter.h \
            ../gre/camera.hpp \
            ../gre/projectiveCamera.hpp \
            ../gre/orthoCamera.hpp \
            ../gre/node.hpp \
            ../gre/scene.hpp \
            ../gre/obj.hpp \
            ../gre/renderer.hpp \
            ../gre/translation.hpp \
            ../gre/objloader.hpp \
            ../gre/shaderProgram.hpp \
            ../gre/shapes.hpp \
            ../gre/texture.hpp \
            scenes/simpleQuad.h
            

FORMS    += forms/mainwindow.ui
RESOURCES += resources/images.qrc

