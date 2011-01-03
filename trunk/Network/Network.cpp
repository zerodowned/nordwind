#include "Network.hpp"
#include "compression/Huffman.hpp"

#include <qapplication.h>
#include <QHostAddress>
#include <QTime>
#include <QVector2D>

Network::Network(QObject *parent)
    : QTcpSocket(parent),
    Loadable(),
    mSeed(qrand()),
    mReaders(256,SLOT(readDefault(QDataStream&,quint8))) {
    setObjectName("Network");

    // register packet decoders
    registerReader(0x1A,SLOT(readItem(QDataStream&,quint8)));
    registerReader(0x1D,SLOT(readItem(QDataStream&,quint8)));
    registerReader(0x1B,SLOT(readLoginConfirmation(QDataStream&)));
    registerReader(0x20,SLOT(readTeleport(QDataStream&)));
    registerReader(0x54,SLOT(readMusicAndSound(QDataStream&,quint8)));
    registerReader(0x55,SLOT(readLoginComplete(QDataStream&)));
    registerReader(0x5B,SLOT(readGameTime(QDataStream&)));
    registerReader(0x6D,SLOT(readMusicAndSound(QDataStream&,quint8)));
    registerReader(0x70,SLOT(readEffect(QDataStream&)));
    registerReader(0x73,SLOT(readPing(QDataStream&)));
    registerReader(0x77,SLOT(readMobile(QDataStream&,quint8)));
    registerReader(0x78,SLOT(readMobile(QDataStream&,quint8)));
    registerReader(0x81,SLOT(readAccountLogin(QDataStream&)));
    registerReader(0x82,SLOT(readAccountLoginFailed(QDataStream&)));
    registerReader(0x8C,SLOT(readLoginServerPackets(QDataStream&,quint8)));
    registerReader(0xB9,SLOT(readFeatures(QDataStream&)));
    registerReader(0x8A,SLOT(readLoginServerPackets(QDataStream&,quint8)));
    registerReader(0xA9,SLOT(readCharacterAndCities(QDataStream&)));
    registerReader(0xAE,SLOT(readTextUnicode(QDataStream&)));

    // prepare write channel
    connect(this,SIGNAL(connected()),SLOT(sendHeader()));
    connect(this,SIGNAL(send(Packet&)),SLOT(writePacket(Packet&)));
    // prepare read channel
    connect(this,SIGNAL(readyRead()),SLOT(readPacket()));
    connect(this,SIGNAL(receivedPacket(const Packet&)),SLOT(dispatch(const Packet&)));
    // signal errors
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(onError(QAbstractSocket::SocketError)));
    // signals state updates
    connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),SLOT(onStateChanged(QAbstractSocket::SocketState)));
}

Network::~Network() {
}

quint32 Network::seed() const {
    return mSeed;
}

void Network::setSeed(quint32 seed) {
    mSeed = seed;
}

void Network::connectToHost() {
    qDebug() << "Connecting to " << peerName() << ":" << peerPort();
        QTcpSocket::connectToHost(peerName(),peerPort());
}

void Network::connectToHost(const QString &account, const QString &password) {
    mAccount = account;
    mPassword = password;
    connectToHost();
}

void Network::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    QAbstractSocket* socket = qobject_cast<QAbstractSocket*>(sender());
    if(socket)
        emit error(socket->errorString(),socket->peerAddress().toString());
}

void Network::onStateChanged(QAbstractSocket::SocketState socketState) {
    QAbstractSocket* socket = qobject_cast<QAbstractSocket*>(sender());
    if(socket) {
        QString message;
        switch(socketState) {
            case QAbstractSocket::UnconnectedState:
                message = tr("Not connected");
                break;
            case QAbstractSocket::HostLookupState:
                message = tr("Host lookup");
                break;
            case QAbstractSocket::ConnectingState:
                    message = tr("Connecting...");
                    break;
            case QAbstractSocket::ConnectedState:
                    message = tr("Connected!");
                    break;
            case QAbstractSocket::ClosingState:
                    message = tr("Disconnecting...");
                    break;
            default:
                    message = tr("Not connected.");
        }
    emit networkStateChanged(message,socket->peerAddress().toString());
    }
}

void Network::writePacket(Packet& packet) {
        if(mEncryption)
                mEncryption->out(packet);
        //if(mCompression)
        //        mCompression->out(packet);
        writeData(packet.data(),packet.length());
        flush();
}

