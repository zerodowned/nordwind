/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "Map.hpp"
#include "Client.hpp"
#include <qpainter.h>
#include <qpixmapcache.h>
#include "core/Resources.hpp"

using namespace game;

Map::Map( Coordinate _position, ID _id, Z _southZ, Z _downZ, Z _eastZ )
: Entity( _position, -5.0f),
  mID(_id) {
//	if(_id==0x2)
//		return;

	qint16 mStl = calculateStrech(_position.z(),_southZ);
	qint16 mStr = calculateStrech(_position.z(),_eastZ);
	qint16 mHl = mStl+calculateStrech(_southZ,_downZ);
	qint16 mHr = mStr+calculateStrech(_eastZ,_downZ);
	QString artID = resource::Resources::artFile() + ":" + QString::number(_id);
	QString texID = resource::Resources::textureFile() + ":" + QString::number(_id);
	QString key = QString(":%2:%3:%4:%5").arg(mID).arg(mStl).arg(mHl).arg(mHr).arg(mStr);
	QPixmap texture;
	if(!QPixmapCache::find(texID+key)&&texture.load(texID,"texture")) {
		QPolygon polygon;
		polygon << QPoint(22,0)
				<< QPoint(0,mStl)
				<< QPoint(22,mHl)
				<< QPoint(22,0)
				<< QPoint(44,mStr)
				<< QPoint(22,mHr);
		QPixmap strechMap(44,qMax(mHl,mHr));
		if(!strechMap.isNull()) {
			QPainter painter(&strechMap);
			painter.setBrush(texture);
			painter.drawConvexPolygon(polygon);
		}
		QPixmapCache::insert(texID+key,strechMap);
		setPixmap(strechMap);
	} else {
		texture.load(artID,"art");
		if(!(mStl==22&&mStr==22&&mHl==44&&mHr==44)&&
		   !QPixmapCache::find(artID+key,texture)) {
			QPixmap strechMap(44,qMax(mHl,mHr));
			if(!strechMap.isNull()) {
				QPainter painter(&strechMap);
				painter.drawPixmap( QRect(0,0,22,mStl),
									texture,
									QRect(0,0,22,22)); // left top
				painter.drawPixmap( QRect(0,mStl,22,mHl-mStl),
									texture,
									QRect(0,22,22,22)); // left bottom
				painter.drawPixmap( QRect(22,0,22,mStr),
									texture,
									QRect(22,0,22,22)); // right top
				painter.drawPixmap( QRect(22,mStr,22,mHr-mStr),
									texture,
									QRect(22,22,22,22)); // right bottom
			}
			QPixmapCache::insert(artID+key,strechMap);
			setPixmap(strechMap);
		} else {
			setPixmap(texture);
		}
	}
//    setToolTip(toolTip()+(QString)*this+QString("%1 %2 %3 %4").arg(mStl).arg(mHl).arg(mStr).arg(mHr));
}
