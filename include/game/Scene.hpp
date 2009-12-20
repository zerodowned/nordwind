#ifndef SCENE_HPP__
#define SCENE_HPP__

#include <qcache.h>
#include <qgraphicsscene.h>
#include <qmap.h>
#include <qpointer.h>
#include "../resource/Facets.hpp"

namespace game {
	class Entity;
	class Scene;
	class Scene : public QGraphicsScene {
		Q_OBJECT;
		public:
			typedef QMultiMap< QPoint, QSharedPointer<Entity> > Block;
			typedef QList< QWeakPointer<Entity> > Cell;
			Scene( QPoint _center, QSize _view, resource::Facet _facet, QObject* parent);
//			Cell getTiles( QPoint _cell, qint16 _minZ = 0x8000, qint16 _maxZ = 0x7FFF );
		protected:
			Scene& constructBlock( QPoint _blockPos );
			Scene& addToScene( Block* _block );
			Scene& removeFromScene( Block* _block );
			resource::Facets::MapTile getNeighbour( const QPoint& _position, const resource::Facets::MapTile& _fallback );
			void connectNotify( const char* _signal );
		private:
			resource::Facet mFacet;
			QHash< QPoint, QWeakPointer<Block> > mBlocks;
//			QCache< QPoint, Block > mCache;
	};
}

inline uint qHash( const QPoint& _p ) {
	return qHash( QPair<int,int>(_p.x(),_p.y()) );
}

inline bool operator<(const QPoint& _p1, const QPoint& _p2) {
	return ( (_p1.x()<_p2.x()&&_p1.y()<=_p2.y()) || (_p1.x()<=_p2.x()&&_p1.y()<_p2.y()) ) ? true : false;
}
#endif
