/*
 * Entity.cpp
 *
 *  Created on: 24.12.2009
 *      Author: idstein
 */
#include "Entity.hpp"

using namespace game;

Entity::Entity( const Coordinate& _position, qreal _layer)
: QGraphicsPixmapItem(),
  mPosition(_position),
  mLayer(_layer) {
//	setAcceptHoverEvents(true);
	setToolTip(mPosition);
//	setFlags(ItemIsSelectable|ItemIsMovable);
	setPos(mPosition.toScenePoint());
	QGraphicsPixmapItem::setZValue(mPosition.toSceneZValue());
	setVisible(true);
}

void Entity::setZValue ( qreal z ) {
	QGraphicsPixmapItem::setZValue(mPosition.toSceneZValue()+z);
}

QVariant Entity::itemChange(GraphicsItemChange _change, const QVariant& _value) {
	switch(_change) {
		case ItemZValueChange: {
			return _value.toReal()+mLayer;
		}
		default:
			return QGraphicsPixmapItem::itemChange(_change,_value);
	}
}

//void Entity::setSelected(bool _selected) {
//	QGraphicsPixmapItem::setSelected(_selected);
//	if(isSelected()) {
//		QGraphicsColorizeEffect* hoverEffect = new QGraphicsColorizeEffect();
//		hoverEffect->setColor(Qt::yellow);
//		hoverEffect->setStrength(0.5f);
//		setGraphicsEffect(hoverEffect);
//	} else {
//		if(graphicsEffect()&&qobject_cast<QGraphicsColorizeEffect*>(graphicsEffect())) {
//			setGraphicsEffect(NULL);
//		}
//	}
//}
