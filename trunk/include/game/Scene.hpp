#ifndef SCENE_HPP__
#define SCENE_HPP__

#include <qgraphicsscene.h>
#include <qmap.h>
#include "../resource/Facets.hpp"

namespace game {
	class Scene;
	class Scene : protected QGraphicsScene
	{
		Q_OBJECT;
		public:
			class Block : public QObject {
				public:
					Block( QPointer<Scene> _scene, QSharedPointer<resource::Block> _rawBlock );
					virtual ~Block();
					void moveToCache();
					void load( QGraphicsScene* _scene );
				private:
					QPointer<Scene> mScene;
					QMultiMap< QPoint, QSharedPointer<QGraphicsItem> > mTiles;
			};
			Scene( resource::Facet _facet, QObject* parent);
			virtual ~Scene();
			void connectNotify( const char* _signal );
		private:
			resource::Facet mFacet;
			QMap< QPoint, QWeakPointer<Block> > mBlocks;
			QCache< QPoint, Block > mCache;
	};
}
#endif
