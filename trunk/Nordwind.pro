# -------------------------------------------------
# Project created by QtCreator 2009-12-15T12:36:48
# -------------------------------------------------
QT += network
TARGET = client
TEMPLATE = app
DESTDIR = build/
INCLUDEPATH += include/
SOURCES += src/main.cpp \
    src/Client.cpp \
    src/core/Settings.cpp \
    src/core/Module.cpp \
    src/core/Log.cpp \
    src/core/CResourceCache.cpp \
    src/core/Core.cpp \
    src/game/CWorldView.cpp \
    src/game/CWorldBlock.cpp \
    src/game/CTextField.cpp \
    src/game/CPushButton.cpp \
    src/game/CPosition.cpp \
    src/game/CLabel.cpp \
    src/game/CItem.cpp \
    src/game/CGuiImage.cpp \
    src/game/CGui.cpp \
    src/game/CGameHandler.cpp \
    src/game/CFacet.cpp \
    src/game/CEditField.cpp \
    src/game/CCursor.cpp \
    src/game/CConsole.cpp \
    src/game/CCheckBox.cpp \
    src/game/CBorderGump.cpp \
    src/network/CSocket.cpp \
    src/network/CNetworkHandler.cpp \
    src/resource/TileData.cpp \
    src/resource/Textures.cpp \
    src/resource/Resources.cpp \
    src/resource/Object.cpp \
    src/resource/IndexFile.cpp \
    src/resource/Hues.cpp \
    src/resource/Gumps.cpp \
    src/resource/Facet.cpp \
    src/resource/CAsciiFonts.cpp \
    src/resource/Block.cpp \
    src/resource/Arts.cpp \
    src/resource/Animations.cpp
HEADERS += include/Client.hpp \
    include/core/Settings.hpp \
    include/core/Module.hpp \
    include/core/Log.hpp \
    include/core/CResourceCache.h \
    include/core/Core.hpp \
    include/game/CWorldView.h \
    include/game/CWorldBlock.h \
    include/game/CTextField.h \
    include/game/CPushButton.h \
    include/game/CPosition.h \
    include/game/CObject.h \
    include/game/CLabel.h \
    include/game/CItem.h \
    include/game/CGuiImage.h \
    include/game/CGui.h \
    include/game/CGameHandler.h \
    include/game/CFacet.h \
    include/game/CEditField.h \
    include/game/CCursor.h \
    include/game/CConsole.h \
    include/game/CCheckBox.h \
    include/game/CBorderGump.h \
    include/network/CSocket.h \
    include/network/CNetworkHandler.h \
    include/resource/TileData.hpp \
    include/resource/Textures.hpp \
    include/resource/Texture.hpp \
    include/resource/Resources.hpp \
    include/resource/Object.hpp \
    include/resource/IndexFile.hpp \
    include/resource/Hues.hpp \
    include/resource/Gumps.hpp \
    include/resource/Facet.hpp \
    include/resource/CAsciiFonts.h \
    include/resource/Cache.hpp \
    include/resource/Block.hpp \
    include/resource/Arts.hpp \
    include/resource/Animations.hpp \
    include/util/Utilities.hpp \
    include/util/Typedefs.hpp \
    include/util/Singleton.hpp \
    include/util/IPointer.h \
    include/util/DebugInformation.hpp \
    include/util/CFunctor.h