void Network::readPacket() {
        Packet packet = readAll();
        if(mEncryption)
          packet = mEncryption->in(mBuffer);
        if(mCompression) {
            mBuffer.append(packet);
            do {
                packet = mCompression->in(mBuffer);
                emit receivedPacket(packet);
            } while(packet.size()>0);
        } else
            emit receivedPacket(packet);
}

void Network::dispatch(const Packet &packet) {
    Reader reader(packet);
    ID id;
    while(!reader.atEnd()) {
        reader >> id;
        connect(this,SIGNAL(readPacket(QDataStream&,quint8)),mReaders[id],Qt::DirectConnection);
        emit readPacket(reader,id);
        disconnect(this,mReaders[id]);
    }
}

bool Network::registerReader(ID id, const char* read) {
    mReaders[id] = read;
    return true;
}

void Network::readDefault(QDataStream &reader, quint8 id ) {
    qDebug() << Q_FUNC_INFO;
    qWarning() << QString::number(id,16) << "Unknown packet => flush buffer" << reader.device()->size() << "bytes";
    qWarning() << reader.device()->readAll().toHex();
}

void Network::readLoginServerPackets(QDataStream &reader, quint8 id) {
    switch(id) {
    case 0xA8: {
            quint16 length, count;
            quint8 flags;
            reader >> length >> flags >> count;
            for(quint16 i = 0; i < count; i++) {
                quint16 index;
                QByteArray name(32,0);
                quint8 percentFull,timeZone;
                quint32 ip;
                reader >> index;
                reader.readRawData(name.data(),name.size());
                reader >> percentFull >> timeZone >> ip;
                emit gameServerReceived(index,name,percentFull,timeZone,QHostAddress(ip).toString());
            }
            break;
        }
    case 0x8C: {
            quint32 IPv4;
            quint16 port;
            reader >> IPv4 >> port >> mSeed;
            disconnectFromHost();
            setPeerName(QHostAddress(IPv4).toString());
            setPeerPort(port);
            connectToHost();
            setProperty("GameServer",true);
            mCompression.reset(new Huffman);
            parent()->connect(this,SIGNAL(inGameReceived(quint32,quint16,quint8,QVector3D,QSize)),SLOT(ingame(quint32,quint16,quint8,QVector3D,QSize)));
            break;
        }
    }
}

void Network::readAccountLogin(QDataStream &reader) {
    qDebug() << Q_FUNC_INFO << "unhandled";
    quint16 length;
    reader >> length;
    reader.skipRawData(length);
}

void Network::readLoginConfirmation(QDataStream &reader) {
    quint32 serial;
    quint16 body,x,y,mapWidth,mapHeight;
    qint8 z;
    quint8 direction;
    reader >> serial;
    reader.skipRawData(4);
    reader >> body >> x >> y;
    reader.skipRawData(1);
    reader >> z;
    reader >> direction;
    reader.skipRawData(9);
    reader >> mapWidth >> mapHeight;
    reader.skipRawData(6);
    emit inGameReceived(serial,body,direction,QVector3D(x,y,z),QSize(mapWidth,mapHeight));
}

void Network::readTeleport(QDataStream& reader) {
    qDebug() << Q_FUNC_INFO;
    quint32 serial;
    quint16 hue,body,x,y;
    quint8 direction, status;
    qint8 z;
    reader >> serial >> body;
    reader.skipRawData(1);
    reader >> hue >> status >> x >> y;
    reader.skipRawData(2);
    reader >> direction >> z;
    emit mobileTeleported(serial,body,direction,QVector3D(x,y,z),hue,status);
}

void Network::readMobile(QDataStream &reader, quint8 id) {
    quint32 serial;
    quint16 length, body,x,y,hue, corpse;
    quint8 direction,status,notoriety;
    qint8 z;
    if(id==0x78)
        reader >> length;
    reader >> serial >> body;
    if(id==0x78&&serial&0x80000000)
        reader >> corpse;
    reader >> x >> y >> z >> direction >> hue >> status >> notoriety;
    // update/add Mobile
    emit mobileUpdated(serial,body,direction,QVector3D(x,y,z),hue,status,notoriety);
    if(id==0x78) {
        // add equipement
        quint32 eserial;
        quint16 eart,ehue;
        quint8 elayer;
        while(!reader.atEnd()) {
            reader >> eserial;
            if(eserial==0x000000000)
                break;
            reader >> eart >> elayer;
            if(eart&0x8000)
                reader >> ehue;
            else
                ehue = 0;
            emit mobileLayerUpdated(serial,eserial,elayer, eart, ehue);
        }
    }
}

