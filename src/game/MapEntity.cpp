/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "game/MapEntity.hpp"
#include "Client.hpp"

using namespace game;

MapEntity::MapEntity( QPoint _offset, QPoint _pos, resource::Facets::MapTile& _data )
: Entity(_offset+_pos, _data.mZ, -4.0f),
  mData(_data) {
	mLand = Client::getInstance()->mResources->getLand(_data.mID);
//	if(mInfo->getTextureID()!=0)
//		mTexture = Client::getInstance()->mResources->getTexture(mInfo->getTextureID());
	setPixmap(mLand->mArt);
    setOffset(0.0f,-mLand->mArt.height());
    setToolTip(toolTip()+"\n"+mLand->mInfo);
}

void MapEntity::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
	QGraphicsPixmapItem::paint(painter,option,widget);
}
