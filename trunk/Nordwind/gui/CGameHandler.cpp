#include "CWorld.h"

using namespace game;

CGameHandler::CGameHandler( QObject* _parent ) : QGraphicsScene( _parent )  {
}

CGameHandler::~CGameHandler(void) {
	unload();
}

bool CGameHandler::load() {
	if(!IModule::load())
		return false;
	return true;
}

bool CGameHandler::unload() {
	if(!IModule::unload())
		return false;
	return true;
}

qint32 CGameHandler::status() {
	return 0;
}

QString CGameHandler::statusDesc( qint32 _status ) {
	return "No Error";
}

QString CGameHandler::statusInfo( ) {
	return "No Error";
}

const QString& CGameHandler::version() {
	return "0.1 03/15/09";
}

const QString& CGameHandler::name() {
	return gameName;
}