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
			Scene( resource::Facet _facet, QObject* parent);
			Scene& loadMap( const QPoint& _center, const QSize& _dimension );
//			Cell getTiles( QPoint _cell, qint16 _minZ = 0x8000, qint16 _maxZ = 0x7FFF );
		protected:
			Scene& constructBlock( QPoint _blockPos );
			Scene& removeFromScene( Block* _block );
			void connectNotify( const char* _signal );
		private:
			resource::Facet mFacet;
			QHash< QPoint, QWeakPointer<Block> > mBlocks;
//			QCache< QPoint, Block > mCache;
	};
}
#endif
