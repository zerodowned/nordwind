/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "game/MapEntity.hpp"
#include "Client.hpp"
#include <qpainter.h>
#include <qvector2d.h>
#include <qpixmapcache.h>

using namespace game;

MapEntity::MapEntity( QPoint _position,
		resource::Facets::MapTile& _data,
		const resource::Facets::MapTile& _south,
		const resource::Facets::MapTile& _down,
		const resource::Facets::MapTile& _east)
: Entity( QVector3D(QVector2D(_position), _data.mZ), -5.0f),
  mData(_data) {
	if(_data.mID==0x2)
		return;
	mStl = calculateStrech(mData.mZ,_south.mZ);
	mStr = calculateStrech(mData.mZ,_east.mZ);
	mHl = calculateStrech(_south.mZ,_down.mZ);
	mHl += mStl;
	mHr = calculateStrech(_east.mZ,_down.mZ);
	mHr += mStr;

	mLand = Client::getInstance()->mResources->getLand(_data.mID);
//	if(mInfo->getTextureID()!=0)
//		mTexture = Client::getInstance()->mResources->getTexture(mInfo->getTextureID());
	if(mStl==22&&mStr==22&&mHl==44&&mHr==44) {
		setPixmap(mLand->mArt);
                setOffset(0.0f,-44.0f);
	} else {
		QPixmap map;
		if(!QPixmapCache::find(QString("ID%1Hue%2Stl%3Str%4Hl%5Hr%6" )
							   .arg(mLand->mGID.mID)
							   .arg(mLand->mGID.mHue->getID())
							   .arg(mStl)
							   .arg(mStr)
							   .arg(mHl)
							   .arg(mHr), map)) {
			QImage strechMap(44,qMax(mHl,mHr), QImage::Format_ARGB32_Premultiplied);
			strechMap.fill(0);
			QPainter painter;
			painter.begin(&strechMap);
			painter.drawPixmap(0,0,22,mStl,mLand->mArt,0,0,22,22); // left top
                        painter.drawPixmap(0,mStl,22,mHl-mStl,mLand->mArt,0,22,22,22); // left bottom
                        painter.drawPixmap(22,0,22,mStr,mLand->mArt,22,0,22,22); // right top
                        painter.drawPixmap(22,mStr,22,mHr-mStr,mLand->mArt,22,22,22,22); // right bottom
			painter.end();
			map = QPixmap::fromImage(strechMap);
			QPixmapCache::insert(QString("ID%1Hue%2Stl%3Str%4Hl%5Hr%6" )
							   .arg(mLand->mGID.mID)
							   .arg(mLand->mGID.mHue->getID())
							   .arg(mStl)
							   .arg(mStr)
							   .arg(mHl)
							   .arg(mHr), map);
		}
                setPixmap(map);
                setOffset(0,-44.0f);
	}
    setToolTip(toolTip()+"\n"+mLand->mInfo);
}

void MapEntity::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
	QGraphicsPixmapItem::paint(painter,option,widget);
}

quint16 MapEntity::calculateStrech( qint16 _z1, qint16 _z2 ) const {
	qint16 sum = (_z1-_z2)*4+22;
	qint16 abs = qAbs(sum);
	quint16 result = abs;
	return result;
}
