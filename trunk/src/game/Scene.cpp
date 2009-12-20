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
                QSharedPointer<resource::Facets::Block> block( mFacet->getBlock(point) );
                mBlocks[point] = new Scene::Block( this, block, this );
            }
	}
//        setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

Scene::Block::Block( QPointer<Scene> _scene, const QSharedPointer<resource::Facets::Block>& _rawBlock, QObject* _parent )
: mScene(_scene) {
        quint8 i = 0;
        foreach( resource::Facets::MapTile mapTile, _rawBlock->getMapTiles() ) {
                QPoint point(i%8,i/8);
                QGraphicsItem* item = new MapEntity(_rawBlock->getOffset(),QPoint(i%8,i/8),mapTile);
                _scene->addItem( item );
                mTiles.insert( point, item );
                i++;
        }
	foreach( resource::Facets::StaticTile staticTile, _rawBlock->getStaticTiles() ) {
		QGraphicsItem* item = new StaticEntity(_rawBlock->getOffset(),staticTile);
		_scene->addItem( item );
		mTiles.insert( QPoint(staticTile.mXOffset,staticTile.mYOffset), item );
	}
}

Scene::Block::~Block() {
    foreach( QGraphicsItem* item, mTiles ) {
        if(item)
            delete item;
    }
}
