#include "game/Scene.hpp"
#include "game/MapEntity.hpp"

using namespace game;

Scene::Scene( resource::Facet _facet, QObject* _parent)
: QGraphicsScene(_parent),
  mFacet(_facet) {
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
	quint8 c = 0;
	foreach( resource::Facets::MapTile mapTile, mRawBlock->first ) {
		QGraphicsItem* tmp = new MapEntity(mRawBlock->getOffset(),c%8,c/8,mapTile);
		QSharedPointer<QGraphicsItem> item( tmp  );
		mTiles[c%8][c/8].push_back( item );
		_scene->addItem( item.data() );
		c++;
	}
}
