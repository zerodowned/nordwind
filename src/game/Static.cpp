/*
 * StaticEntity.cpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */
#include "game/Static.hpp"
#include "Client.hpp"

using namespace game;

Static::Static( Coordinate _position, ID _id, ID _hueID )
: Entity( _position ),
  mData(Client::getInstance()->resources()->getTile(_id,_hueID)){
	if(mData.isNull())
		return;

	setPixmap(mData->getArt());
	if(pixmap().width()>44)
		setX(x()-pixmap().width()/2+22);
	setY(y()-(pixmap().height()-44));

	qreal layer = 0.0f;
	if(mData->getInfo().mFlags.testFlag(resource::TileData::Surface))
		layer -= 0.5f;
//		if(mTile->getInfo().mFlags.testFlag(resource::TileData::Roof))
//			layer += 0.5f;
//		if(mTile->getInfo().mFlags.testFlag(resource::TileData::Wall))
//			layer += 0.1f;
	if(mData->getInfo().mFlags.testFlag(resource::TileData::Foliage))
		layer += 15.0f;
	setLayer(layer);

	if(mData->getInfo().mFlags.testFlag(resource::TileData::Transparent))
		setOpacity(0.5f);
	if(mData->getInfo().mFlags.testFlag(resource::TileData::Translucent))
		setOpacity(0.5f);
}
