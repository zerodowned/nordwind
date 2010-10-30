#include "Socket.hpp"

#include <qapplication.h>
#include <QtEndian>

using namespace network;

Socket::Socket(const QString& host, quint16 port, quint32 seed) : QTcpSocket(),
mSeed(seed) {
	setObjectName("Network");
	setParent(qApp);
	setPeerName(host);
	setPeerPort(port);

        connect(this,SIGNAL(error(QAbstractSocket::SocketError)),qApp,SLOT(error()));
        connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),qApp,SLOT(networkState(QAbstractSocket::SocketState)));
}


Socket::~Socket() {
}

void Socket::establishConnection() {
    connectToHost(peerName(),peerPort());
    connect(this,SIGNAL(readyRead()),SLOT(dispatch()));
    connect(this,SIGNAL(connected()),SLOT(sendHeader()));
    connect(this,SIGNAL(readyWrite()),qApp,SLOT(connectionEstablished()));
}

void Socket::sendHeader() {
    QTcpSocket::writeData(reinterpret_cast<char*>(&mSeed),sizeof(mSeed));
    emit readyWrite();
}

qint64 Socket::writeData(const char* data,qint64 maxSize) {
	QByteArray buffer(data,maxSize);
	if(mEncryption)
		mEncryption->out(buffer);
	if(mCompression)
		mCompression->out(buffer);
        qint64 result = QTcpSocket::writeData(buffer.data(),buffer.length());
        flush();
        return result;
}

qint64 Socket::readData(char* data, qint64 maxSize) {
	QByteArray buffer(data,maxSize);
	buffer.resize(QTcpSocket::readData(buffer.data(),buffer.length()));
        if(mEncryption)
                mEncryption->in(buffer);
        if(mCompression)
		mCompression->in(buffer);
	memcpy(data,buffer.data(),buffer.length());
	return buffer.length();
}

/*void Socket::connectToGameServer(const QString& account, const QString& password) {
	mStream << 0x91 << mSeed;
	QByteArray acc = account.toAscii();
	mStream.writeBytes(acc.data(),30);
	QByteArray pwd = password.toAscii();
	mStream.writeBytes(pwd.data(),30);
}*/

void Socket::dispatch() {
}

