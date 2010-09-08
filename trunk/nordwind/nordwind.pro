CONFIG += qt debug warn_on
TARGET = client
TEMPLATE = app
VERSION = 0.0.1
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build
SOURCES += game/Mobile.cpp \
    game/Entity.cpp \
    game/Static.cpp \
    game/Map.cpp \
    core/Resources.cpp \
    core/UserInterface.cpp \
    game/Scene.cpp \
    game/View.cpp \
    main.cpp \
    Client.cpp \
    resource/TileData.cpp \
    resource/Hues.cpp \
    resource/MulFileEngine.cpp \
    resource/IndexFileHandle.cpp \
    resource/FacetIOHandler.cpp
HEADERS += core/Console.h \
    game/Coordinate.hpp \
    core/Cursor.hpp \
    network/Packet.hpp \
    network/Socket.hpp \
    game/Map.hpp \
    game/Static.hpp \
    game/Dynamic.hpp \
    game/Character.hpp \
    game/Mobile.hpp \
    game/Entity.hpp \
    core/Resources.hpp \
    core/UserInterface.hpp \
    game/View.hpp \
    game/Scene.hpp \
    Client.hpp \
    resource/TileData.hpp \
    resource/Hues.hpp \
    util/Utilities.hpp \
    Typedefs.hpp \
    util/Singleton.hpp \
    util/DebugInformation.hpp \
    resource/MulFileEngine.hpp \
    resource/IndexFileHandle.hpp \
    resource/FacetIOHandler.hpp \
    resource/Index.hpp
