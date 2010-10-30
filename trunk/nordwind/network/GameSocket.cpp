/*
 * GameSocket.cpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#include "GameSocket.hpp"
#include <qapplication.h>
#include "compression/Huffman.hpp"
#include "encryption/Twofish2.hpp"

using namespace network;

GameSocket::GameSocket(const QString& host, quint16 port, quint32 authID)
    : Socket(host,port,0x1000007F),
    mLastPacketID(0xFF),
mAuthID(authID) {
    setObjectName("GameServer");
    mCompression.reset(new Huffman);
    mEncryption.reset(new Twofish(mSeed));
    mStream.setDevice(&mInbound);
}

GameSocket::~GameSocket() {
}

void GameSocket::establishConnection() {
    Socket::establishConnection();
    connect(qApp,SIGNAL(login(const QString&,const QString&)),SLOT(authenticate(const QString&,const QString&)));
}

void GameSocket::stopConnection() {
    disconnect(this,SLOT(authenticate(QString,QString)));
}

void GameSocket::dispatch() {
    // catch every incoming data, decompress & decrypt it and push it into the buffer for dispatch operations
    QByteArray inbound = readAll();
	qDebug() << inbound.toHex();
    mStream.writeRawData(inbound.data(),inbound.length());
    while(mLastPacketID!=0xFF||!mStream.atEnd()&&mStream.readRawData(reinterpret_cast<char*>(&mLastPacketID),1)==1) {
        switch(mLastPacketID) {
            default:
                qWarning() << "Unknown packet" << mLastPacketID;
                qDebug() << mInbound.readAll().toHex();
        }
    }
}

void GameSocket::authenticate(const QString &account, const QString &password) {
    Socket::Packet auth(65,0);
    auth[0] = 0x91;
    auth.replace(1,30,account.toAscii()).replace(31,30,password.toAscii());
    auth.replace(61,4,reinterpret_cast<char*>(&mAuthID));
    writeData(auth.data(),auth.length());
}