void Network::readFeatures(QDataStream &reader) {
    qDebug() << Q_FUNC_INFO << "unhandled";
    quint32 flags;
    reader >> flags;
    parent()->setProperty("Features",flags);
}

void Network::readCharacterAndCities(QDataStream &reader) {
    quint16 length;
    quint8 count;
    reader >> length >> count;
    for(quint8 i = 0; i < count; i++ ) {
        QByteArray name(30,0);
        QByteArray password(30,0);
        reader.readRawData(name.data(),name.size());
        reader.readRawData(password.data(),password.size());
        emit characterReceived(name,password);
    }
    length -= count*60+1;
    reader >> count;
    for(quint8 i = 0; i < count; i++) {
        quint8 index;
        reader >> index;
        QByteArray name(31,0);
        QByteArray area(31,0);
        reader.readRawData(name.data(),name.size());
        reader.readRawData(area.data(),area.size());
        emit cityReceived(index,name,area);
    }
    length -= count*63+1;
    if(length>=5) {
        qDebug() << Q_FUNC_INFO << "flags not handled";
        reader.skipRawData(length-5);
        quint32 flags;
        reader >> flags;
    }
    // TODO save flags?
}

void Network::readTextUnicode(QDataStream &reader) {
    quint32 serial;
    quint16 body,length,hue,font;
    quint8 mode;
    QByteArray lang(4,0);
    QByteArray name(30,0);
    reader >> length >> serial >> body >> mode >> hue >> font;
    reader.readRawData(lang.data(),lang.size());
    reader.readRawData(name.data(),name.size());
    QString message(length-50,0x0000);
    for(QString::Iterator iter = message.begin(); iter != message.end(); iter++)
        reader >> *iter;
    emit unicodeTextReceived(serial,body,mode,hue,font,lang,name,message);
}

void Network::readMusicAndSound(QDataStream& reader, quint8 id) {
    qDebug() << Q_FUNC_INFO << "unhandled";
    switch(id) {
    case 0x6D: {
        quint16 musicNumber;
        reader >> musicNumber;
        emit musicPlayReceived(musicNumber);
        break;
    }
    case 0x54: {
        quint8 flags;
        quint16 effect, volume, x,y,z;
        reader >> flags >> effect >> volume >> x >> y >> z;
        emit soundPlayReceived(flags,effect,volume,QVector3D(x,y,z));
        break;
    }
    }
}

void Network::readItem(QDataStream &reader, quint8 id) {
    quint32 serial;
    switch(id) {
        case 0x1D:
            reader >> serial;
            emit dynamicRemoved(serial);
            break;
    case 0x1A:
        quint16 length, artwork, amount,x,y,hue;
        quint8 stackID, direction, status;
        qint8 z;
        reader >> length >> serial >> artwork;
        if(serial&0x80000000)
            reader >> amount;
        else
            amount = 1;
        serial&=0x7FFFFFFF;
        if(artwork&0x8000)
            reader >> stackID;
        else
            stackID = 0;
        reader >> x >> y;
        if(x&0x8000)
            reader >> direction;
        else
            direction = -1;
        reader >> z;
        if(y&0x8000)
            reader >> hue;
        else
            hue = 0;
        if(y&0x4000)
            reader >> status;
        else
            status = -1;
        x &= 0x7FFF;
        y &= 0x3FFF;
        emit itemUpdated(serial,artwork+stackID,QVector3D(x,y,z),hue,amount,direction,status);
    }
}

void Network::readPing(QDataStream &reader) {
    qDebug() << Q_FUNC_INFO << "unhandled";
    quint8 value;
    reader >> value;
    emit pingReceived(value);
}

void Network::readLoginComplete(QDataStream &reader) {
    Q_UNUSED(reader);
    emit loginCompleted();
}

void Network::readGameTime(QDataStream &reader) {
    qDebug() << Q_FUNC_INFO << "unhandled";
    quint8 hours,minutes,seconds;
    reader >> hours >> minutes >> seconds;
    qApp->setProperty("GameTime",QTime(hours,minutes,seconds));
}

