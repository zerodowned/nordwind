#ifndef SCENE_HPP__
#define SCENE_HPP__

#include <qgraphicsscene.h>
#include <qcache.h>
#include <qmap.h>
#include <qpoint.h>
#include <qsharedpointer.h>
#include "Typedefs.hpp"

inline bool operator<(const QPoint& _p1, const QPoint& _p2) {
	return (_p1.x() < _p2.x() || (_p1.x() == _p2.x() && _p1.y() < _p2.y())) ? true
			: false;
}

namespace resource {
	class FacetIOHandler;
}

namespace game {
	class Entity;
	class Scene;
	class Scene : public QGraphicsScene {
		Q_OBJECT;
		public:
			typedef QMultiMap< QPoint, QSharedPointer<Entity> > Block;
			Scene( const QString& name, QObject* parent);
			void loadMap( const QRect& rect );
		protected:
			void constructBlock( QPoint _blockPos );
			void connectNotify( const char* _signal );
		private:
			QSharedPointer<resource::FacetIOHandler> mFacet;
			QCache< ID, Block > mBlocks;
	};
}
#endif
