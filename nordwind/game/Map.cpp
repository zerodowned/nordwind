/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "Map.hpp"
//#include "../Data.hpp"
//#include "../data/TileData.hpp"
#include <qpainter.h>
#include <qpixmapcache.h>
#include <QGLWidget>
#include <qdebug.h>

using namespace game;

inline qint16 strech(Z _z1, Z _z2) {
	return qAbs((_z1 - _z2) * 4 + 22);
}

Map::Map(ID id, Z z, Z southZ, Z eastZ, Z downZ) :
	QGraphicsItem(), mID(id), mZ(z), mTexture(QPixmap(
			QString("art.mul:%1").arg(mID), "map")), mSouthZ(southZ), mEastZ(
			eastZ), mDownZ(downZ) {
	//    setToolTip(toolTip()+(QString)*this+QString("%1 %2 %3 %4").arg(mStl).arg(mHl).arg(mStr).arg(mHr));
}

Map::~Map() {
}

QRectF Map::boundingRect() const {
	return QRectF(0.0f, 0.0f, 44.0f, qMax(strech(mZ, mSouthZ) + strech(mSouthZ,
			mDownZ), strech(mZ, mEastZ) + strech(mEastZ, mDownZ)));
}

void Map::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
		QWidget* widget) {
	if (mID == 0x2 || mTexture.isNull())
		return;
	qint16 stl = strech(mZ, mSouthZ);
	qint16 str = strech(mZ, mEastZ);
	qint16 hl = stl+strech(mSouthZ,mDownZ);
	qint16 hr = str+strech(mEastZ,mDownZ);
// 	if (painter->paintEngine()->type() == QPaintEngine::OpenGL
// 			|| painter->paintEngine()->type() == QPaintEngine::OpenGL2) {
// 		QGLWidget* glwidget = qobject_cast<QGLWidget*> (widget);
// 		//QGLContext* context = glwidget->context();
// 		painter->beginNativePainting();
// 		GLuint tex = glwidget->bindTexture(mTexture);
// 		glwidget->drawTexture(QPointF(0,0), tex);
// 		glBindTexture(GL_TEXTURE_2D, tex);
// 		glBegin(GL_QUADS);
// 		{
// 			glTexCoord2f(0.0f, 0.5f);
// 			glVertex2f(0.0f,stl); // left
// 
// 			glTexCoord2f(.5f, 1.0f);
// 			glVertex2f(22.0f,qMax(hl,hr)); // bottom
// 
// 			glTexCoord2f(1.0f, 0.5f);
// 			glVertex2f(44.0f,str); // right
// 
// 			glTexCoord2f(0.5f, 0.0f);
// 			glVertex2f(22.0f,0.0f); // top
// 		}
// 		glEnd();
// 		
// 		painter->endNativePainting();
// 	} else {
		// 44x44 raster paint backup without texture
		painter->drawPixmap(0, 0, 22, stl, mTexture, 0, 0, 22, 22); // left top
		painter->drawPixmap(0, stl, 22, hl - stl, mTexture, 0, 22, 22, 22); // left bottom
		painter->drawPixmap(22, 0, 22, str, mTexture, 22, 0, 22, 22); // right top
		painter->drawPixmap(22, str, 22, hr - str, mTexture, 22, 22, 22, 22); // right bottom
//	}
	// lines
	const QPointF indices[6] = { QPointF(22.0f, 0.0f), QPointF(0.0f, stl),
		QPointF(22.0f, hl), QPointF(22.0f,
		0.0f), QPointF(22.0f, hr),
		QPointF(44.0f, str) };
	painter->drawConvexPolygon(indices, 3);
	painter->drawConvexPolygon(indices + 3, 3);
}

//QImage Map::load(Z southZ, Z eastZ, Z downZ) const {
//
//	if(qMax(hl,hr)<=0)
//	return;
//	const data::Info& info(data::TileData::instance().at(mD.mID));
//	QString artID = Data::resource(Data::Art,mD.mID);
//	ID tID = info.textureID();
//	QString texID = Data::resource(Data::Texture,tID);
//	QString key = QString(":%1:%2:%3:%4").arg(stl).arg(hl).arg(hr).arg(str);
//	// 	if(tID&&!QPixmapCache::find(texID+key)&&texture.load(texID,"texture")) {
//	// 		QImage image(44,qMax(mHl,mHr),QImage::Format_ARGB32_Premultiplied);
//	// 		image.fill(0);
//	// 		QPainter painter;
//	// 		if(!painter.begin(&image))
//	// 			qWarning() << "Unable to start QPainter";
//	// 		painter.setBrush(texture);
//	// 		painter.drawConvexPolygon(QPolygon() << QPoint(22,0) << QPoint(44,mStr) << QPoint(22,mHr));
//	// 		painter.drawConvexPolygon(QPolygon() << QPoint(22,0) << QPoint(0,mStl) << QPoint(22,mHl));
//	// 		painter.drawPolyline(QPolygon() << QPoint(22,0) << QPoint(0,mStl) << QPoint(22,mHl) << QPoint(22,0) << QPoint(44,mStr) << QPoint(22,mHr) << QPoint(22,0));
//	// 		painter.end();
//	// 		texture = QPixmap::fromImage(image);
//	// 		QPixmapCache::insert(texID+key,texture);
//	// 	} else {
//	texture.load(artID,"map");
//	if(!(stl==22&&str==22&&hl==44&&hr==44)&&
//		!texture.isNull()&&
//		!QPixmapCache::find(artID+key,texture)) {
//			QImage image(44,qMax(hl,hr),QImage::Format_ARGB32_Premultiplied);
//			image.fill(0);
//			QPainter painter;
//			if(!painter.begin(&image))
//				qFatal("Unable to start QPainter");
//			painter.drawPixmap( QRect(0,0,22,stl),
//				texture,
//				QRect(0,0,22,22)); // left top
//			painter.drawPixmap( QRect(0,stl,22,hl-stl),
//				texture,
//				QRect(0,22,22,22)); // left bottom
//			painter.drawPixmap( QRect(22,0,22,str),
//				texture,
//				QRect(22,0,22,22)); // right top
//			painter.drawPixmap( QRect(22,str,22,hr-str),
//				texture,
//				QRect(22,22,22,22)); // right bottom
//			//painter.drawPolyline(QPolygon() << QPoint(22,0) << QPoint(0,mStl) << QPoint(22,mHl) << QPoint(22,0) << QPoint(44,mStr) << QPoint(22,mHr) << QPoint(22,0));
//			painter.end();
//			texture = QPixmap::fromImage(image);
//			QPixmapCache::insert(artID+key,texture);
//	}
//	//}
//}
