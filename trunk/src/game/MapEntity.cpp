/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "game/MapEntity.hpp"
#include "Client.hpp"

using namespace game;

MapEntity::MapEntity( QPoint _offset, quint8 _x, quint8 _y, resource::Facets::MapTile& _data )
: Entity(_offset.x()+_x,_offset.y()+_y, _data.mZ),
  mData(_data),
  mArt(Client::getInstance()->mResources->getLandArt(_data.mID)),
  mInfo(Client::getInstance()->mResources->getTileInfo(_data.mID).staticCast<resource::TileData::LandInfo>()) {
	if(mInfo->getTextureID()!=0)
		mTexture = Client::getInstance()->mResources->getTexture(mInfo->getTextureID());
	setPixmap(*(mArt->getImage()));
}

MapEntity::~MapEntity() {
}

void MapEntity::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
	QGraphicsPixmapItem::paint(painter,option,widget);
}
