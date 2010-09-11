/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "Map.hpp"
#include "../Client.hpp"
#include <qpainter.h>
#include <qpixmapcache.h>
#include <qdebug.h>
#include "../core/Resources.hpp"

using namespace game;

Map::Map( const QPoint& p, ID _id, Z _z, Z _southZ, Z _downZ, Z _eastZ )
: Entity( Coordinate(p,_z), -5.0f),
  mID(_id) {
	if(_id==0x2)
		return;

	qint16 mStl = calculateStrech(_z,_southZ);
	qint16 mStr = calculateStrech(_z,_eastZ);
	qint16 mHl = qAbs(mStl+calculateStrech(_southZ,_downZ));
	qint16 mHr = qAbs(mStr+calculateStrech(_eastZ,_downZ));
	if(qMax(mHl,mHr)<=0)
		return;
	const resource::Info& info(Client::getInstance()->resources().tileData().at(mID));
	QString artID = resource::Resources::artFile() + ":" + QString::number(mID);
	ID tID = info.textureID();
	QString texID = resource::Resources::textureFile() + ":" + QString::number(tID);
	QString key = QString(":%1:%2:%3:%4").arg(mStl).arg(mHl).arg(mHr).arg(mStr);
	QPixmap texture;
// 	if(tID&&!QPixmapCache::find(texID+key)&&texture.load(texID,"texture")) {
// 		QImage image(44,qMax(mHl,mHr),QImage::Format_ARGB32_Premultiplied);
// 		image.fill(0);
// 		QPainter painter;
// 		if(!painter.begin(&image))
// 			qWarning() << "Unable to start QPainter";
// 		painter.setBrush(texture);
// 		painter.drawConvexPolygon(QPolygon() << QPoint(22,0) << QPoint(44,mStr) << QPoint(22,mHr));
// 		painter.drawConvexPolygon(QPolygon() << QPoint(22,0) << QPoint(0,mStl) << QPoint(22,mHl));
// 		painter.drawPolyline(QPolygon() << QPoint(22,0) << QPoint(0,mStl) << QPoint(22,mHl) << QPoint(22,0) << QPoint(44,mStr) << QPoint(22,mHr) << QPoint(22,0));
// 		painter.end();
// 		texture = QPixmap::fromImage(image);
// 		QPixmapCache::insert(texID+key,texture);
// 	} else {
		texture.load(artID,"map");
		if(!(mStl==22&&mStr==22&&mHl==44&&mHr==44)&&
			!texture.isNull()&&
		   !QPixmapCache::find(artID+key,texture)) {
			QImage image(44,qMax(mHl,mHr),QImage::Format_ARGB32_Premultiplied);
			image.fill(0);
			QPainter painter;
			if(!painter.begin(&image))
				qFatal("Unable to start QPainter");
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
			//painter.drawPolyline(QPolygon() << QPoint(22,0) << QPoint(0,mStl) << QPoint(22,mHl) << QPoint(22,0) << QPoint(44,mStr) << QPoint(22,mHr) << QPoint(22,0));
			painter.end();
			texture = QPixmap::fromImage(image);
			QPixmapCache::insert(artID+key,texture);
		}
	//}
	setPixmap(texture);
//    setToolTip(toolTip()+(QString)*this+QString("%1 %2 %3 %4").arg(mStl).arg(mHl).arg(mStr).arg(mHr));
}
