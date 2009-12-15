#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <Qt/QTcpSocket.h>
#include <Qt/QQueue.h>

namespace network {
	class CSocket : public QTcpSocket
	{
		enum eSocketState {
				DISCONNECTED,
				CONNECTING,
				LOGIN,
				INGAME
		}
	Q_OBJECT
	private:
		QQueue<CPacket*> m_pendingPackets;
		quint32 m_seed;

		QString m_address;
		quint16 m_port;

		QVector<QString> m_accounts;
		quint32 m_lastPacketId;

		QList<CIncomingPacket*> m_inPackets;
		QByteArray m_inBuffer;

		// @TODO packet logging
		//void logPacket(const QByteArray &data);
		//void logPacket(CIncomingPacket *packet);

		// status
		quint32 m_outBytes;
		quint32 m_inBytes;
		quint32 m_inBytesCompressed;
		quint32 m_features;

		QVector<fnIncomingPacketHandler> m_packethandler;
	public:
		CSocket();
		virtual ~CSocket();
		
		// bool registerPacket( CPacket* _packet );
		// void unregisterPacket( quint8 _id );
		// bool canHandlePacket( quint8 _id );

	};
}
#endif