#include "CNetworkHandler.h"
#include "CSocket.h"

using namespace network;

CNetworkHandler::CNetworkHandler( QObject* parent /*= 0*/ ) 
: IModule(parent),
m_instance(this) {
}


CNetworkHandler::~CNetworkHandler() {
	unload();
}

const QString& CNetworkHandler::version() {
	return networkVersion;
}

const QString& CNetworkHandler::name() {
	return networkName;
}

bool CNetworkHandler::load() {
	if(!IModule::load())
		return false;
	settings()->beginGroup("Hosts");
	foreach( QString subgroup, settings()->childGroups() ) {
		if( !m_sockets.contains( subgroup ) ) {
			CSocket* socket_ = new CSocket();
			QString address_ = settings()->value( "address", "" ).toString();
			quint16 port_ = settings()->value("port", 0 ).toUInt();
			socket_
			m_sockets[subgroup] = socket_;
		}
	}
	settings()->endGroup()
}


bool CNetworkHandler::unload() {
	if(!IModule::unload())
		return false;

}

