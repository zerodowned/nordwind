/*
 * MapEntity.hpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#ifndef MAPENTITY_HPP_
#define MAPENTITY_HPP_

#include "Entity.hpp"
#include "../core/Resources.hpp"
#include "../resource/Facets.hpp"

namespace game {

	class Map : public Entity {
		public:
			Map( Coordinate _position, ID _id, qint32 _southZ, qint32 _downZ, qint32 _eastZ );
//			void paint( QPainter * painter, const QStyleOptionGraphicsItem * option,QWidget * widget = 0 );
			operator QString() const;
		protected:
			qint16 calculateStrech( qint16 _z1, qint16 _z2 ) const;
		private:
			resource::Land mLand;
			QPolygon mPolygon;
	};

	inline qint16 Map::calculateStrech( qint16 _z1, qint16 _z2 ) const {
		return (_z1-_z2)*4+22;
	}
}


#endif /* MAPENTITY_HPP_ */
