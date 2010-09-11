/*
 * StaticEntity.cpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */
#include "Static.hpp"
#include "../Client.hpp"
#include "../core/Resources.hpp"
#include <qdebug.h>
#include <qdatetime.h>

using namespace game;

Static::Static( Coordinate _position, ID id, Hue hue )
: Entity( _position ),
  mID(id),
  mHue(hue) {
	QPixmap texture(resource::Resources::artFile()+":"+QString::number(id+0x4000),"static");
	if(texture.isNull())
		return;

	const resource::Info& info(Client::getInstance()->resources().tileData().at(id+0x4000));
	qreal layer = 0.0f;
	resource::Info::Flags flags(info.flags());
	if(flags&resource::Info::Surface)
		layer -= .9f;
// 	if(flags&resource::Info::Roof)
// 		layer += 0.5f;
	if(flags&resource::Info::Wall)
		layer += 4.0f;
// 	if(flags&resource::Info::Foliage)
// 		layer += 15.0f;
	setZValue(layer);

 	//if(flags&(resource::Info::Transparent)
	// does not fade on transparent circle
	if(flags&resource::Info::Translucent)
 		setOpacity(0.9f);
	setPixmap(texture);
}

void Static::setPixmap(const QPixmap &pixmap) {
	setOffset(pixmap.width()>44?(44-pixmap.width())/2:0,44-pixmap.height());
	QGraphicsPixmapItem::setPixmap(pixmap);		
}