/*
 * StaticEntity.cpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */
#include "Static.hpp"
#include "Client.hpp"
#include "core/Resources.hpp"
#include <qdebug.h>
#include <qdatetime.h>

using namespace game;

Static::Static( Coordinate _position, ID id, Hue hue )
: Entity( _position ),
  mID(id),
  mHue(hue) {
        QTime time;
        qDebug() << "Loading" << resource::Resources::artFile()+":"+QString::number(id+0x4000);
        time.start();
	QPixmap texture(resource::Resources::artFile()+":"+QString::number(id+0x4000),"art");
        qDebug() << "Load finished after" << time.elapsed() << "ms";
	if(texture.isNull())
		return;
	if(pixmap().width()>44)
		setX(x()-pixmap().width()/2+22);
	setY(y()-(pixmap().height()-44));

	const resource::Info& info(Client::getInstance()->resources().tileData().at(id+0x4000));
	qreal layer = 0.0f;
	resource::Info::Flags flags(info.flags());
	if(flags&resource::Info::Surface)
		layer -= 0.5f;
//		if(mTile->getInfo().mFlags.testFlag(resource::TileData::Roof))
//			layer += 0.5f;
//		if(mTile->getInfo().mFlags.testFlag(resource::TileData::Wall))
//			layer += 0.1f;
	if(flags&resource::Info::Foliage)
		layer += 15.0f;
	setLayer(layer);

	if(flags&(resource::Info::Transparent|resource::Info::Translucent))
		setOpacity(0.5f);
}
