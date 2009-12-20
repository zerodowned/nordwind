/*
 * Entity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <qgraphicsitem.h>

namespace game {
	class Entity : public QGraphicsPixmapItem {
		public:
			Entity( QPoint _position = QPoint(0,0), qint16 _z = 0, qreal _layerModifier = 0.0f );
			quint32 getX() const;
			quint32 getY() const;
			qint16 getZ() const;
			void setX( quint32 _x );
			void setY( quint32 _y );
			void setZ( qint16 _z );
			void setLayerModifier( qreal _layerModifier );
		protected:
			void updatePosition();
			void updateZValue();
			qreal calcZLayer() const;
			QPointF toScenePos() const;
			QPoint mPosition;
			qint16 mZ;
			qreal mLayerModifier;
	};

	inline Entity::Entity( QPoint _position, qint16 _z, qreal _layerModifier )
	: QGraphicsPixmapItem(),
	  mPosition(_position),
	  mZ(_z),
	  mLayerModifier(_layerModifier) {
            updatePosition();
            updateZValue();
            setVisible(true);
            setToolTip(QString("(%1|%2|%3)").arg(mPosition.x()).arg(mPosition.y()).arg(mZ));
	}

	inline quint32 Entity::getX() const {
		return mPosition.x();
	}

	inline quint32 Entity::getY() const {
		return mPosition.y();
	}

	inline qint16 Entity::getZ() const {
		return mZ;
	}

	inline void Entity::setX( quint32 _x ) {
		mPosition.setX(_x);
		updatePosition();
	}

	inline void Entity::setY( quint32 _y ) {
		mPosition.setY(_y);
		updatePosition();
	}

	inline void Entity::setZ( qint16 _z ) {
		mZ = _z;
		updatePosition();
		updateZValue();
	}

	inline void Entity::setLayerModifier( qreal _layerModifier ) {
		mLayerModifier = _layerModifier;
		updateZValue();
	}

	inline void Entity::updatePosition() {
		setPos(toScenePos());
	}

	inline void Entity::updateZValue() {
		setZValue(calcZLayer());
	}

	inline QPointF Entity::toScenePos() const {
		return QPointF( 22.0f*(mPosition.x()-mPosition.y()),
				22.0f*(mPosition.manhattanLength())-4.0f*mZ );
	}

	inline qreal Entity::calcZLayer() const {
		return qreal(mZ) +
				qreal(mPosition.x())*4.0f +
				qreal(mPosition.y())*4.0f +
				mLayerModifier;
		// TODO make it more flexible by exchanging const values
	}
}

#endif /* ENTITY_HPP_ */
