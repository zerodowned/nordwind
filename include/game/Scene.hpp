#ifndef SCENE_HPP__
#define SCENE_HPP__

#include <qgraphicsscene.h>
#include <qmap.h>
#include "../resource/Facets.hpp"

namespace game {
	class Scene : public QGraphicsScene
	{
		Q_OBJECT;
	public:
		class Block {
			public:
				Block( QSharedPointer<resource::Facets::Block> _rawBlock );
				virtual ~Block();
				void load( QGraphicsScene* _scene );
			private:
				QSharedPointer<resource::Facets::Block> mRawBlock;
				QVector< QVector< QList< QSharedPointer<QGraphicsItem> > > > mTiles;
		};
		Scene( resource::Facet _facet, QObject* parent);
		virtual ~Scene();
	private:
		resource::Facet mFacet;
		QList<Block*> mBlocks;
	};
}
#endif
