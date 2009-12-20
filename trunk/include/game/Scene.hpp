#ifndef SCENE_HPP__
#define SCENE_HPP__

#include <qcache.h>
#include <qgraphicsscene.h>
#include <qmap.h>
#include <qpointer.h>
#include "../resource/Facets.hpp"

namespace game {
	class Scene;
	class Scene : public QGraphicsScene {
		Q_OBJECT;
		public:
			class Block : public QObject {
				public:
					Block( QPointer<Scene> _scene, const QSharedPointer<resource::Block>& _rawBlock, QObject* _parent );
					virtual ~Block();
					//					void moveToCache();
				private:
					QPointer<Scene> mScene;
					QMultiMap< QPoint, QGraphicsItem* > mTiles;
			};
                        Scene( QPoint _center, QSize _view, resource::Facet _facet, QObject* parent);
//			void connectNotify( const char* _signal );
		private:
			resource::Facet mFacet;
			QMap< QPoint, QWeakPointer<Block> > mBlocks;
			QCache< QPoint, Block > mCache;
	};
}

inline bool operator<(const QPoint& _p1, const QPoint& _p2) {
	return ( (_p1.x()<_p2.x()&&_p1.y()<=_p2.y()) || (_p1.x()<=_p2.x()&&_p1.y()<_p2.y()) ) ? true : false;
}
#endif
