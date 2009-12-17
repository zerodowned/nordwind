#include "game/Scene.hpp"
#include "game/MapEntity.hpp"

using namespace game;

Scene::Scene( resource::Facet _facet, QObject* _parent)
: QGraphicsScene(_parent),
  mFacet(_facet) {
	for(quint8 x = 0; x < 8; x ++ ) {
		for(quint8 y = 0; y < 8; y ++ ) {
			QSharedPointer<Scene::Block> block( new Scene::Block( mFacet->getBlock(QPoint(x,y))) );
			block->load(this);
			mBlocks.push_back( block );
		}
	}
}

Scene::~Scene() {
}

Scene::Block::Block( QSharedPointer<resource::Facets::Block> _rawBlock )
: mRawBlock(_rawBlock) {
	mTiles.resize(8);
	foreach( QVector< QList< QSharedPointer<QGraphicsItem> > > vec, mTiles ) {
		vec.resize(8);    
	}
}

Scene::Block::~Block() {
}

void Scene::Block::load( QGraphicsScene* _scene ) {
	Q_ASSERT(_scene!=NULL);
        mTiles.resize(8);
        for( quint8 x = 0; x < 8; x++ ) {
            mTiles[x].resize(8);
            for( quint8 y = 0; y < 8; y++ ) {
                QList< QSharedPointer<QGraphicsItem> > list;
                QGraphicsItem* tmp = new MapEntity(mRawBlock->getOffset(),x,y,mRawBlock->first[y*8+x]);
                list.push_back( QSharedPointer<QGraphicsItem>( tmp  ) );
                _scene->addItem( tmp );
                mTiles[x][y] = list;
            }
        }
}
