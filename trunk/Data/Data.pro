CONFIG += qt plugin debug warn_on
TARGET = Data
TEMPLATE = lib
VERSION = 0.0.8
CONFIG += plugin
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build
#DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats
DESTDIR = ../Release/imageformats

SOURCES += ImageIOPlugin.cpp \ 
	ImageIOHandler.cpp \
    AnimationIOHandler.cpp \
    ArtIOHandler.cpp \
    TextureIOHandler.cpp \
    GumpIOHandler.cpp \
    MulFileEngine.cpp

INCLUDEPATH += ../Nordwind

HEADERS += ImageIOPlugin.hpp \
    ImageIOHandler.hpp \
    Index.hpp \
    MulFileEngine.hpp \
    AnimationIOHandler.hpp \
    ArtIOHandler.hpp \
    TextureIOHandler.hpp \
    GumpIOHandler.hpp
