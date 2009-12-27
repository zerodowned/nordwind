#include "game/Scene.hpp"
#include "game/Map.hpp"
#include "game/Static.hpp"

using namespace game;

Scene::Scene( resource::Facet _facet, QObject* _parent )
: QGraphicsScene(_parent),
  mFacet(_facet) {
}

Scene& Scene::loadMap(const QPoint& _center,const QSize& _dimension) {
	QPoint offset(qMax((_center.x()-(_dimension.width()>>1))>>3,0),
				  qMax((_center.y()-(_dimension.height()>>1))>>3,0));
	for(quint16 y = 0; y<(_dimension.height()>>3); y++ ) {
		for(quint16 x = 0; x <(_dimension.width()>>3); x++ ) {
			QPoint point = offset + QPoint(x,y);
			constructBlock(point);
		}
	}
	return *this;
}

Scene& Scene::constructBlock( QPoint _blockPos ) {
	Block* block = new Block;
	resource::Block rawBlock(mFacet->getBlock(_blockPos));
		resource::Facets::MapTiles mapTiles = rawBlock.getMapTiles();
		QPoint maxOffset(mFacet->getBlockSize().width(),mFacet->getBlockSize().height());
		foreach( QPoint cellOffset, mapTiles.keys() ) {
			if(cellOffset<maxOffset) {
				QPoint realPosition = rawBlock.getOffset() + cellOffset;
				resource::Facets::MapTile tile( mapTiles[cellOffset] );
				QSharedPointer<Entity> map( new Map(Coordinate(realPosition,tile.mZ),
													tile.mID,
													mapTiles[cellOffset+QPoint(0,1)].mZ, // South
													mapTiles[cellOffset+QPoint(1,1)].mZ, // Down
													mapTiles[cellOffset+QPoint(1,0)].mZ )); // East
				addItem(map.data());
				block->insert( cellOffset, map );
			}
		}
//		foreach( resource::Facets::StaticTile staticTile, rawBlock.getStaticTiles() ) {
//			QPoint cellOffset(staticTile.mXOffset,staticTile.mYOffset);
//			QSharedPointer<Entity> item( new Static( Coordinate(cellOffset + rawBlock.getOffset(),staticTile.mZ),staticTile.mID,staticTile.mHueID) );
//			addItem(item.data());
//			block->insert( cellOffset, item );
//		}
	return *this;
}

void Scene::connectNotify(const char* _signal) {
	if(QLatin1String(_signal)==SLOT(changed(const QList<QRectF>&))) {

	}
}
