# -------------------------------------------------
# Project created by QtCreator 2009-12-15T12:36:48
# -------------------------------------------------
# CONFIG = windows warn_on qt debug
QT += network script
TARGET = client
TEMPLATE = app
VERSION = 0.0.1
DESTDIR = ./bin
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build
INCLUDEPATH += include/
SOURCES += src/game/Mobile.cpp \
    src/game/Entity.cpp \
    src/game/Static.cpp \
    src/game/Map.cpp \
    src/core/Resources.cpp \
    src/core/UserInterface.cpp \
    src/game/Scene.cpp \
    src/game/View.cpp \
    src/main.cpp \
    src/Client.cpp \
    src/resource/TileData.cpp \
    src/resource/Textures.cpp \
    src/resource/IndexFile.cpp \
    src/resource/Hues.cpp \
    src/resource/Gumps.cpp \
    src/resource/Facets.cpp \
    src/resource/Arts.cpp \
    src/resource/Animations.cpp
HEADERS += include/core/Console.h \
    include/game/Coordinate.hpp \
    include/core/Cursor.hpp \
    include/network/Packet.hpp \
    include/network/Socket.hpp \
    include/game/Map.hpp \
    include/game/Static.hpp \
    include/game/Dynamic.hpp \
    include/game/Character.hpp \
    include/game/Mobile.hpp \
    include/resource/GID.hpp \
    include/game/Entity.hpp \
    include/core/Resources.hpp \
    include/core/UserInterface.hpp \
    include/game/View.hpp \
    include/game/Scene.hpp \
    include/Client.hpp \
    include/resource/TileData.hpp \
    include/resource/Textures.hpp \
    include/resource/IndexFile.hpp \
    include/resource/Hues.hpp \
    include/resource/Gumps.hpp \
    include/resource/Facets.hpp \
    include/resource/Arts.hpp \
    include/resource/Animations.hpp \
    include/util/Utilities.hpp \
    include/util/Typedefs.hpp \
    include/util/Singleton.hpp \
    include/util/DebugInformation.hpp
