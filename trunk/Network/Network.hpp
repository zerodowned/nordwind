#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Packet.hpp"
#include "Pipe.hpp"

#include <QVector3D>
#include <Loadable.hpp>
#include <qtcpsocket.h>

class Network: public QTcpSocket, public Loadable {
Q_OBJECT	;
        Q_INTERFACES(Loadable);
        Q_PROPERTY(QString host READ peerName WRITE setPeerName);
        Q_PROPERTY(quint16 port READ peerPort WRITE setPeerPort);
        Q_PROPERTY(quint32 seed READ seed WRITE setSeed);
public:
        Network(QObject* parent = NULL);
        virtual ~Network();
        using QTcpSocket::setPeerName;
        using QTcpSocket::setPeerPort;

        quint32 seed() const;
        void setSeed(quint32 seed);
Q_SIGNALS:
        void error(const QString& error, const QString& server);
        void networkStateChanged(const QString& text, const QString& server);
        void readyWrite();
        void receivedPacket(const Packet& packet);
        void readPacket(QDataStream& reader,quint8 id);
        void send(Packet& packet);

        void pingReceived(quint8 value);
        void musicPlayReceived(quint16 id);
        void soundPlayReceived(quint8 flags, quint16 effect, quint16 volume, const QVector3D& pos);

        void loginCompleted();

        void gameServerReceived(quint16 index, const QString& name, quint8 percentFull, quint8 timeZone, const QString& ip);
        void connectedToGameServer();
        void cityReceived( quint8 index, const QString& name, const QString& area );
        void characterReceived( const QString& name, const QString& password );
        void inGameReceived( quint32 serial, quint16 body, quint8 direction, const QVector3D& position, const QSize& map);

        void itemUpdated(quint32 serial, quint16 artwork, const QVector3D& position, quint16 hue, quint16 amount, quint8 direction, quint8 status);
        void dynamicRemoved(quint32 serial);

        void mobileUpdated(quint32 serial, quint16 body, quint8 direction, const QVector3D& pos, quint16 hue, quint8 status, quint8 notoriety);
        void mobileTeleported(quint32 serial, quint16 body, quint8 direction, const QVector3D& targetPos, quint16 hue, quint8 status);
        void mobileLayerUpdated(quint32 mobileSerial, quint32 itemSerial, quint8 layer, quint16 art, quint16 hue);
        void status(quint32 serial, const QString& name, const QVector2D& HP, const QVector2D& stamina, const QVector2D& mana,
                    bool nameChangeAllowed, quint8 gender, quint16 strength,
                    quint16 dexteriy, quint16 intelligence,
                    quint32 gold, quint16 physicalResistance, quint16 weight);

        void graphicalEffectReceived(quint8 type, quint32 source, quint32 target, quint16 objectID, QVector3D sourcePos, QVector3D targetPos, quint8 speed, quint8 duration, quint8 fixedDuration, quint8 explode);

        void unicodeTextReceived(quint32 serial, quint16 body, quint8 mode, quint16 hue, quint16 font, QByteArray lang, QString name, QString message);
public Q_SLOTS:
        bool registerReader(ID id, const char* slot);
        void connectToHost(const QString& account, const QString& password);
        void sendPing(quint8 value = 0);
        void sendLogin();
        void sendLogout();
        void sendLoginCharacter(const QString& name, const QString& password,quint32 index);
        void sendMoveRequest(quint8 direction, quint8 sequence, quint32 stackKey);
protected Q_SLOTS:
        void connectToHost();
        void sendHeader();
        void onError(QAbstractSocket::SocketError socketError);
        void onStateChanged(QAbstractSocket::SocketState socketState);
        void readDefault(QDataStream& reader, quint8 id);
        void readLoginServerPackets(QDataStream& reader, quint8 id);
         void readAccountLogin(QDataStream& reader);
         void readAccountLoginFailed(QDataStream& reader);
         void readLoginConfirmation(QDataStream& reader);
         void readTeleport(QDataStream& reader);
         void readFeatures(QDataStream& reader);
         void readCharacterAndCities(QDataStream& reader);
         void readTextUnicode(QDataStream& reader);
         void readMusicAndSound(QDataStream& reader, quint8 id);
         void readPing(QDataStream& reader);
         void readLoginComplete(QDataStream& reader);
         void readGameTime(QDataStream& reader);
         void readEffect(QDataStream& reader);
         void readStatus(QDataStream& reader);
         void readItem(QDataStream& reader, quint8 id);
         void readMobile(QDataStream& reader, quint8 id);
private Q_SLOTS:
        void dispatch(const Packet& packet);
        void readPacket();
        void writePacket(Packet& packet);
private:
        quint32 mSeed;
        QVector<const char*> mReaders;
        QByteArray mBuffer;
        QScopedPointer<Pipe> mEncryption;
        QScopedPointer<Pipe> mCompression;
        QString mAccount;
        QString mPassword;
};

#endif // NETWORK_HPP
