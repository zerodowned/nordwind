/*
 * GameSocket.hpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#ifndef GAMESOCKET_HPP_
#define GAMESOCKET_HPP_

#include "Socket.hpp"
#include <qbuffer.h>

/*
 *
 */
namespace network {

    class GameSocket : public Socket {
        Q_OBJECT;
public:
        GameSocket(const QString& host, quint16 port, quint32 authID);
	virtual ~GameSocket();
	enum Feature {
		// 0xb9 Features
		T2A = 0x0001 | 0x0004, // T2A complete
		LBR = 0x0008 | 0x0002, // LBR + Sound
		AOS = 0x0010, // AOS

		// Characterlist Features
		CONTEXTMENUS = 0x08,
		TOOLTIPS = 0x20,
		AOSPROFESSIONS = 0x20,
		SEPROFESSIONS = 0x80,
		SIXTHCHARSLOT = 0x40,
		ONECHARSLOT = 0x14,
	};
	Q_DECLARE_FLAGS(Features, Feature);
private:
        quint8 mLastPacketID;
        quint32 mAuthID;
        QBuffer mInbound;
        QDataStream mStream;
	Features mFeatures;
    public Q_SLOTS:
        void establishConnection();
        void stopConnection();
        void dispatch();
        void authenticate(const QString& account, const QString& password);
};

}

#endif /* GAMESOCKET_HPP_ */
