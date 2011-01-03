CONFIG += qt debug warn_on
QT += opengl network
TARGET = client
TEMPLATE = app
VERSION = 0.0.1
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build

debug {
    #DEFINES += _DEBUG
    INCLUDEPATH += ../conan/include
    LIBS += -L../conan/lib/ -lConand
}

RESOURCES = resources/resources.qrc

SOURCES += game/Scene.cpp \
    game/TileData.cpp \
    gui/ViewWidget.cpp \
    gui/PaperdollWidget.cpp \
    gui/ShardPage.cpp \
    Nordwind.cpp \
    gui/Client.cpp \
    Logger.cpp \
    gui/LoginDialog.cpp \
    game/Object.cpp \
    game/Dynamic.cpp \
    game/object/Static.cpp \
    game/object/Mobile.cpp \
    game/object/Ground.cpp \
    game/object/Item.cpp \
    gui/CharacterListDialog.cpp

HEADERS += Typedefs.hpp \
    game/TileData.hpp \
    game/Scene.hpp \
    gui/ViewWidget.hpp \
    gui/PaperdollWidget.hpp \
    gui/ShardPage.hpp \
    Nordwind.hpp \
    gui/Client.hpp \
    Logger.hpp \
    gui/LoginDialog.hpp \
    game/Object.hpp \
    game/Dynamic.hpp \
    game/object/Static.hpp \
    game/object/Mobile.hpp \
    game/object/Ground.hpp \
    game/object/Character.hpp \
    game/object/Item.hpp \
    gui/CharacterListDialog.hpp

FORMS += gui/PaperdollWidget.ui \
    gui/ShardPage.ui \
    gui/LoginDialog.ui \
    gui/CharacterListDialog.ui
