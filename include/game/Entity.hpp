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
			class Position {
				public:
					Position();
					Position(quint16 _x, quint16 _y, qint16 _z);
					quint16 mX;
					quint16 mY;
					qint16 mZ;
					qint16 mLayerModifier;
					QPointF toScenePos() const;
					qreal getLayer() const;
			};
			Entity( quint16 _x, quint16 _y, qint16 _z );
			virtual ~Entity();
			const Position& position() const;
			Position& position();
		private:
			Position mPosition;
	};

	inline Entity::Position::Position()
	: mX(0),
	  mY(0),
	  mZ(0),
	  mLayerModifier(0) {
	}

	inline Entity::Position::Position(quint16 _x, quint16 _y, qint16 _z)
	: mX(_x),
	  mY(_y),
	  mZ(_z),
	  mLayerModifier(0) {
	}

	inline QPointF Entity::Position::toScenePos() const {
		return QPointF( 22.0f*(mX-mY), 22.0f*(mX+mY)-4.0f*mZ );
	}

	inline qreal Entity::Position::getLayer() const {
		return mZ + mX/6144.0f + mY/4096.0f;
	}

	inline Entity::Entity( quint16 _x, quint16 _y, qint16 _z )
	: QGraphicsPixmapItem(),
	  mPosition(_x,_y,_z) {
		setPos(mPosition.toScenePos());
		setZValue(mPosition.getLayer());
	}

	inline Entity::~Entity() {
	}

	inline const Entity::Position& Entity::position() const {
		return mPosition;
	}

	inline Entity::Position& Entity::position() {
		return mPosition;
	}
}

#endif /* ENTITY_HPP_ */
