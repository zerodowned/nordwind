/*
 * StaticEntity.cpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */
#include "game/StaticEntity.hpp"
#include "Client.hpp"

using namespace game;

StaticEntity::StaticEntity( QPoint _offset, resource::Facets::StaticTile& _rawData )
: Entity(_offset + QPoint(_rawData.mXOffset,_rawData.mYOffset), _rawData.mZ ),
  mRawData(_rawData) {
	mTile = Client::getInstance()->mResources->getTile(_rawData.mID,_rawData.mHueID);
	if(!mTile.isNull()) {
		setPixmap(mTile->mArt);
                setOffset((mTile->mArt.width()>44) ? (mTile->mArt.width()-44)/-2.0f : 0.0f,
                          -mTile->mArt.height());
		qreal layerModifier = 0.0f;
		if(mTile->mInfo.mFlags.testFlag(resource::TileData::Surface))
			layerModifier -= 0.5f;
		if(mTile->mInfo.mFlags.testFlag(resource::TileData::Roof))
			layerModifier += 0.5f;
		if(mTile->mInfo.mFlags.testFlag(resource::TileData::Wall))
                        layerModifier += 0.1f;
                if(mTile->mInfo.mFlags.testFlag(resource::TileData::Foliage))
                    layerModifier += 20.0f;
        setLayerModifier(layerModifier);
        setToolTip(toolTip()
                   +QString("\nID:%1\nHue:%2(%3)\n").arg(mTile->mGID.mID).arg(mTile->mGID.mHue->getName()).arg(mTile->mGID.mHue->getID())
                   +mTile->mInfo+QString("\nz:%1").arg(zValue()) );
	}
}
