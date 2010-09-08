/*
 * Entity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <qgraphicsitem.h>
#include "Coordinate.hpp"

namespace game {
	class Entity : public QGraphicsPixmapItem {
		public:
			Entity( const Coordinate& _position, qreal _layer = 0.0f);
			qreal getLayer() const;
			Entity& setLayer(qreal _layer);
			const Coordinate& getPosition() const;
			Entity& setPosition( const Coordinate& _position );
		protected:
			QVariant itemChange(GraphicsItemChange _change, const QVariant & _value);
//			void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
//			void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
		private:
			Coordinate mPosition;
			qreal mLayer;
	};

	inline qreal Entity::getLayer() const {
		return mLayer;
	}

	inline Entity& Entity::setLayer(qreal _layer) {
		mLayer = _layer;
		return *this;
	}

	inline const Coordinate& Entity::getPosition() const {
			return mPosition;
	}

	inline Entity& Entity::setPosition( const Coordinate& _position) {
		mPosition = _position;
		return *this;
	}

//	inline void Entity::hoverEnterEvent(QGraphicsSceneHoverEvent* _event) {
//		graphicsEffect()->setEnabled(true);
//	}
//
//	inline void Entity::hoverLeaveEvent(QGraphicsSceneHoverEvent* _event) {
//		graphicsEffect()->setEnabled(false);
//	}
}

#endif /* ENTITY_HPP_ */