void Network::readAccountLoginFailed(QDataStream &reader) {
    quint8 errCode;
    reader >> errCode;
    QString message("Unknown Error");
    switch(errCode) {
        case 0:
            message = "No Account";
            break;
        case 1:
            message = "Account in Use";
            break;
        case 2:
            message ="Account Blocked";
            break;
        case 3:
            message = "No Password";
            break;
        case 6:
            message = "IGR";
            break;
        case 9:
            message = "Character Transfer";
            break;
        case 0xFE:
            message = "Idle";
            break;
        default:
            message = "Bad Communication";
    }
    emit error(message,"GameServer");
}

void Network::readEffect(QDataStream &reader) {
    qDebug() << Q_FUNC_INFO << "unhandled";
    quint32 source,target;
    quint16 objectID,x,y,tarX,tarY;
    quint8 type, speed, duration, fixedDuration, explode;
    qint8 z, tarZ;
    reader >> type >> source >> target >> objectID >> x >> y >> z >> tarX >> tarY >> tarZ >> speed >> duration;
    reader.skipRawData(2); // unknown quint8[2]
    reader >> fixedDuration >> explode;
    emit graphicalEffectReceived(type,source,target,objectID,QVector3D(x,y,z),QVector3D(tarX,tarY,tarZ),speed,duration,fixedDuration,explode);
}

void Network::readStatus(QDataStream &reader) {
    quint32 serial, gold;
    quint16 length, hp, maxHP, strength, dexterity, intelligence, stamina, maxStamina, mana,maxMana, physicalResistance,weight;
    quint8 supportedFeatures, gender;
    bool allowNameChange;
    QByteArray name(30,0);
    reader >> length >> serial;
    reader.readRawData(name.data(),name.size());
    reader >> hp >> maxHP >> allowNameChange >> supportedFeatures >> gender
            >> strength >> dexterity >> intelligence >> stamina >> maxStamina
            >> mana >> maxMana >> gold >> physicalResistance >> weight;
    qDebug() << "Unsupported yet" << reader.device()->readAll().toHex();
    emit status(serial,name,QVector2D(hp,maxHP),QVector2D(stamina,maxStamina),QVector2D(mana,maxMana),
                allowNameChange,gender,strength,dexterity,intelligence,
                gold,physicalResistance,weight);
}

void Network::sendHeader() {
    qDebug() << Q_FUNC_INFO;
    QDataStream stream(this);
    stream << mSeed;
    flush();
    emit readyWrite();
    sendLogin();
}

void Network::sendPing(quint8 value) {
    qDebug() << Q_FUNC_INFO;
    Packet ping;
    QDataStream writer(&ping,QIODevice::WriteOnly);
    writer << (quint8)0x73 << value;
    emit send(ping);
}

void Network::sendLogout() {
    qDebug() << Q_FUNC_INFO;
    Packet logout;
    Writer writer(&logout,QIODevice::WriteOnly);
    writer << (quint8)0x01 << (quint32)0xFFFFFFFF;
    emit send(logout);
}

void Network::sendMoveRequest(quint8 direction, quint8 sequence, quint32 stackKey) {
    qDebug() << Q_FUNC_INFO;
    Packet moveRequest;
    Writer writer(&moveRequest,QIODevice::WriteOnly);
    writer << (quint8)0x02 << direction << sequence << stackKey;
    emit send(moveRequest);
}

void Network::sendLogin() {
    qDebug() << Q_FUNC_INFO;
    Packet auth;
    Writer writer(&auth,QIODevice::WriteOnly);
    if(property("GameServer").toBool())
        writer << (quint8)0x91 << mSeed;
    else
        writer << (quint8)0x80;
    writer.writeRawData(mAccount.leftJustified(30,0,true).toAscii(),30);
    writer.writeRawData(mPassword.leftJustified(30,0,true).toAscii(),30);
    if(!property("GameServer").toBool())
        writer << (quint8)0x00;
    emit send(auth);
}

void Network::sendLoginCharacter(const QString &name, const QString& password, quint32 index) {
    qDebug() << Q_FUNC_INFO;
    Packet login;
    Writer writer(&login,QIODevice::WriteOnly);
    writer << (quint8)0x5D << 0xEDEDEDED;
    writer.writeRawData(name.leftJustified(30,0,true).toAscii(),30);
    writer.writeRawData(password.leftJustified(30,0,true).toAscii(),30);
    writer << index << mSeed;
    emit send(login);
}

Q_EXPORT_PLUGIN2(Network, Network);
