/*
 * Entity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <qgraphicsitem.h>
#include <qvector3d.h>

namespace game {
	class Entity : public QGraphicsPixmapItem {
		public:
			Entity( QVector3D _pos = QVector3D(0.0f,0.0f,0.0f), qreal _layerModifier = 0.0f );
			QVector3D position() const;
			Entity& setPosition( QVector3D _position );
			Entity& setLayerModifier( qreal _layerModifier );
		protected:
			Entity& updatePosition();
			Entity& updateZValue();
			qreal calcZLayer() const;
			QPointF toScenePos() const;
			QVector3D mPosition;
			qreal mLayerModifier;
	};

	inline Entity::Entity( QVector3D _pos, qreal _layerModifier )
	: QGraphicsPixmapItem(),
	  mPosition(_pos),
	  mLayerModifier(_layerModifier) {
            updatePosition();
            updateZValue();
            setVisible(true);
            setToolTip(QString("(%1|%2|%3)").arg(mPosition.x()).arg(mPosition.y()).arg(mPosition.z()));
	}

	inline QVector3D Entity::position() const {
		return mPosition;
	}

	inline Entity& Entity::setPosition( QVector3D _pos ) {
		mPosition = _pos;
		return updatePosition();
	}

	inline Entity& Entity::setLayerModifier( qreal _layerModifier ) {
		mLayerModifier = _layerModifier;
		return updatePosition();
	}

	inline Entity& Entity::updatePosition() {
		setPos(toScenePos());
		return updateZValue();
	}

	inline Entity& Entity::updateZValue() {
		setZValue(calcZLayer());
		return *this;
	}

	inline QPointF Entity::toScenePos() const {
		return QPointF( 22.0f*(mPosition.x()-mPosition.y()),
				22.0f*(mPosition.x()+mPosition.y())-4.0f*mPosition.z() );
	}

	inline qreal Entity::calcZLayer() const {
		return mPosition.z() +
				mPosition.x()*4.0f +
				mPosition.y()*4.0f +
				mLayerModifier;
	}
}

#endif /* ENTITY_HPP_ */
