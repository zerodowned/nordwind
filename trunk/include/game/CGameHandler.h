#ifndef CWORLDVIEW_H
#define CWORLDVIEW_H
#include "../../interfaces/IModule.h"
#include <Qt/QMap.h>

/*
	This class holds:
	- all needed mapblocks
	- all needed staticblocks
	- all available dynamics
	- updates the world on network packets
*/
namespace game {
	class CGameHandler : public IModule {
		Q_OBJECT;
	private:
		// serial -> CDynamicObject*
		//QMap< quint32, CDynamicObject* > m_dynamicObjects;
		QMap< QString, CFacet* > m_facets;
		QMap< quint8, QString > m_facetlookup;
	public:
		CGameHandler( QObject* _parent);
		~CGameHandler();
		bool load();
		bool unload();
		bool reload();

		CFacet* facet( QString _facetname );
		CFacet* facet( quint8 _facetid );
		CFacet* facet() const;
		const QString& version();
		const QString& name();
		qint32 status();
		QString statusDesc( qint32 _status );
		QString statusInfo();
	};
	const QString gameVersion = "0.1 03/17/09";
	const QString gameName = "game";
}

#endif