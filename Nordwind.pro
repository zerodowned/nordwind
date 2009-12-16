# -------------------------------------------------
# Project created by QtCreator 2009-12-15T12:36:48
# -------------------------------------------------
# CONFIG = windows warn_on qt debug
QT += network
TARGET = client
TEMPLATE = app
VERSION = 0.0.1
DESTDIR = ./bin
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build
INCLUDEPATH += include/
SOURCES += src/main.cpp \
    src/Client.cpp \
#    src/resource/TileData.cpp \
	src/resource/Cache.cpp \
    src/resource/Textures.cpp \
    src/resource/Resources.cpp \
    src/resource/Object.cpp \
    src/resource/IndexFile.cpp \
    src/resource/Hues.cpp \
    src/resource/Gumps.cpp \
    src/resource/Facets.cpp \
    src/resource/Arts.cpp \
    src/resource/Animations.cpp
HEADERS += include/Client.hpp \
#    include/resource/TileData.hpp \
    include/resource/Textures.hpp \
    include/resource/Texture.hpp \
    include/resource/Resources.hpp \
    include/resource/Object.hpp \
    include/resource/IndexFile.hpp \
    include/resource/Hues.hpp \
    include/resource/Gumps.hpp \
    include/resource/Facets.hpp \
    include/resource/Cache.hpp \
    include/resource/Arts.hpp \
    include/resource/Animations.hpp \
    include/util/Utilities.hpp \
    include/util/Typedefs.hpp \
    include/util/Singleton.hpp \
    include/util/DebugInformation.hpp \
