/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "game/Map.hpp"
#include "Client.hpp"
#include <qpainter.h>
#include <qpixmapcache.h>

using namespace game;

Map::Map( Coordinate _position, ID _id, qint32 _southZ, qint32 _downZ, qint32 _eastZ )
: Entity( _position, -5.0f),
  mLand(Client::getInstance()->resources()->getLand(_id)) {

	if(/*_id==0x2 ||*/ mLand.isNull())
		return;

	qint16 mStl = calculateStrech(_position.z(),_southZ);
	qint16 mStr = calculateStrech(_position.z(),_eastZ);
	qint16 mHl = mStl+calculateStrech(_southZ,_downZ);
	qint16 mHr = mStr+calculateStrech(_eastZ,_downZ);

	if(mStl==22&&mStr==22&&mHl==44&&mHr==44) {
		setPixmap(mLand->getArt());
	} else {
		QPixmap map;
		if(!QPixmapCache::find(QByteArray::number(_id)+QByteArray::number(mStl)+QByteArray::number(mHl)+QByteArray::number(mStr)+QByteArray::number(mHr),map)) {
			QImage strechMap(44,qMax(mHl,mHr), QImage::Format_ARGB32_Premultiplied);
			strechMap.fill(0);

			QPainter painter;
			painter.begin(&strechMap);
			if(mLand->getTexture().isNull()) {
				QPixmap src = mLand->getArt();
				painter.drawPixmap( QRect(0,0,22,mStl),
									src,
									QRect(0,0,22,22)); // left top
				painter.drawPixmap( QRect(0,mStl,22,mHl-mStl),
									src,
									QRect(0,22,22,22)); // left bottom
				painter.drawPixmap( QRect(22,0,22,mStr),
									src,
									QRect(22,0,22,22)); // right top
				painter.drawPixmap( QRect(22,mStr,22,mHr-mStr),
									src,
									QRect(22,22,22,22)); // right bottom
			} else {
				QPolygon polygon;
				polygon << QPoint(22,0)
						<< QPoint(0,mStl)
						<< QPoint(22,mHl)
						<< QPoint(22,0)
						<< QPoint(44,mStr)
						<< QPoint(22,mHr);
				painter.setBrush(mLand->getTexture());
				painter.drawConvexPolygon(polygon);
			}
			painter.end();
			map = QPixmap::fromImage(strechMap);
			QPixmapCache::insert(QByteArray::number(_id)+QByteArray::number(mStl)+QByteArray::number(mHl)+QByteArray::number(mStr)+QByteArray::number(mHr), map);
		}
		setPixmap(map);
	}
//	if(mInfo->getTextureID()!=0)
//		mTexture = Client::getInstance()->mResources->getTexture(mInfo->getTextureID());
//	setOffset(0.0f,-44.0f);
    setToolTip(toolTip()+(QString)*this+QString("%1 %2 %3 %4").arg(mStl).arg(mHl).arg(mStr).arg(mHr));
}

Map::operator QString() const {
	return QString("%1\n"
				   "highLeft:%2\n"
				   "highRight:%3\n"
				   "strechLeft:%4\n"
				   "strechRight:%5\n")
				   .arg(*mLand);
}
