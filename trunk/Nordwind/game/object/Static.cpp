/*
 * StaticEntity.cpp
 *
 *  Created on: 18.12.2009
 *      Author: idstein
 */
#include "Static.hpp"
#include "../TileData.hpp"

#include <QGraphicsColorizeEffect>
#include <QMetaEnum>

using namespace game;

Static::Static(const ID &id,const ID &hue)
    : Object(id),
    QGraphicsPixmapItem(),
mHue(hue) {
    updatePixmap(id,hue);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void Static::setID(const ID &id) {
    Object::setID(id);
    updatePixmap(id,0);
}

void Static::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter,option,widget);
}

void Static::updatePixmap(const ID &newID, const ID &newHue) {
    QPixmap pixmap(QString("arts:%1/%2")
                              .arg(mID+0x4000)
                              .arg(mHue),"art");
    setPixmap(pixmap);
    if(newID!=0)
        setOffset(pixmap.width()>=44?(44-pixmap.width())/2:0,
              44-pixmap.height());
}

ID Static::hue() const {
    return mHue;
}

void Static::setHue(const ID &hue) {
    mHue = hue;
    updatePixmap(0,hue);
}

QVariant Static::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
    switch(change) {
    case QGraphicsItem::ItemZValueChange: {
            double z = value.toDouble();
            const Info& info = TileData::instance()->statics(mID);
            z += info.height();
            const TileData::Flags flags(info.flags());
            /*if(flags&Info::Wall)
                z += 15.0f;
            if(flags&Info::Roof)
                z += 20.0;*/
            if(flags&TileData::Surface)
                z -= -1.0f;
            if(flags&TileData::Foliage)
                z += 18.0f;
            /*const QMetaObject meta = TileData::staticMetaObject;
            QMetaEnum enumerator = meta.enumerator(meta.indexOfEnumerator("Flag"));
            setToolTip(QString("Name: %1 (%6)<br>(%2,%3,%4) zValue: %5<br>Flags:%7").arg(info.name()).arg(mCoordinates.x()).arg(mCoordinates.y()).arg(mCoordinates.z()).arg(z).arg(mID).arg(QString(enumerator.valueToKeys(flags))));*/
            //if(flags&(resource::Info::Transparent)
            // does not fade on transparent circle
            //if(flags&Info::Translucent)
            //        setOpacity(0.9f);
            return z;
        }
    case QGraphicsItem::ItemSelectedHasChanged: {
            if(value.toBool()) {
                QGraphicsColorizeEffect* selection = new 	QGraphicsColorizeEffect();
                selection->setColor(Qt::yellow);
                selection->setStrength(0.5f);
                setGraphicsEffect(selection);
            } else {
                setGraphicsEffect(NULL);
            }
        }
    default:
        return value;
    }
}
