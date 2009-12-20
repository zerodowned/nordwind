#include "game/Scene.hpp"
#include "game/MapEntity.hpp"
#include "game/StaticEntity.hpp"

using namespace game;

Scene::Scene( QPoint _center, QSize _view, resource::Facet _facet, QObject* _parent )
: QGraphicsScene(_parent),
  mFacet(_facet) {
    setItemIndexMethod(QGraphicsScene::NoIndex);
    QPoint offset(qMax((_center.x()-(_view.width()>>1))>>3,0),
    		qMax((_center.y()-(_view.height()>>1))>>3,0));
    for(quint16 y = 0; y<(_view.height()>>3); y++ ) {
    	for(quint16 x = 0; x <(_view.width()>>3); x++ ) {
    		QPoint point = offset + QPoint(x,y);
    		constructBlock(point);
    	}
    }
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

resource::Facets::MapTile Scene::getNeighbour( const QPoint& _position, const resource::Facets::MapTile& _fallback ) {
	resource::Block rawBlock(mFacet->getBlock(mFacet->fromCellToBlockPosition(_position)));
	if(rawBlock.getMapTiles().size()==64) {
                return rawBlock.getMapTiles()[((_position.y()%8)*8)+(_position.x()%8)];
	} else {
		return _fallback;
	}
}

Scene& Scene::constructBlock( QPoint _blockPos ) {
	Block* block = new Block;
	resource::Block rawBlock(mFacet->getBlock(_blockPos));
	quint8 i = 0;
	foreach( resource::Facets::MapTile mapTile, rawBlock.getMapTiles() ) {
		QPoint realPosition = rawBlock.getOffset() + QPoint(i%8,i/8);
		QSharedPointer<Entity> map( new MapEntity(realPosition,
											mapTile,
											getNeighbour(realPosition + QPoint(0,1),mapTile), // South
											getNeighbour(realPosition + QPoint(1,1),mapTile), // Down
											getNeighbour(realPosition + QPoint(1,0),mapTile) )); // East
		block->insert( QPoint(i%8,i/8), map );
		i++;
	}
	foreach( resource::Facets::StaticTile staticTile, rawBlock.getStaticTiles() ) {
		QSharedPointer<Entity> item( new StaticEntity(rawBlock.getOffset(),staticTile) );
		block->insert( QPoint(staticTile.mXOffset,staticTile.mYOffset), item );
	}
	addToScene(block);
	return *this;
}

Scene& Scene::addToScene( Block* _block ) {
	foreach( QSharedPointer<Entity> item, _block->values() ) {
		addItem(item.data());
	}
	return *this;
}

void Scene::connectNotify(const char* _signal) {
	if(QLatin1String(_signal)==SLOT(changed(const QList<QRectF>&))) {

	}
}
