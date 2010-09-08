CONFIG += qt plugin debug warn_on
TARGET = OSI
TEMPLATE = lib
VERSION = 0.0.1
CONFIG += plugin
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build
DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats

SOURCES += OSIImageIOHandler.cpp \
    OSIImageIOHandlerPrivate.cpp \
    AnimationIOHandler.cpp \
    ArtIOHandler.cpp \
    TextureIOHandler.cpp \
    GumpIOHandler.cpp \
    OSIImage.cpp

HEADERS += OSIImage.hpp \
    OSIImageIOHandler.hpp \
    OSIImageIOHandlerPrivate.hpp \
    ../nordwind/resource/Index.hpp \
    AnimationIOHandler.hpp \
    ArtIOHandler.hpp \
    TextureIOHandler.hpp \
    GumpIOHandler.hpp \
