#ifndef PACKETS_HPP
#define PACKETS_HPP

#include <qglobal.h>

namespace network {

    struct LoginConfirmation {
        static const quint8 mPacketID = 0x1B;
        quint32 mSerial;
        quint8 mUnknown1[4];
        quint16 mBody;
        quint16 mX;
        quint16 mY;
        quint8 mUnknown2;
        qint8 mZ;
        quint8 mDirection;
        quint8 mUnknown3[9];
        quint16 mMapWidth;
        quint16 mMapHeight;
        quint8 mUnknown[6];
    };

    struct Teleport {
        static const quint8 mPacketID = 0x20;
        quint32 mSerial;
        quint16 mBody;
        quint8 mUnknown1;
        quint16 mHue;
        quint8 mStatus;
        quint16 mX;
        quint16 mY;
        quint8 mUnknown[2];
        quint8 mDirection;
        qint8 mZ;
    };

    struct EquippedMobile {
        static const quint8 mPacketID = 0x78;
        quint16 mLength;
        quint32 mSerial;
        quint16 mBody;
        quint16 mX;
        quint16 mY;
        qint8 mZ;
        quint8 mDirection;
        quint16 mHue;
        quint8 mStatus;
        quint8 mNotoriety;
        struct Equipement {
            quint32 mSerial;
            quint16 mArt;
            quint8 mLayer;
            quint16 mHue;
        };
    };
}
#endif // PACKETS_HPP
