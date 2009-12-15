#ifndef CNETWORKHANDLER_H_
#define CNETWORKHANDLER_H_

#include "../client/interfaces/IModule.h"
#include <Qt/QMap.h>

namespace network {
	class CSocket;
	class CNetworkHandler : public QObject
	{
		Q_OBJECT
	private:
		QMap<QSring /*_identifier*/, CSocket* /*_socket*/> m_sockets;
		static CNetworkHandler* m_instance;
	public:
		CNetworkHandler(QObject* _parent = NULL);
		virtual ~CNetworkHandler();
		const QString& version();
		const QString& name();
		bool load();
		bool unload();
		bool reload();
		qint32 status();
		QString statusDesc( qint32 _status );
		QString statusInfo();
		static CNetworkHandler& instance( void ) {
				return (*m_instance);
		}

		// identifier is subgroup from network.ini
		CSocket* socket( QString _identifier );
		bool addSocket( QString _identifier );
		bool removeSocket( QString _identifier );
	};
	const QString networkVersion = "0.1 03/31/09";
	const QString networkName = "network";
}
#endif