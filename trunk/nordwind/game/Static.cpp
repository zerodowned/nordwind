/*
 * StaticEntity.cpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */
#include "Static.hpp"
#include "../Data.hpp"
#include "../data/TileData.hpp"
#include <qdebug.h>

using namespace game;

Static::Static(ID id, Z z)
: QGraphicsPixmapItem(QPixmap(QString("art.mul:%1").arg(id+0x4000),"static")),
  mID(id),
  mZ(z) {
	setOffset(pixmap().width()>44?(44-pixmap().width())/2:0,44-pixmap().height());
	const data::Info& info = data::TileData::instance().at(mID+0x4000);
	qreal layer = 0.0f;
	data::Info::Flags flags(info.flags());
// 	if(flags&data::Info::Surface)
// 		layer -= .9f;
// 	if(flags&resource::Info::Roof)
// 		layer += 0.5f;
// 	if(flags&data::Info::Wall)
// 		layer += 4.0f;
// 	if(flags&resource::Info::Foliage)
// 		layer += 15.0f;
	//setZValue(layer);

 	//if(flags&(resource::Info::Transparent)
	// does not fade on transparent circle
	if(flags&data::Info::Translucent)
 		setOpacity(0.9f);
}
