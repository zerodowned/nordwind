#-------------------------------------------------
#
# Project created by QtCreator 2010-11-01T12:50:24
#
#-------------------------------------------------

CONFIG += qt plugin debug warn_on
QT += network
TARGET = Network
TEMPLATE = lib
VERSION = 0.0.1
CONFIG += plugin
RCC_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
UI_DIR = ./build
DESTDIR = ../Release

SOURCES += Network.cpp \
    encryption/XOREncryption.cpp \
    encryption/Twofish2.cpp \
    compression/Huffman.cpp

INCLUDEPATH += ../Nordwind

HEADERS += Network.hpp \
    Packets.hpp \
    encryption/XOREncryption.hpp \
    encryption/Twofish2.hpp \
    compression/Huffman.hpp \
    Packet.hpp \
    Pipe.hpp
