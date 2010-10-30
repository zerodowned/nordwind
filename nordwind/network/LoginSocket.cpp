/*
 * LoginSocket.cpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#include "LoginSocket.hpp"
#include "GameSocket.hpp"
#include <qapplication.h>
#include <qhostaddress.h>
#include <QtEndian>

using namespace network;

LoginSocket::LoginSocket(const QString& host, quint16 port)
: Socket(host,port),
mLastPacketID(0xFF) {
	setObjectName("LoginServer");
}

LoginSocket::~LoginSocket() {
}

void LoginSocket::establishConnection() {
    Socket::establishConnection();
    connect(qApp,SIGNAL(login(const QString&,const QString&)),SLOT(authenticate(const QString&,const QString&)));
}

void LoginSocket::stopConnection() {
    disconnect(this,SLOT(authenticate(QString,QString)));
}

void LoginSocket::dispatch() {
    while(mLastPacketID!=0xFF||(bytesAvailable()&&read(reinterpret_cast<char*>(&mLastPacketID),1)==1)) {
        switch(mLastPacketID) {
            case 0x81:
                quint16 length;
                if(!(peek(reinterpret_cast<char*>(length),2)==2&&bytesAvailable()==length-1))
                    return;
                read(length-1);
                break;
        case 0x8C: {
                if(!bytesAvailable()==11)
                    return;
                quint32 IPv4;
                read(reinterpret_cast<char*>(&IPv4),4);
                quint16 port;
                read(reinterpret_cast<char*>(&port),2);
                quint32 authID;
                read(reinterpret_cast<char*>(&authID),4);
                // disconnect from login server
                disconnectFromHost();
                deleteLater();
                // connect to game server
                GameSocket* socket = new GameSocket(QHostAddress(qToBigEndian(IPv4)).toString(),qToBigEndian(port),authID);
                socket->establishConnection();
                mLastPacketID = 0xFF;
                return;
            }
            default:
                qWarning() << "Unknown packet" << mLastPacketID;
                qDebug() << readAll().toHex();
        }
    }
}

void LoginSocket::authenticate(const QString& account, const QString& password) {
	Socket::Packet auth(62,0x00);
	auth[0] = 0x80;
	auth.replace(1,30,account.toAscii()).replace(31,30,password.toAscii());
        writeData(auth.data(),auth.length());
}
