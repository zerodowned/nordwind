/*
 * LoginSocket.hpp
 *
 *  Created on: Oct 30, 2010
 *      Author: idstein
 */

#ifndef LOGINSOCKET_HPP_
#define LOGINSOCKET_HPP_

#include "Socket.hpp"

/*
 *
 */
namespace network {

class LoginSocket : public Socket {
    Q_OBJECT;
public:
	LoginSocket(const QString& host, quint16 port);
	virtual ~LoginSocket();
public Q_SLOTS:
        void establishConnection();
        void stopConnection();
	void dispatch();
	void authenticate(const QString& account, const QString& password);
private:
        quint8 mLastPacketID;
};

}

#endif /* LOGINSOCKET_HPP_ */
